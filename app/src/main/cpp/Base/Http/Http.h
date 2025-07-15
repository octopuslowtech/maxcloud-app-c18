//
// Created by Admin on 12/22/2024.
//

#ifndef MAXCLOUD_HTTP_H
#define MAXCLOUD_HTTP_H

#include <curl/curl.h>

#include <Base.h>
#include <Storage.h>


class Http : public Base{
public:
    enum class Method {
        GET,
        POST,
        PUT,
        DELETE,
        FORM_DATA,
        NONE,
    };

    struct HttpResult {
        string response;
        int statusCode;
        string redirect_url;
        bool isSucceeded() const {
            return !response.empty(); // && statusCode == 200
        }
    };


    Http(string url, Method method = Method::GET);

    void setContentType(string contentType);

    void setHeader(const string &header);

    void setHeader(const string &headerKey, const string &headerValue);

    void setUserAgent(const string &userAgent);

    void setFormData(vector<std::pair<string, string>> bodyList);

    void setBody(const string &body);

    void setConnectTimeout(int timeout);

    void addBearerToken(const string &token);

    HttpResult connect();

    bool connectDownload(string &path,const string& message = "Download Process", int maxRetries = 10);

    static string urlDecode(const string &input);
private:
    CURL *curl;
    struct curl_slist *headers;
    curl_mime *mime;

    string url;

    void release();

    static size_t requestCallBack(void *contents, size_t size, size_t nmemb, string *response);

    static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal,
                         curl_off_t ulnow);

};


#endif //MAXCLOUD_HTTP_H
