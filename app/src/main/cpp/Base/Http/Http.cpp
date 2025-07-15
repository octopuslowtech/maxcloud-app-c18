//
// Created by Admin on 12/22/2024.
//

#include "Http.h"

Http::Http(string url, Http::Method method) : curl(nullptr), headers(nullptr), mime(nullptr) {
    curl = curl_easy_init();
    if (!curl) return;

    this->url = url;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // check path cacert.pem

    if (!Storage::existPath("/data/user/0/com.maxcloud.app/files/cacert.pem")) {
        Storage::copyAssetsToFiles("cacert.pem");
    }

    curl_easy_setopt(curl, CURLOPT_CAINFO, "/data/user/0/com.maxcloud.app/files/cacert.pem");

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

    curl_easy_setopt(curl, CURLOPT_USERAGENT,
                     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");

    // Bật tự động theo dõi chuyển hướng
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);

    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");

    switch (method) {
        case Method::GET:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
        case Method::POST:
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, 1L);
            break;
        case Method::PUT:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            break;
        case Method::DELETE:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case Method::FORM_DATA:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            break;
        case Method::NONE:
            break;
    }
}

void Http::setContentType(std::string contentType) {
    setHeader("Content-Type", contentType);
}

void Http::setHeader(const string &header) {
    headers = curl_slist_append(headers, header.c_str());
}

void Http::setHeader(const string &headerKey, const string &headerValue) {
    string header = headerKey + ": " + headerValue;
    headers = curl_slist_append(headers, header.c_str());
}

void Http::setBody(const string &body) {
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, body.c_str());
}


void Http::setUserAgent(const string &userAgent) {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
}

void Http::setFormData(vector<std::pair<string, string>> bodyList) {
    mime = curl_mime_init(curl);

    for (auto &item: bodyList) {
        curl_mimepart *field = curl_mime_addpart(mime);
        curl_mime_name(field, item.first.c_str());
        curl_mime_data(field, item.second.c_str(), CURL_ZERO_TERMINATED);
    }

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
}

void Http::setConnectTimeout(int timeout) {
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
}

void Http::addBearerToken(const string &token) {
    setHeader("Authorization: Bearer " + token);
}

Http::HttpResult Http::connect() {
    HttpResult result;
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, requestCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (res != CURLE_OK || http_code != 200) {
        Common::NLog("[" + url + "] Request fail with code " + to_string(http_code) + " (" +
                     curl_easy_strerror(res) + "):" + response, "Http");
    }

    result.response = response;
    result.statusCode = http_code;

    char *redirect_url = nullptr;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &redirect_url);

    result.redirect_url = redirect_url;

    release();

    return result;
}

bool Http::connectDownload(string &path, const string &message, int maxRetries) {
    if (!Storage::isSdCardAccess()) {
        Base::updateLine("No permission to access SD card");
        delay(5);
        return false;
    }

    if (path.empty()) {
        Common::NLog("Path is empty", "Ex: connectDownload");
        return false;
    }

    int retryCount = 0;
    while (retryCount < maxRetries) {
        if (Storage::existPath(path))
            Storage::removePath(path);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        FILE *file = fopen(path.c_str(), "wb");
        if (!file) {
            Common::NLog("Failed to open file: " + path, "Ex: connectDownload");
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        std::string progressMessage = message;
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progressMessage);

        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        fclose(file);

        if (res == CURLE_OK && http_code == 200) {
            release();

            if (Common::strEndsWith(path, ".jpg") ||
                Common::strEndsWith(path, ".png") ||
                Common::strEndsWith(path, ".mp4") ||
                Common::strEndsWith(path, ".mov")) {
                delay2(200);
                for(int loop = 3;loop > 0;loop--){
                    Storage::scanMedia(path);
                    delay2(300);
                }
            }
            return true;
        }

        Common::NLog("Download failed with code " + std::to_string(http_code) + " (" +
                     curl_easy_strerror(res) + "). Retrying... (" +
                     std::to_string(retryCount + 1) + "/" + std::to_string(maxRetries) + ")",
                     "Ex: connectDownload");

        Storage::removePath(path);

        delay(5,
              message + " Retry (" + to_string(retryCount + 1) + "/" + std::to_string(maxRetries) +
              ")");

        retryCount++;
    }

    Common::NLog("Failed to download file " + message + " with url [" + url + "]", "Ex: connectDownload");

    release();
    return false;
}

void Http::release() {
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (mime) {
        curl_mime_free(mime);
    }
}

size_t Http::requestCallBack(void *contents, size_t size, size_t nmemb, string *response) {
    size_t totalSize = size * nmemb;
    if (response) {
        response->append((char *) contents, totalSize);
    }
    return totalSize;
}

int Http::progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal,
                           curl_off_t ulnow) {
    std::string *message = static_cast<std::string *>(clientp);

    string lastStatus = Base::getLastStatus();
    if (dltotal > 0) {
        static int lastPercent = -1;
        int percent = static_cast<int>((dlnow * 100) / dltotal);
        if (percent != lastPercent) {
            lastPercent = percent;
            Base::updateLine(*message + " (" + to_string(percent) + "%)");
        }
    }
    return 0;
}

string Http::urlDecode(const string &input) {
    std::ostringstream decoded;
    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] == '%') {
            if (i + 2 < input.length()) {
                std::istringstream hex(input.substr(i + 1, 2));
                int value;
                hex >> std::hex >> value;
                decoded << static_cast<char>(value);
                i += 2;
            }
        } else if (input[i] == '+') {
            decoded << ' '; // '+' được giải mã thành khoảng trắng.
        } else {
            decoded << input[i];
        }
    }
    return decoded.str();
}

