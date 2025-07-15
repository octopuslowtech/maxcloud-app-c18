//
// Created by Admin on 12/22/2024.
//

#ifndef MAXCLOUD_API_H
#define MAXCLOUD_API_H

#include <Base.h>
#include <Storage.h>
#include <JSON.h>
#include <Http.h>


using Method = Http::Method;

class API : public Base {
public:
    static string get2FA(const string &twoFa);

    static string getIP();

    class Mail {
    public:
        static string buyMail1989(string user, string pass, string productId);

        static JSON buyUnlimitMail(string apiKey, string productId);

        static string getOtpHotmail(const string &email, const std::string &refresh_token,const std::string &client_id);
    };

    class TempMail {
    public:
        static string getOtpGenerator(const string &cookie);

        static void deleleMailGenerator(const string &cookie, const string &body);

        static JSON getListOtp1secmail(const string &mail, const string &domain);

        static JSON getOtp1secmail(const string &mail, const string &domain);

    };

    class LLM {
    public:
        static string askGemini(const string &apiKey, const string &model, string &question);

        static string askOpenAI(const string &apiKey, const string &model, string &question);

        static string improvePrompt(string &question);
    };


    class OMOCaptcha {
    public:
        static JSON createJob(const string &type, const string &base64);

        static JSON getJobResult(const string &jobId);
    };


    class TDTT {
    public:
        string endPoint;

        static JSON getTraodoiTT(const string &cookie);


    };

    class Facebook {
    public:
        static string getDisplayName(const string &uid);

        static string getLinkFromUID(const string &uid);

        static string findIdFacebook(const string &uid);
    };


    struct IResult {
        string response = "";
        JSON json;
        string data = "";
        vector<string> messages;
        string message;
        bool succeeded = false;

        IResult(const bool &succeeded = false,
                string message = "") :
                succeeded(succeeded),
                message(message) {}
    };

};


#endif //MAXCLOUD_API_H
