//
// Created by HieuThuHai on 8/13/2024.
//

#include <openssl/aes.h>
#include <openssl/err.h>
#include "Common.h"

void Common::NLog(string content, const string &tag, LogLevel logLevel) {
    int maxLogSize = 1000;
    content = "[" + tag + "] : " + content;
    int length = content.length();
    for (int i = 0; i < length; i += maxLogSize) {
        int chunkSize = std::min(maxLogSize, length - i);
        if (logLevel == LogLevel::ERROR || logLevel == LogLevel::WARNING)
            __android_log_print(ANDROID_LOG_ERROR, "XNative", "%.*s", chunkSize,
                                content.c_str() + i);
        else
            __android_log_print(ANDROID_LOG_DEBUG, "XNative", "%.*s", chunkSize,
                                content.c_str() + i);
    }

    if (logLevel == LogLevel::ERROR){
        string path = "/sdcard/com.maxcloud.app/native/" + Common::getDateNow() + ".txt";

        string contentOut = "["  + getTimeNow() + "] " + content + "\n";

        std::ofstream outFile(path, std::ios::out | std::ios::app);
        if (outFile.is_open()) {
            outFile << contentOut;
            outFile.close();
        }
    }
}

std::string Common::imageToBase64Str(const std::string &imagePath) {
    try {
        std::ifstream inputFile(imagePath, std::ios::binary);
        if (!inputFile) {
            std::cerr << "Error: Unable to open file: " << imagePath << std::endl;
            return "";
        }

        std::vector<char> imageBytes((std::istreambuf_iterator<char>(inputFile)),
                                     std::istreambuf_iterator<char>());

        std::string imageString(imageBytes.begin(), imageBytes.end());

        return base64_encode(imageString);
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return "";
    }
}

string Common::base64_encode(const string &input) {
    BIO *bio, *b64;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input.c_str(), input.length());
    BIO_flush(bio);

    char *data;
    long length = BIO_get_mem_data(bio, &data);

    string encoded(data, length);
    BIO_free_all(bio);
    return encoded;
}

string Common::base64_encode(const unsigned char *data, size_t length) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    size_t i;
    unsigned char arr3[3];
    unsigned char arr4[4];

    for (i = 0; i < length; i++) {
        arr3[i % 3] = data[i];

        if ((i + 1) % 3 == 0) {
            arr4[0] = (arr3[0] & 0xfc) >> 2;
            arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
            arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
            arr4[3] = arr3[2] & 0x3f;

            for (int j = 0; j < 4; j++) {
                encoded += table[arr4[j]];
            }
        }
    }

    if (i % 3 != 0) {
        for (int j = i % 3; j < 3; j++) {
            arr3[j] = '\0';
        }

        arr4[0] = (arr3[0] & 0xfc) >> 2;
        arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
        arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
        arr4[3] = arr3[2] & 0x3f;

        for (int j = 0; j < (i % 3) + 1; j++) {
            encoded += table[arr4[j]];
        }

        while ((i++ % 3) != 0) {
            encoded += '=';
        }
    }

    return encoded;
}

//string Common::base64_decode(const std::string &encoded) {
//    BIO *bio, *b64;
//    char *decodedData = new char[encoded.length()];
//    memset(decodedData, 0, encoded.length());
//
//    b64 = BIO_new(BIO_f_base64());
//    bio = BIO_new_mem_buf(encoded.c_str(), -1);
//    bio = BIO_push(b64, bio);
//
//    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
//    int decodedLength = BIO_read(bio, decodedData, encoded.length());
//    BIO_free_all(bio);
//
//    std::string decoded(decodedData, decodedLength);
//    delete[] decodedData;
//
//    return decoded;
//}


std::string Common::base64_decode(const std::string& encoded) {
    BIO *bio, *b64;
    std::vector<char> decodedData(encoded.length()); // Sử dụng vector để tự động quản lý bộ nhớ

    b64 = BIO_new(BIO_f_base64());
    if (!b64) {
        throw std::runtime_error("Failed to create base64 BIO");
    }

    bio = BIO_new_mem_buf(encoded.c_str(), encoded.length());
    if (!bio) {
        BIO_free(b64);
        throw std::runtime_error("Failed to create memory BIO");
    }

    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    int decodedLength = BIO_read(bio, decodedData.data(), encoded.length());
    BIO_free_all(bio);

    if (decodedLength < 0) {
        throw std::runtime_error("Failed to decode base64 string");
    }

    return std::string(decodedData.data(), decodedLength);
}


//std::string base64_chars =
//        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//        "abcdefghijklmnopqrstuvwxyz"
//        "0123456789+/";

//bool CommonExt::is_base64(unsigned char c) {
//    return (isalnum(c) || (c == '+') || (c == '/'));
//}

//std::string Common::base64_encode(const std::string &input) {
//    std::string encoded;
//    int i = 0;
//    int j = 0;
//    unsigned char char_array_3[3];
//    unsigned char char_array_4[4];
//
//    for (const auto &c: input) {
//        char_array_3[i++] = c;
//        if (i == 3) {
//            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
//            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//            char_array_4[3] = char_array_3[2] & 0x3f;
//
//            for (i = 0; (i < 4); i++)
//                encoded += base64_chars[char_array_4[i]];
//            i = 0;
//        }
//    }
//
//    if (i) {
//        for (j = i; j < 3; j++)
//            char_array_3[j] = '\0';
//
//        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
//        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//
//        for (j = 0; (j < i + 1); j++)
//            encoded += base64_chars[char_array_4[j]];
//
//        while ((i++ < 3))
//            encoded += '=';
//    }
//
//    return encoded;
//}
//
//std::string Common::base64_decode(const std::string &encoded) {
//    int in_len = encoded.size();
//    int i = 0;
//    int j = 0;
//    int in_ = 0;
//    unsigned char char_array_4[4], char_array_3[3];
//    std::string decoded;
//
//    while (in_len-- && (encoded[in_] != '=') &&
//           (isalnum(encoded[in_]) || (encoded[in_] == '+') || (encoded[in_] == '/'))) {
//        char_array_4[i++] = encoded[in_];
//        in_++;
//        if (i == 4) {
//            for (i = 0; i < 4; i++)
//                char_array_4[i] = base64_chars.find(char_array_4[i]);
//
//            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
//            char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//            char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];
//
//            for (i = 0; (i < 3); i++)
//                decoded += char_array_3[i];
//            i = 0;
//        }
//    }
//
//    if (i) {
//        for (j = i; j < 4; j++)
//            char_array_4[j] = 0;
//
//        for (j = 0; j < 4; j++)
//            char_array_4[j] = base64_chars.find(char_array_4[j]);
//
//        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
//        char_array_3[1] = ((char_array_4[1] & 0x0f) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//
//        for (j = 0; (j < i - 1); j++)
//            decoded += char_array_3[j];
//    }
//
//    return decoded;
//}

bool Common::strToBool(string input) {
    return input.find("true") != string::npos;
}

string Common::strReplace(const string &input, const string &search, const string &replace) {
    string result = input;
    size_t pos = 0;

    while ((pos = result.find(search, pos)) != string::npos) {
        result.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return result;
}

string Common::strTrim(const std::string &str, const std::string &chars) {
    size_t start = str.find_first_not_of(chars);

    size_t end = str.find_last_not_of(chars);

    if (start == std::string::npos || end == std::string::npos)
        return std::string();

    return str.substr(start, end - start + 1);
}

string Common::strToLower(const string &str) {
    string result = str;
    for (char &c: result)
        c = std::tolower(static_cast<unsigned char>(c));

    return result;
}

std::string Common::strToUpper(const std::string &input) {
    std::string result = input;
    for (char &c: result)
        c = std::toupper(static_cast<unsigned char>(c));

    return result;
}

int Common::findMin(const vector<int> &lst) {
    if (lst.empty())
        return -1;

    auto value = std::min_element(lst.begin(), lst.end());
    return *value;
}

int Common::findMax(const vector<int> &lst) {
    if (lst.empty())
        return -1;

    auto value = std::max_element(lst.begin(), lst.end());
    return *value;
}

string Common::ranString(int length, const string &typeRandom) {
    std::string result = "";

    std::vector<std::string> arr;
    size_t start = 0, end;
    while ((end = typeRandom.find("_", start)) != std::string::npos) {
        arr.push_back(typeRandom.substr(start, end - start));
        start = end + 1;
    }
    arr.push_back(typeRandom.substr(start));

    for (int i = 0; i < length; i++) {
        std::string typeChar = arr[ranNumber(0, arr.size() - 1)];

        if (typeChar == "a") {
            result += randomLowercase(1);
        } else if (typeChar == "A") {
            result += randomUppercase(1);
        } else if (typeChar == "0") {
            result += randomNumber(1);
        } else if (typeChar == "$") {
            result += randomSpecialChar(1);
        }
    }

    return result;
}

string Common::randomChar(string input, const string &typeRandom, const char &c) {
    string output = "";

    std::vector<std::string> arr;
    size_t start = 0, end;
    while ((end = typeRandom.find("_", start)) != std::string::npos) {
        arr.push_back(typeRandom.substr(start, end - start));
        start = end + 1;
    }
    arr.push_back(typeRandom.substr(start));

    for (int i = 0; i < input.size(); i++) {
        if (input[i] != c) {
            output += input[i];
            continue;
        }

        std::string typeChar = arr[ranNumber(0, arr.size() - 1)];

        if (typeChar == "a") {
            output += randomLowercase(1);
        } else if (typeChar == "A") {
            output += randomUppercase(1);
        } else if (typeChar == "0") {
            output += randomNumber(1);
        } else if (typeChar == "$") {
            output += randomSpecialChar(1);
        }
    }

    return output;
}

string Common::ranNumberStr(int length) {
    std::string characters = "0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);
    std::string randomString = "";
    for (int i = 0; i < length; ++i) {
        int randomIndex = dis(gen);
        randomString += characters[randomIndex];
    }
    return randomString;
}

template<typename T>
bool Common::containsItem(const std::vector<T> &lst, const T &item) {
    for (const T &element: lst) {
        if (element == item)
            return true;
    }
    return false;
}

bool Common::containsItem(const std::vector<std::string> &lst, const std::string &item) {
    return std::find(lst.begin(), lst.end(), item) != lst.end();
}


bool Common::containsItemLower(const std::vector<std::string> &lst, const std::string &item) {
    if (lst.empty() || item.empty())
        return false;
    for (const std::string &element: lst) {
        if (strContainsLower(item, element))
            return true;
    }
    return false;
}

std::vector<string>
Common::exceptList(const std::vector<string> &lst1, const std::vector<string> &lst2) {
    std::vector<string> result;
    for (string item: lst1) {
        if (!containsItem(lst2, item)) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<string>
Common::intersectList(const std::vector<string> &lst1, const std::vector<string> &lst2) {
    std::vector<string> result;
    for (string item: lst1) {
        if (containsItem(lst2, item)) {
            result.push_back(item);
        }
    }
    return result;
}

void Common::removeIndex(std::vector<std::string> &lst, int index) {
    if (index >= 0 && index < lst.size())
        lst.erase(lst.begin() + index);
}

void Common::removeItem(vector<std::string> &lst, const string &item) {
    auto it = std::find(lst.begin(), lst.end(), item);
    if (it != lst.end())
        lst.erase(it);
}

void Common::removeDuplicateItem(vector<std::string> &lst) {
    std::sort(lst.begin(), lst.end());
    lst.erase(std::unique(lst.begin(), lst.end()), lst.end());
}

bool Common::strContains(const std::string &input1, const std::string &input2) {
    return input1.find(input2) != std::string::npos;
}

bool Common::strContainsLower(const std::string &input1, const std::string &input2) {
    return strToLower(input1).find(strToLower(input2)) != std::string::npos;
}


bool Common::strStartsWith(const string &mainString, const string &prefix) {
    if (mainString.length() < prefix.length())
        return false;
    return mainString.substr(0, prefix.length()) == prefix;
}

bool Common::strEndsWith(const string &mainString, const string &postfix) {
    if (mainString.length() < postfix.length())
        return false;
    return mainString.compare(mainString.length() - postfix.length(), postfix.length(), postfix) ==
           0;
}

string Common::replaceSpecialCharXML(string input) {
    if (strContainsLower(input, "\n"))
        input = strReplace(input, "\n", "\\n");
    if (strContainsLower(input, "\""))
        input = strReplace(input, "\"", "'");
    if (strContainsLower(input, "&"))
        input = strReplace(input, "&", "&amp;");
    if (strContainsLower(input, "<"))
        input = strReplace(input, "<", "&lt;");
    if (strContainsLower(input, ">"))
        input = strReplace(input, ">", "&gt;");
    return input;
}

bool Common::checkEmail1secmail(string email) {
    string domain = strSplit(email, "@")[1];
    return containsItem({
                                "1secmail.com",
                                "1secmail.org",
                                "1secmail.net",
                                "icznn.com",
                                "ezztt.com",
                                "vjuum.com",
                                "laafd.com",
                                "txcct.com"
                                "dpptd.com"
                                "rteet.com"
                        }, domain);
}

bool
Common::hasExceededTimeout(std::chrono::high_resolution_clock::time_point timeStart,
                           int timeout) {
    return Common::getElapsedTime(timeStart) >= timeout;
}

int
Common::getRemainingTime(std::chrono::high_resolution_clock::time_point timeStart,
                         int timeout) {
    return timeout - getElapsedTime(timeStart);
}

int Common::getElapsedTime(std::chrono::high_resolution_clock::time_point timeStart) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - timeStart);
    return static_cast<int>(duration.count());
}

int Common::getElapsedTimeMillis(std::chrono::high_resolution_clock::time_point timeStart) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = currentTime - timeStart;
    return static_cast<int>(duration.count());
}

string Common::currentDate(string format) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);

    return buf;
}

string Common::currentDatetime(string format) {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);

    return buf;
}


bool Common::isEmail(const std::string &email) {
    // Biểu thức chính quy để kiểm tra định dạng email
    std::regex email_regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    // Sử dụng hàm std::regex_match để kiểm tra
    return std::regex_match(email, email_regex);
}

string Common::ranSplitArray(std::string input, string key) {
    vector<string> lstSplit = strSplit(input, key);
    if (lstSplit.empty()) return "";

    int randomIndex = ranArrayNumber(lstSplit.size());
    return lstSplit[randomIndex];
}


bool Common::jboolean2bool(jboolean input) {
    return input == JNI_TRUE;
}

int Common::ranArrayNumber(int maxValue) {
    if (maxValue <= 1) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, maxValue - 1);
    return dis(gen);
}

bool Common::strIsNumber(const string &str) {
    if (str.empty())
        return false;
    for (char c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

uint16_t Common::stringToUint16(const std::string& str) {
    try {
        unsigned long value = std::stoul(str);

        // Kiểm tra nếu giá trị nằm ngoài phạm vi uint16_t
        if (value > std::numeric_limits<uint16_t>::max()) {
            throw std::out_of_range("Value out of range for uint16_t");
        }

        return static_cast<uint16_t>(value); // Ép kiểu về uint16_t
    } catch (const std::exception& e) {
        Common::NLog("Error: " + std::string(e.what()), "stringToUint16", LogLevel::ERROR);
        return 0; // Trả về giá trị mặc định
    }
}


int Common::ranIndex(const std::vector<std::string> &lst) {
    if (lst.empty())
        return -1;
    return ranNumber(0, lst.size() - 1);
}

string Common::ranItem(const std::vector<std::string> &lst) {
    if (lst.empty())
        return "";
    int index = ranIndex(lst);
    return lst[index];
}

//template <typename T>
//int Common::ranIndex(const std::vector<T> &lst) {
//    if (lst.empty())
//        return -1;
//
//    // Seed the random number generator
//    std::srand(std::time(0));
//
//    return std::rand() % lst.size();
//}
//
//template <typename T>
//T Common::ranItem(const std::vector<T> &lst) {
//    if (lst.empty())
//        return T(); // Return default-constructed value for empty vector
//
//    int index = ranIndex(lst);
//    return lst[index];
//}

string Common::ranItemSplit(const string &s, const string &delimiter) {
    return ranItem(strSplit(s, delimiter));
}

vector<string>
Common::strSplit(const string &s, const string &delimiter, bool isRemoveEmptyItem) {
    if (s.empty()) return vector<string>();
    vector<string> tokens;
    string inputCopy = s;

    size_t pos = 0;
    string token;

    while ((pos = inputCopy.find(delimiter)) != string::npos) {
        token = inputCopy.substr(0, pos);

        if (!isRemoveEmptyItem || !token.empty())
            tokens.push_back(token);

        inputCopy.erase(0, pos + delimiter.length());
    }

    if (!isRemoveEmptyItem || !inputCopy.empty()) {
        tokens.push_back(inputCopy);
    }

    return tokens;
}

std::string Common::strJoin(const std::vector<std::string> &vec, const std::string &delimiter) {
    std::string result;

    // Lặp qua từng phần tử của vector
    for (size_t i = 0; i < vec.size(); ++i) {
        // Nối phần tử vào chuỗi kết quả
        result += vec[i];

        // Nếu không phải phần tử cuối cùng, thêm ngăn cách vào
        if (i < vec.size() - 1) {
            result += delimiter;
        }
    }

    return result;
}

int Common::ranNumber(int minValue, int maxValue) {
    if (minValue == maxValue)
        return minValue;
    if (minValue > maxValue) {
        int temp = minValue;
        minValue = maxValue;
        maxValue = temp;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minValue, maxValue);
    return dis(gen);
}

bool Common::ranBool(int percent) {
    if (percent <= 0)
        return false;

    if (percent >= 100)
        return true;

    return ranNumber(1, 99) < percent;
}

string Common::ranKiTuDacBiet(int length) {
    std::string characters = "&@?$#";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, characters.size() - 1);
    std::string randomString = "";
    for (int i = 0; i < length; ++i) {
        int randomIndex = dis(gen);
        randomString += characters[randomIndex];
    }
    return randomString;
}

string Common::ranGuid() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    uint64_t part1 = dis(gen);
    uint64_t part2 = dis(gen);

    std::stringstream ss;
    ss << std::hex << std::setfill('0')
       << std::setw(8) << (part1 >> 32) << "-"
       << std::setw(4) << ((part1 >> 16) & 0xFFFF) << "-"
       << std::setw(4) << (part1 & 0xFFFF) << "-"
       << std::setw(4) << (part2 >> 48) << "-"
       << std::setw(12) << (part2 & 0xFFFFFFFFFFFFULL);

    return ss.str();
}

string Common::ranNameViet() {
    vector<string> lstHo = {
            "Nguyễn", "Trần", "Lê", "Phạm", "Hoàng", "Phan", "Vũ", "Đặng", "Bùi", "Đỗ", "Hồ", "Ngô",
            "Dương", "Lý", "Huỳnh", "Võ"
    };
    vector<string> lstTenDem = {
            "Diệu", "Quỳnh", "Trâm", "Nguyệt", "Trân", "Quế", "Trúc", "Xuyến", "Thiên", "Ngọc",
            "Nghi", "Linh", "Thục", "Thu", "Thiên", "Hiếu", "Thái", "Dạ", "Quỳnh", "Thiên", "Ái",
            "Kim", "Vân", "Hồng", "Minh", "Diễm", "Chi", "Bạch", "Ngọc", "Mộc", "Ha", "Thương",
            "Dại", "Thu", "Uyển", "Yến", "Thục", "Hạnh", "Thanh", "Tú", "Song", "Cát", "Lâm",
            "Hương", "Dạ", "Anh", "Đoan", "Phượng", "Tịnh", "Hải", "Thiên", "Gia", "Thành", "Trung",
            "Thái", "Hải", "Thành", "Thông", "Phúc", "Tài", "Mạnh", "Chấn", "Bảo", "Khang", "Đăng",
            "Tuấn", "Thanh", "Hiền", "Thiện", "Thụ", "Minh", "Nhân", "Trọng", "Trung", "Khôi",
            "Hạo", "Phương", "Thanh", "Hữu", "Minh", "Đông", "Sơn", "Tùng", "Ái", "Thái", "Trường",
            "Thiện", "Thạch", "An", "Anh", "Thanh", "Chiến", "Toàn", "Minh", "Đình", "Kiến", "Nhân",
            "Khôi", "Quang", "Uy", "Hoài", "Huyền", "Thùy", "Trung", "Tú", "Vàng", "Hạ", "Lệ",
            "Tuyết", "Yên", "Ngọc", "Bảo", "Khải", "Sơn", "Nguyệt", "Bảo", "Ly", "Minh", "Hương",
            "Lan", "Lien", "Linh", "Mai", "Quỳnh", "Hiền", "Hạc", "Nhật", "Quỳnh", "Huyền", "Kỳ",
            "Vinh", "Thụy", "Van", "Hạnh", "Kiều", "Từ", "Thien", "Hải", "Hướng", "Thuy", "Kim",
            "Minh", "Yen", "Truc", "Hồng", "Hạ", "Hồng", "Hương", "Khánh"
    };
    vector<string> lstTen = {
            "Anh", "Cát", "Châu", "Chi", "Di", "Diệp", "Dung", "Đan", "Đoan", "Giang", "Hà", "Hạnh",
            "Hòa", "Hương", "Khanh", "Khánh", "Khuê", "Khue", "Kiều", "Lan", "Lien", "Mien", "Mi",
            "Nga", "Ngọc", "Nguyệt", "Nhã", "Oanh", "Quyên", "San", "Tâm", "Thư", "Tường", "Tuyền",
            "Thảo", "Thi", "Trang", "Vũ", "Yên", "Yến", "Ân", "Bảo", "Công", "Dũng", "Dương",
            "Đăng", "Đạt", "Điền", "Đức", "Hùng", "Hưng", "Kiện", "Khoa", "Kiệt", "Liêm", "Minh",
            "Ngôn", "Nhân", "Nhật", "Nghĩa", "Nguyên", "Nhiên", "Phi", "Phong", "Phước", "Quân",
            "Quốc", "Sơn", "Tùng", "Thái", "Thế", "Thắng", "Triết", "Trung", "Văn", "Vĩ", "Vinh",
            "An", "Băng", "Bằng", "Bích", "Bình", "Ca", "Chau", "Chung", "Cuc", "Dạ", "Dao", "Diệu",
            "Du"
    };

    return ranItem(lstHo) + " " + ranItem(lstTenDem) + " " + ranItem(lstTen);
}

string Common::ranNameNgoai() {
    vector<string> lstHo = {
            "Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore",
            "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson",
            "Garcia", "Martinez", "Robinson", "Clark", "Rodriguez", "Lewis", "Lee", "Walker",
            "Hall", "Allen", "Young", "Hernandez", "King", "Wright", "Lopez", "Hill", "Scott",
            "Green", "Adams", "Baker", "Gonzalez", "Nelson", "Carter", "Mitchell", "Perez",
            "Roberts", "Turner", "Phillips", "Campbell", "Parker", "Evans", "Edwards", "Collins",
            "Stewart", "Sanchez", "Morris", "Rogers", "Reed", "Cook", "Morgan", "Bell", "Murphy",
            "Bailey", "Rivera", "Cooper", "Richardson", "Cox", "Howard", "Ward", "Torres",
            "Peterson", "Gray", "Ramirez", "James", "Watson", "Brooks", "Kelly", "Sanders", "Price",
            "Bennett", "Wood", "Barnes", "Ross", "Henderson", "Coleman", "Jenkins", "Perry",
            "Powell", "Long", "Patterson", "Hughes", "Flores", "Washington", "Butler", "Simmons",
            "Foster", "Gonzales", "Bryant", "Alexander"
    };
    vector<string> lstTen = {
            "James", "John", "Robert", "Michael", "William", "David", "Richard", "Joseph", "Thomas",
            "Charles", "Christopher", "Daniel", "Matthew", "Anthony", "Donald", "Mark", "Paul",
            "Steven", "Andrew", "Kenneth", "Joshua", "George", "Kevin", "Brian", "Edward", "Ronald",
            "Timothy", "Jason", "Jeffrey", "Ryan", "Jacob", "Gary", "Nicholas", "Eric", "Stephen",
            "Jonathan", "Larry", "Justin", "Scott", "Brandon", "Frank", "Benjamin", "Gregory",
            "Samuel", "Raymond", "Patrick", "Alexander", "Jack", "Dennis", "Jerry", "Tyler",
            "Aaron", "Jose", "Henry", "Douglas", "Adam", "Peter", "Nathan", "Zachary", "Walter",
            "Kyle", "Harold", "Carl", "Jeremy", "Keith", "Roger", "Gerald", "Ethan", "Arthur",
            "Terry", "Christian", "Sean", "Lawrence", "Austin", "Joe", "Noah", "Jesse", "Albert",
            "Bryan", "Billy", "Bruce", "Willie", "Jordan", "Dylan", "Alan", "Ralph", "Gabriel",
            "Roy", "Juan", "Wayne", "Eugene", "Logan", "Randy", "Louis", "Russell", "Vincent",
            "Philip", "Bobby", "Johnny", "Bradley"
    };

    return ranItem(lstHo) + " " + ranItem(lstTen);
}

string Common::boolToString(bool value) {
    return value ? "true" : "false";
}

int Common::strToInt(const string &str) {
    if (str.empty())
        return -1;
    try {
        return stoi(str);
    } catch (const std::exception &e) {
    }
    return -1;
}

string Common::spinText(const string &text) {
    if (text.empty())
        return text;

    std::string pattern = "\\{[^{}]*\\}";
    std::regex re(pattern);

    std::string result = text;

    std::smatch match;
    while (std::regex_search(result, match, re)) {
        std::string seg = match.str().substr(1, match.length() - 2);
        std::vector<std::string> choices;
        size_t pos = 0;
        while ((pos = seg.find("|")) != std::string::npos) {
            choices.push_back(seg.substr(0, pos));
            seg.erase(0, pos + 1);
        }
        choices.push_back(seg);

        // Chọn một lựa chọn ngẫu nhiên
        size_t randomIndex = ranNumber(0, choices.size() - 1);
        result = match.prefix().str() + choices[randomIndex] + match.suffix().str();
    }

    result = spinIcon(result);

    return result;
}

string Common::getDateNow() {
    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);

    // Định dạng ngày tháng năm
    std::tm *tm_info = std::localtime(&now);
    char dateBuffer[11]; // dd/MM/yyyy
    std::strftime(dateBuffer, sizeof(dateBuffer), "%d_%m_%Y", tm_info);
    return string(dateBuffer);
}

string Common::getTimeNow() {
    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);

    // Định dạng ngày tháng năm
    std::tm *tm_info = std::localtime(&now);
    char timeBuffer[9]; // HH:mm:ss
    std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", tm_info);
    return string(timeBuffer);
}

string Common::randomChar(int length, const string &allChars) {
    std::string result = "";

    int allCharsLength = allChars.length();

    for (int i = 0; i < length; i++) {
        result += allChars[ranNumber(0, allCharsLength - 1)];
    }

    return result;
}

string Common::randomLowercase(int length) {
    return randomChar(length, "abcdefghijklmnopqrstuvxyz");
}

string Common::randomUppercase(int length) {
    return randomChar(length, "ABCDEFGHIJKLMNOPQRSTUVXYZ");
}

string Common::randomNumber(int length) {
    return randomChar(length, "0123456789");
}

string Common::randomSpecialChar(int length) {
    return randomChar(length, "~!@#$%^&*()_+`-={}|[]\\:\";'<>?,./");
}

string Common::getIcon(const string &type) {
    // Các biến chứa các loại icon
    string icon1 = "🙂|😀|😄|😆|😅|😂|🤣|😊|😌|😉|😏|😍|😘|😗|😙|😚|🤗|😳|🙃|😇|😈|😛|😝|😜|😋|🤤|🤓|😎|🤑|😒|🙁|☹️|😞|😔|😖|😓|😢|😢|😭|😟|😣|😩|😫|😕|🤔|🙄|😤|😠|😡|😶|🤐|😐|😑|😯|😲|😧|😨|😰|😱|😪|😴|😬|🤥|🤧|🤒|😷|🤕|😵|🤢|🤠|🤡|👿|👹|👺|👻|💀|👽|👾|🤖|💩|🎃";
    string icon2 = "♥️|❤️|💛|💚|💙|💜|🖤|💖|💝|💔|❣️|💕|💞|💓|💗|💘|💟|💌|💋|👄|💄|💍|📿|🎁|👙|👗|👚|👕|👘|🎽|👘|👖|👠|👡|👢|👟|👞|👒|🎩|🎓|👑|⛑️|👓|🕶️|🌂|👛|👝|👜|💼|🎒|🛍️|🛒|🎭|🎦|🎨|🤹|🎊|🎉|🎈|🎧|🎷|🎺|🎸|🎻|🥁|🎹|🎤|🎵|🎶|🎼|⚽|🏀|🏈|⚾|🏐|🏉|🎱|🎾|🏸|🏓|🏏|🏑|🏒|🥅|⛸️|🎿|🥊|🥋|⛳|🎳|🏹|🎣|🎯|🚵|🎖️|🏅|🥇|🥈|🥉|🏆";
    string icon3 = "🍏|🍎|🍐|🍊|🍋|🍌|🍉|🍇|🍓|🍈|🥝|🥑|🍍|🍒|🍑|🍆|🥒|🥕|🌶|🌽|🍅|🥔|🍠|🌰|🥜|🍯|🥐|🍞|🥖|🧀|🥚|🍳|🥓|🍤|🍗|🍖|🍕|🌭|🍔|🍟|🥙|🌮|🌯|🥗|🥘|🍝|🍜|🍲|🍣|🍱|🍛|🍚|🍙|🍘|🍢|🍡|🍧|🍨|🍦|🥞|🍰|🎂|🍮|🍭|🍥|🍬|🍫|🍿|🍩|🍪|🍼|🥛|☕|🍵|🍶|🍺|🍻|🥂|🍷|🥃|🍸|🍹|🍾|🥄|🍴|🍽";
    string icon4 = "😺|😸|😹|😻|😼|😽|🙀|😿|😾|🐱|🐶|🐰|🐭|🐹|🦊|🐻|🐼|🐨|🐯|🦁|🐮|🐗|🐷|🐽|🐸|🐵|🙈|🙉|🙊|🦍|🐺|🐑|🐐|🐏|🐴|🦄|🦌|🦏|🦅|🐤|🐣|🐥|🐔|🐓|🦃|🐦|🦆|🦇|🦉|🕊️|🐧|🐕|🐩|🐈|🐇|🐁|🐀|🐿|🐒|🐖|🐆|🐅|🐃|🐂|🐄|🐎|🐪|🐫|🐘|🐊|🐢|🐠|🐟|🐡|🐬|🦈|🐳|🐋|🦑|🐙|🦐|🐚|🦀|🦂|🦎|🐍|🐛|🐜|🕷️|🕸️|🐞|🦋|🐝|🐌|🐲|🐉|🐾";
    string icon5 = "🌼|🌸|🌺|🏵️|🌻|🌷|🌹|🥀|💐|🌾|🎋|☘|🍀|🍃|🍂|🍁|🌱|🌿|🎍|🌵|🌴|🌳|🌳|🎄|🍄|🌎|🌍|🌏|🌜|🌛|🌕|🌖|🌗|🌘|🌑|🌒|🌓|🌔|🌚|🌝|🌙|💫|⭐|🌟|✨|⚡|🔥|💥|☄️|🌞|☀️|🌤️|⛅|🌥️|🌦️|☁️|🌧️|⛈️|🌩️|🌨️|🌈|💧|💦|☂️|☔|🌊|🌫|🌪|💨|❄|🌬|⛄|☃️";
    string icon6 = "🚗|🚕|🚙|🚌|🚎|🏎|🚓|🚑|🚒|🚐|🚚|🚛|🚜|🛴|🚲|🛵|🏍|🚘|🚖|🚍|🚔|🚨|💺|✈|🛫|🛬|🛩|🚁|🚀|🛰|🚡|🚠|🚟|🚃|🚋|🚞|🚝|🚄|🚅|🚈|🚂|🚆|🚊|🚇|🚉|🛶|⛵|🛥|🚤|🚢|⛴|🛳|⚓|🚧|⛽|🚏|🚦|🚥|🛣|🛤|🏗|🏭|🏠|🏡|🏘|🏚|🏢|🏬|🏤|🏣|🏥|🏦|🏪|🏫|🏨|🏩|🏛|🏰|🏯|🏟️|⛪|💒|🕌|🕍|🕋|⛩|🗼|🗿|🗽|🗺|🎪|🎠|🎡|🎢|⛲|⛱|🏖|🏝|🏕|⛺|🗾|⛰|🏔|🗻|🌋|🏞|🏜|🌅|🌄|🎑|🌠|🎇|🎆|🏙|🌇|🌆|🌃|🌌|🌉|🌁";
    string icon7 = "📱|📲|💻|🖥|⌨|🖨|🖱|🖲|🕹|🎮|💽|💾|💿|📀|📼|📷|📸|📹|🎥|📽|🎞|🎬|📞|☎|📟|📠|📺|📻|🎙|🎚|🎛|📡|📢|📣|🔔|💡|🕯|🔦|🔋|🔌|⌚|⏱|⏲|⏰|🕰|⌛|⏳|🔮|💎|🎲|🎰|💸|💵|💴|💶|💷|💰|💳|💲|💱|⚖|🔫|💣|🔪|🗡|⚔|🛡|🚬|⚰|⚱|🗜️|🔧|🔨|⚒|🛠|⛏|🔩|⚙|⛓|💈|🌡|💊|💉|⚗|🔬|🔭|🚿|🛁|🚽|🛎|🔑|🗝|🚪|🛋|🛏|🖼|🏺|🗑|🛢|🕳|🏮|🎏|🎎|🎐|🎫|🎟️|🎀|🎗️|📯|✉|📩|📨|📧|📦|📪|📫|📬|📭|📮|📥|📤|📜|📃|📄|📑|📊|📈|📉|🗒|📅|📆|🗓|📇|🗃|🗳|🗄|📋|📁|📂|🗂|📓|📔|📒|📕|📗|📘|📙|📚|📖|🗞|📰|📝|✏|🖊|🖍|🖌|🖋|✒|📌|📍|📎|🖇|🔖|🏷|🔗|🔍|🔎|📐|📏|✂|🔒|🔓|🔏|🔐";
    string icon8 = "🙂|😀|😄|😆|😅|😂|🤣|😊|😌|😉|😍|😘|😗|😙|😚|🤗|😳|🙃|😛|😝|😜|😋|🤤|🤓|😎";

    string icon = "";
    std::vector<std::string> lst;

    // Chọn icon dựa trên loại
    if (type == "[r1]") {
        lst = strSplit(icon1, "|");
    } else if (type == "[r2]") {
        lst = strSplit(icon2, "|");
    } else if (type == "[r3]") {
        lst = strSplit(icon3, "|");
    } else if (type == "[r4]") {
        lst = strSplit(icon4, "|");
    } else if (type == "[r5]") {
        lst = strSplit(icon5, "|");
    } else if (type == "[r6]") {
        lst = strSplit(icon6, "|");
    } else if (type == "[r7]") {
        lst = strSplit(icon7, "|");
    } else if (type == "[r8]") {
        lst = strSplit(icon8, "|");
    } else if (type == "[d]") {
        icon = getDateNow();
    } else if (type == "[t]") {
        icon = getTimeNow();
    }

    if (lst.size() > 0)
        icon = ranItem(lst);
    return icon;
}

string Common::getNumber(const string &input) {
    std::vector<std::string> numbers = strSplit("0️⃣|1️⃣|2️⃣|3️⃣|4️⃣|5️⃣|6️⃣|7️⃣|8️⃣|9️⃣", "|");

    std::string output;
    for (char character: input) {
        std::string temp(1, character);
        int num = strToInt(temp);
        if (num > -1)
            temp = numbers[num];
        output += temp;
    }
    return output;
}

string Common::getChar(const string &input) {
    std::string charOriginal = "abcdefghijklmnopqrstuvwxyz";
    std::vector<std::string> chars = strSplit(
            "🇦|🇧|🇨|🇩|🇪|🇫|🇬|🇭|🇮|🇯|🇰|🇱|🇲|🇳|🇴|🇵|🇶|🇷|🇸|🇹|🇺|🇻|🇼|🇽|🇾|🇿", "|");

    std::string output;
    for (char character: input) {
        std::string temp(1, character);
        auto it = std::find(charOriginal.begin(), charOriginal.end(), character);
        if (it != charOriginal.end()) {
            int index = std::distance(charOriginal.begin(), it);
            temp = chars[index];
        }
        output += temp + " ";
    }

    return output;
}

string Common::spinIcon(std::string input) {
    std::string output;

    // Xử lý icon
    std::vector<std::string> lstKey = {"[r1]", "[r2]", "[r3]", "[r4]", "[r5]", "[r6]", "[r7]",
                                       "[r8]", "[d]", "[t]"};
    for (const auto &typeStr: lstKey) {
        size_t pos = input.find(typeStr);
        while (pos != std::string::npos) {
            output += input.substr(0, pos) + getIcon(typeStr);
            input = input.substr(pos + typeStr.length());
            pos = input.find(typeStr);
        }
        output += input;
        input = output;
        output = "";
    }
    output += input;

    // Xử lý số
    input = output;
    output = "";
    std::regex reNumber("\\[n(\\d+)\\]");
    std::smatch matchNumber;
    while (std::regex_search(input, matchNumber, reNumber)) {
        output += matchNumber.prefix().str() + ranString(strToInt(matchNumber[1].str()), "0");
        input = matchNumber.suffix().str();
    }
    output += input;

    // Xử lý chuỗi
    input = output;
    output = "";
    std::regex reString("\\[s(\\d+)\\]");
    std::smatch matchString;
    while (std::regex_search(input, matchString, reString)) {
        output += matchString.prefix().str() + ranString(std::stoi(matchString[1].str()), "a");
        input = matchString.suffix().str();
    }
    output += input;

    // Thay dãy số
    input = output;
    output = "";
    std::regex reQ("\\[q(.*?)\\]");
    std::smatch matchQ;
    while (std::regex_search(input, matchQ, reQ)) {
        output += matchQ.prefix().str() + getNumber(matchQ[1].str()) + " ";
        input = matchQ.suffix().str();
    }
    output += input;

    // Thay chữ cái
    input = output;
    output = "";
    std::regex reC("\\[c(.*?)\\]");
    std::smatch matchC;
    while (std::regex_search(input, matchC, reC)) {
        output += matchC.prefix().str() + getChar(matchC[1].str());
        input = matchC.suffix().str();
    }
    output += input;

    return output;
}

vector<string>
Common::regexGroups(const string &input, const string &pattern, std::size_t groupIndex) {
    std::regex rPattern(pattern);
    std::smatch matches;

    std::vector<std::string> results;

    auto it = input.cbegin();
    while (std::regex_search(it, input.cend(), matches, rPattern)) {
        if (groupIndex < matches.size())
            results.push_back(matches[groupIndex].str());

        it = matches.suffix().first;
    }

    return results;
}

//index: 0:first, -1: last, other: index
string Common::regexGroups1(const string &input, const string &pattern, int index) {
    vector<string> lst = regexGroups(input, pattern, 1);
    if (lst.empty())
        return "";
    if (index == -1)
        return lst.back();
    if (lst.size() > index)
        return lst[index];
    return lst.front();
}

string Common::regexValue(const string &input, const string &pattern) {
    vector<string> lst = regexGroups(input, pattern, 0);
    if (lst.empty())
        return "";
    return lst.front();
}

bool Common::isSocialAction(const string &action) {
    static const std::vector<std::string> socialActions = {
            "tiktok", "facebook", "twitter", "instagram", "threads"
    };

    for (string item: socialActions) {
        if (strContains(action, item)) return true;
    }
    return false;
}

std::string Common::encodeUnicode(const std::string &input) {
    std::ostringstream encoded;
    for (unsigned char c: input) {
        if (c >= 0x20 && c <= 0x7E) {
            encoded << c;
        } else {
            encoded << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int) c;
        }
    }
    return encoded.str();
}


uint64_t Common::currentTimeInMicroSeconds() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return uint64_t(time.tv_sec) * 1000 * 1000 + time.tv_usec;
}

bool Common::isValidXML(const std::string& xmlString) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xmlString.c_str());

    return result;
}

string Common::parseJWTPayload(const string &jwt){
    std::vector<std::string> parts = strSplit(jwt, ".");

    std::string payload = base64_decode(parts[1]);

    return payload;
}

std::string Common::timestampToDateString(const long &timestamp, const string& format ) {
    std::time_t time = static_cast<std::time_t>(timestamp);

    std::tm *tm_info = std::localtime(&time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), format.c_str(), tm_info);

    return std::string(buffer);
}
long Common::strToLong(const string &str) {
    if (str.empty())
        return -1;
    try {
        return std::stol(str);
    } catch (const std::exception &e) {
        Common::NLog("Error: " + std::string(e.what()), "strToLong", LogLevel::ERROR);
        return -1;
    }
}

