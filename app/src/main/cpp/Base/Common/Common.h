#ifndef MAXCLOUD_COMMON_H
#define MAXCLOUD_COMMON_H

#include <jni.h>
#include <string>
#include <android/log.h>
#include <regex>
#include <random>
#include <list>
#include <vector>
#include <sys/wait.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <fstream>
#include <thread>
#include <dirent.h>
#include <iterator>
#include <locale>
#include <codecvt>
#include <tuple>
#include <pugixml.hpp>

#include <JExt.h>
#include <cryptor.hpp>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <omp.h>


using std::string, std::vector, std::regex, std::list, std::chrono::high_resolution_clock;
using std::stoi, std::to_string;

#define NLOG(content) Common::NLog(content, __PRETTY_FUNCTION__)

class Common {
public:
    enum class LogLevel {
        DEBUG,
        ERROR,
        WARNING
    };

    static void
    NLog(string content, const string &tag = "Common", LogLevel logLevel = LogLevel::DEBUG);

    static string imageToBase64Str(const string &imagePath);

    static string base64_encode(const string &input);

    static string base64_encode(const unsigned char *data, size_t length);

    static string base64_decode(const std::string &encoded);

    static bool strToBool(string input);

    static string strReplace(const string &input, const string &search, const string &replace);

    static string strToLower(const string &str);

    static string strTrim(const std::string &str, const std::string &chars = " \t\n\r");

    static string strToUpper(const std::string &input);

    static string ranString(int length, const std::string &typeRandom = "a_A_0");

    static string randomChar(string input, const string &typeRandom = "a_A_0", const char &c = '*');

    static string ranNumberStr(int length = 1);

    template<typename T>
    static bool containsItem(const std::vector<T> &lst, const T &item);

    static bool strContains(const std::string &input1, const std::string &input2);

    static bool containsItemLower(const std::vector<std::string> &lst, const std::string &item);

    static vector<string>
    exceptList(const std::vector<std::string> &lst1, const std::vector<std::string> &ls2);

    static vector<string>
    intersectList(const std::vector<std::string> &lst1, const std::vector<std::string> &ls2);

    static bool strContainsLower(const std::string &input1, const std::string &input2);

    static bool strStartsWith(const string &mainString, const string &prefix);

    static bool strEndsWith(const string &mainString, const string &postfix);

    static string replaceSpecialCharXML(string input);

    static bool checkEmail1secmail(string email);

    static bool
    hasExceededTimeout(std::chrono::high_resolution_clock::time_point timeStart, int timeout = -1);

    static int
    getRemainingTime(std::chrono::high_resolution_clock::time_point timeStart, int timeout);

    static int getElapsedTime(std::chrono::high_resolution_clock::time_point timeStart);

    static int getElapsedTimeMillis(std::chrono::high_resolution_clock::time_point timeStart);

    static string currentDate(string format = "%Y%m%d");

    static string currentDatetime(string format = "%d/%m/%Y %H:%M:%S");

    static bool isEmail(const std::string &email);

    static string ranSplitArray(std::string input, string key = "|");

    static bool jboolean2bool(jboolean input);

    static int ranArrayNumber(int maxValue);

    static bool strIsNumber(const string &str);

    static uint16_t stringToUint16(const std::string &str);

    static vector<string>
    strSplit(const string &s, const string &delimiter = "\n", bool isRemoveEmptyItem = false);

    static string ranItemSplit(const string &s, const string &delimiter);

    static int ranIndex(const std::vector<std::string> &lst);

    static string ranItem(const std::vector<std::string> &lst);

    static void removeItem(std::vector<std::string> &lst, const std::string &item);

    static void removeDuplicateItem(std::vector<std::string> &lst);

    static void removeIndex(std::vector<std::string> &lst, int index);

    static bool containsItem(const std::vector<std::string> &lst, const std::string &item);

    static string strJoin(const std::vector<std::string> &vec, const std::string &delimiter);

    static int ranNumber(int minValue, int maxValue);

    static bool ranBool(int percent = 50);

    static string ranKiTuDacBiet(int length = 1);

    static string ranGuid();

    static string ranNameViet();

    static string ranNameNgoai();

    static int strToInt(const string &str);

    static long strToLong(const string &str);

    static string boolToString(bool value);

    static int findMin(const vector<int> &lst);

    static int findMax(const vector<int> &lst);

    static string spinText(const string &text);

    static string getDateNow();

    static string getTimeNow();

    static string regexValue(const string &input, const string &pattern);

    static string regexGroups1(const string &input, const string &pattern, int index = 0);

    static vector<string>
    regexGroups(const std::string &input, const std::string &pattern, std::size_t groupIndex = 0);

    static bool isSocialAction(const string &action);

    static uint64_t currentTimeInMicroSeconds();

    static string encodeUnicode(const string &input);

    static bool isValidXML(const std::string &xmlString);

    static string parseJWTPayload(const string &jwt);

    static std::string
    timestampToDateString(const long &timestamp, const string &format = "%a %b %d %H:%M:%S");
private:
    static string randomChar(int length, const string &allChars);

    static string randomLowercase(int length);

    static string randomUppercase(int length);

    static string randomNumber(int length);

    static string randomSpecialChar(int length);

    static string getIcon(const string &type);

    static string getNumber(const string &input);

    static string getChar(const string &input);

    static string spinIcon(std::string input);
};

#endif //MAXCLOUD_COMMON_H
