#include <Social.h>

class TiktokExt : public Social {
public:

    Task handleLogin();

    bool handleLogout();

    string handleScreen(string &xml);

    ResultScreen gotoHome(string &xml);

    ResultScreen gotoProfile(string link = "", string message = "");

    ResultScreen searchTiktok(const string &keyword, string typeSearch = "Videos");

    ResultScreen gotoLive(string link = "", string message = "", int delayTime = 3);

    void gotoLink(const string &uri, const string &message = "", int delayTime = 3);

    bool checkEmailGenerator(string email);

    std::pair<bool, string> buyGmail(const string& user,const string& pass, const string& productId = "9", const int& typeMail = 0);

    string getOtp1secmail(const string &email, int timeout = 60, const string &oldOtp = "");

    string getOtpGenerator(const string &email, int timeout = 60, const string &oldOtp = "");

    string getOtpFromEmail(const string& email, int timeout = 60, const string &oldOtp = "");

    string omoCaptcha(const string &type, const string &base64);

    static std::tuple<bool, JSON> setupTDTT();

    void ResetVariable();

};