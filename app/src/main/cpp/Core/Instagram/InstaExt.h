//
// Created by Admin on 12/14/2024.
//

#ifndef MAXCLOUD_INSTAEXT_H
#define MAXCLOUD_INSTAEXT_H


#include <Social.h>

class InstaExt : public Social {
public:
    Task handleLogin();

    bool handleLogout();

    string handleScreen(string &xml);

    //void gotoLink(const string &uri, const string &message = "", int delayTime = 3);

    void gotoLink(const string &uri);

    bool gotoProfile(string user = "");

    void gotoHashtag(string hashtag);

    bool gotoHome();

    void gotoInbox();

    bool gotoCreateStory();

    void gotoSettings();

    void gotoPost(string link);

    bool gotoReel();

    bool gotoCreateReel();

    bool gotoCreatePostImg();

    string getIdPostFromLink(string link);

    bool IsBlock();

    bool IsStop();

    bool IsLogined();

    bool IsLogout();

    bool IsRelogined();

    bool IsSomethingWentWrong();

    bool IsCheckpoint();

    void ResetVariable();

    void hideKeyboard();

    vector<Job> getJobs();

    void gotoSearch();

private:
    bool isBlock = false;
    bool isLogined = false;
    bool isLogout = false;
    bool isRelogined = false;
    bool isSomethingWentWrong = false;
    string typeCheckpoint = "";


};


#endif //MAXCLOUD_INSTAEXT_H
