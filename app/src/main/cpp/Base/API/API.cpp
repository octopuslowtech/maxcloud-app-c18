//
// Created by Admin on 12/22/2024.
//

#include "API.h"

string API::get2FA(const std::string &twoFa) {
    try {
        Http client("https://2fa.live/tok/" + twoFa, Method::GET);
        auto result = client.connect();
        if (!result.isSucceeded()) return "";
        JSON json(result.response);
        return json.getString("token");
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: get2FA", LogLevel::ERROR);
    }
    return "";
}

string API::getIP() {
    try {
        Http client("https://ifconfig.co/ip", Method::GET);
        auto result = client.connect();
        if (!result.isSucceeded()) return "";
        return Common::strTrim(result.response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: get2FA");
    }
    return "";
}

string API::Mail::buyMail1989(std::string user, std::string pass, std::string productId) {
    try {
        string url =
                "https://shop1989nd.com/api/BResource.php?username=" + user + "&password=" + pass +
                "&id=" + productId + "&amount=1";
        Http client(url, Method::GET);

        string response = client.connect().response;

        return response;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: Mail::buyMail1989", LogLevel::ERROR);
    }
    return "";
}

JSON API::Mail::buyUnlimitMail(::string apiKey, string productId) {
    try {
        JSON body{};
        body.set("quantity", 1);
        body.set("token", apiKey);
        body.set("product_id", productId);

        Http client("https://unlimitmail.com/api/buyHotMailUd", Method::POST);

        client.setBody(body.toString());

        client.setContentType("application/json");

        string response = client.connect().response;

        return JSON(response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: Mail::buyMail1989", LogLevel::ERROR);
    }
    return JSON();
}

string API::Mail::getOtpHotmail(const std::string &email, const std::string &refresh_token,const std::string &client_id) {
    try {
        if(email.empty() || refresh_token.empty() || client_id.empty())
        {
            NLOG("input Empty");
            return "";
        }
        Http client("https://tools.dongvanfb.net/api/get_code_oauth2", Method::POST);

        JSON body;
        body.set("email", email);
        body.set("refresh_token", refresh_token);
        body.set("client_id", client_id);
        body.set("type", "tiktok");
        client.setBody(body.toString());

        auto result = client.connect();

        if (!result.isSucceeded()) return "";

        JSON json{result.response};

        return json.getString("code");
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: TempMail::getOtpHotmail", LogLevel::ERROR);
    }
    return "";
}


string API::TempMail::getOtpGenerator(const string &cookie) {
    try {
        Http client("https://generator.email/", Method::GET);
        client.setHeader("cookie", cookie);
        client.setHeader("user-agent",
                         "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");

        return client.connect().response;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: TempMail::getOtp1secmail", LogLevel::ERROR);
    }
    return "";
}

void API::TempMail::deleleMailGenerator(const std::string &cookie, const std::string &body) {
    try {
        Http client("https://generator.email/del_mail.php", Method::POST);
        client.setHeader("cookie : " + cookie);
        client.setHeader(
                "user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");

        client.setContentType("application/json");

        client.setBody(body);

        client.connect();

    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: TempMail::deleleMail1secmail", LogLevel::ERROR);
    }
}

JSON API::TempMail::getListOtp1secmail(const string &mail, const string &domain) {
    try {
        Http client("https://www.1secmail.com/api/v1/?action=getMessages&login=" + mail +
                    "&domain=" +
                    domain, Method::GET);

        auto result = client.connect();

        return JSON(result.response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: TempMail::getListOtp1secmail", LogLevel::ERROR);
    }
    return JSON();
}

JSON API::TempMail::getOtp1secmail(const string &mail, const string &domain) {
    try {
        Http client("https://www.1secmail.com/api/v1/?action=readMessage&login=" + mail +
                    "&id=" +
                    domain, Method::GET);

        auto result = client.connect();

        return JSON(result.response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: TempMail::getOtp1secmail", LogLevel::ERROR);
    }
    return JSON();
}

static string omoAPIKey = "bN7eY6uhLwfj6Vqaxnx5hi5I3iEgIMNlp2VBE4dEGrQSXe0vSjyGE38JJvvJmXh2wuCxD6gzVEiQ3flf";

JSON API::OMOCaptcha::createJob(const std::string &type, const std::string &base64) {
    try {
        Http client("https://omocaptcha.com/api/createJob", Method::POST);

        client.setContentType("application/json");

        JSON body;
        body.set("api_token", omoAPIKey);

        body.set("data", JSON()
                .set("type_job_id", type)
                .set("image_base64", base64).toNlohmannJson());

        string bodyStr = body.toString();

        client.setBody(bodyStr);

        auto result = client.connect();

        if (!result.isSucceeded()) {
            return JSON();
        }

        Common::NLog(result.response, "OMOCaptcha::createJob");


        return JSON(result.response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: OMOCaptcha::createJob");
    }
    return JSON();
}

JSON API::OMOCaptcha::getJobResult(const std::string &jobId) {
    try {
        Http client("https://omocaptcha.com/api/getJobResult", Method::POST);

        client.setContentType("application/json");

        JSON body;
        body.set("api_token", omoAPIKey);
        body.set("job_id", jobId);

        string bodyStr = body.toString();

        client.setBody(bodyStr);

        auto result = client.connect();

        if (!result.isSucceeded()) return JSON();

        Common::NLog(result.response, "OMOCaptcha::getJobResult");

        return JSON(result.response);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: OMOCaptcha::getJobResult");
    }
    return JSON();
}

string API::Facebook::getDisplayName(const std::string &uid) {
    Http client("https://www.facebook.com/api/graphql", Method::NONE);

    client.setFormData(
            {
                    {"doc_id",    "5341536295888250"},
                    {"variables", R"({"height":500,"scale":1,"userID":")" + uid +
                                  R"(","width":500})"}
            }
    );

    auto response = client.connect();
    if (!response.isSucceeded()) return "";

    JSON json(response.response);

    return json.getString("data.profile.name");
}

string API::Facebook::getLinkFromUID(const std::string &uid) {
    try {
        string url = Common::strStartsWith(uid, "https://") ? uid : "https://www.facebook.com/" +
                                                                    uid;
        Http client(url, Method::GET);

        auto response = client.connect();

        if (response.statusCode != 200) return "";

        if (Common::strStartsWith(response.redirect_url, "https://www.facebook.com/login/?next=")) {
            string link = Common::strSplit(response.redirect_url, "next=")[1];
            link = Http::urlDecode(link);
        }

        if (Common::strContains(response.redirect_url, "photo")) {
            string temp = Common::regexGroups1(response.response,
                                               "creation_time\":\\d+,\"url\":\"(.*?)\"", 0);
            if (!temp.empty()) {
                return Common::strReplace(temp, "\\/", "/");
            }
        }

        if (!Common::strStartsWith(uid, "https://") && response.redirect_url == url) {
            return "";
        }

        return response.redirect_url;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: OMOCaptcha::getJobResult");
    }
    return "";
}

string API::Facebook::findIdFacebook(const std::string &uid) {
    try {
        string url = Common::strStartsWith(uid, "https://") ? uid : "https://www.facebook.com/" +
                                                                    uid;

        std::vector<std::string> patterns = {
                R"(facebook\.com/profile.php\?id=(\d+).*?)",
                R"(facebook\.com/groups/(\d+)/\?ref=share)",
                R"(facebook\.com/groups/.*?\?multi_permalinks=(\d+).*?)",
                R"(facebook\.com/groups/.*?/posts/(\d+).*?)",
                R"(facebook\.com/.*?\?profile_id=(\d+).*?)",
                R"(facebook\.com/watch.*?\?v=(\d+).*?)"
        };

        for (const auto &pattern: patterns) {
            std::string id = Common::regexGroups1(url, pattern);
            if (!id.empty()) {
                return id;
            }
        }

        std::vector<std::string> services = {
                "https://id.traodoisub.com/",
                "https://huyblogs.com/check-tim-lay-id-facebook/",
                "https://ahachat.com/tra-cuu-so-dien-thoai",
                "https://www.phanmemninja.com/tim-uid-facebook-lay-uid-facebook-find-uid-facebook-free",
                "https://ffb.vn/get-uid",
//                "https://tools1s.com/findmyfbid/",
                "https://phanmemmkt.vn/tim-fb-uid",
                "https://fbuid.net/",
//                "https://lookup-id.com/",
//                "https://www.easyme.pro/"
        };

        for (const auto &service: services) {
            if (service == "https://id.traodoisub.com/") {
                Http client("https://id.traodoisub.com/api.php", Method::NONE);
                client.setFormData({{"link", url}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("id");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://huyblogs.com/check-tim-lay-id-facebook/") {
                string username = Common::strReplace(url, "https://www.facebook.com/", "");
                if (Common::strEndsWith(username, "/"))
                    username = username.substr(0, username.size() - 1);

                Http client("https://api.findids.net/api/get-uid-from-username", Method::NONE);
                client.setFormData({{"username", username}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("data.id");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://ahachat.com/tra-cuu-so-dien-thoai") {
                Http client("https://ahachat.com/get-uid", Method::NONE);
                client.setFormData({{"url", url}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("data");
                if (!id.empty()) return id;
                continue;
            }

            if (service ==
                "https://www.phanmemninja.com/tim-uid-facebook-lay-uid-facebook-find-uid-facebook-free") {
                Http client("https://www.phanmemninja.com/wp-content/uid.php", Method::NONE);
                client.setFormData({{"link", url}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("data");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://ffb.vn/get-uid") {
                Http client("https://ffb.vn/api/tool/get-id-fb?idfb=" + url, Method::GET);

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("id");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://tools1s.com/findmyfbid/") {
                string username = Common::strReplace(url, "https://www.facebook.com/", "");
                if (Common::strEndsWith(username, "/"))
                    username = username.substr(0, username.size() - 1);

                Http client("https://tools1s.com/findmyfbid/process.php", Method::NONE);

                client.setFormData(
                        {
                                {"CheckOneLink___", ""},
                                {"username",        username},
                                {"Check_time",      ""},
                        }
                );

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("id");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://phanmemmkt.vn/tim-fb-uid") {
                Http client("https://id.traodoisub.com/api.php", Method::NONE);
                client.setFormData({{"link", url}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("id");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://fbuid.net/") {
                Http client("https://fchat-app.salekit.com:4039/api/v1/facebook/get_uid",
                            Method::NONE);
                client.setFormData({{"link", url}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("uid");
                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://lookup-id.com/") {
                Http client("https://lookup-id.com/", Method::NONE);

                client.setFormData({{"fburl", url},
                                    {"check", "Lookup"}});

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                string id = Common::regexGroups1(response.response, "id=\"code\">(\\d+)<", 0);

                if (!id.empty()) return id;
                continue;
            }

            if (service == "https://www.easyme.pro/") {
                Http client("https://www.easyme.pro/api/getUid.php", Method::NONE);

                JSON body;
                body.set("url", url);
                client.setBody(body.toString());

                auto response = client.connect();
                if (!response.isSucceeded()) continue;

                JSON json(response.response);
                string id = json.getString("id");
                if (!id.empty()) return id;
                continue;
            }
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: OMOCaptcha::getJobResult");
    }
    return "";
}

string API::LLM::askGemini(const string &apiKey,const string &model, string &question) {
    Http client("https://generativelanguage.googleapis.com/v1beta/models/gemini-" + model +  ":generateContent?key=" + apiKey, Method::POST);

    client.setContentType("application/json");

    question = improvePrompt(question);

    client.setBody(R"({"contents":[{"parts":[{"text":")" + question + R"("}]}]})");

    auto response = client.connect();

    if (!response.isSucceeded()) return "";

    JSON json(response.response);

    return json.getString("candidates.0.content.parts.0.text");
}

string API::LLM::askOpenAI(const string &apiKey,const string &model, string &question) {
    Http client("https://api.openai.com/v1/chat/completions", Method::POST);

    client.setContentType("application/json");

    client.addBearerToken(apiKey);

    question = improvePrompt(question);

    JSON body;
    body.set("model", model);
    body.set("messages", json::array({
        {
            {"role", "user"},
            {"content", json::array({
                {
                    {"type", "text"},
                    {"text", question}
                }
            })}
        }
    }));

    client.setBody(body.toString());

    auto response = client.connect();

    if (!response.isSucceeded()) return "";

    JSON json(response.response);

    return json.getString("choices.0.message.content");
}

string API::LLM::improvePrompt(string &question) {
    question = Common::strReplace(question, " ", "+");
    question = Common::strReplace(question, "\n", "+");
    question = Common::strReplace(question, "\t", "+");
    question = Common::strReplace(question, "\r", "+");

    return std::string();
}
