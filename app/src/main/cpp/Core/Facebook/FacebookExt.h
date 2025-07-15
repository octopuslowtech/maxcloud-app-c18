#include <Social.h>

class FacebookExt : public Social {
public:
    enum LikeType {
        LIKE = 0,
        LOVE = 1,
        CARE = 2,
        HAHA = 3,
        WOW = 4,
        SAD = 5,
        ANGRY = 6
    };

    std::map<LikeType, std::string> likeTypeToString = {
            {LIKE,  "LIKE"},
            {LOVE,  "LOVE"},
            {CARE,  "CARE"},
            {HAHA,  "HAHA"},
            {WOW,   "WOW"},
            {SAD,   "SAD"},
            {ANGRY, "ANGRY"}
    };

    Task handleLogin();

    bool handleLogout();

    bool checkLiveUid(string uid = "");

    bool isUid(string id);

    bool gotoLink(const string &link);

    bool gotoHome();

    bool gotoMenu();

    bool gotoFriends();

    bool gotoFriendsList(int typeMethod = 1);

    bool gotoGroupsList();

    bool gotoGroupMember(const string &id);

    bool gotoLinkSettingsEmail();

    bool gotoSettings();

    bool gotoSettingsSecurity();

    bool gotoReel();

    bool gotoCreateReel();

    bool gotoCreateStory();

    bool gotoPage(const string& id = "");

    bool gotoProfile(const string& id = "");

    bool gotoProfileEdit();

    bool gotoGroup(const string& id = "");

    bool gotoWatch();

    bool gotoSearchWatch(string tuKhoa = "");

    bool gotoNotify();

    string gotoPost(string link);

    bool gotoLive(const string& link);

    void navigateFB(const string &link);

    string getIdPostFromLink(string link);

    static bool IsStop();

    bool IsBlock() const;

    bool IsLogined() const;

    bool IsLogout() const;

    bool IsRelogined() const;

    bool IsSomethingWentWrong() const;

    bool IsCheckpoint();

    void ResetVariable();

    void deleteDataBackupFacebook();

    void reconnectInternet();

    string handleScreen(string &xml);

    bool waitScreen(const string& screen, string xml, int timeout);

    void hideKeyboard();
private:
    bool isBlock = false;
    bool isLogined = false;
    bool isLogout = false;
    bool isRelogined = false;
    bool isSomethingWentWrong = false;
    string typeCheckpoint = "";
    string timeBlock = "";
};