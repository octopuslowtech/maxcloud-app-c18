#include <Social.h>

class GoogleExt : public Social {
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

    void ResetVariable();

    Task handleLogin();

    bool handleLogout();

    string handleScreen(string &xml);

    bool checkLiveUID(string uid = "");

    bool isUid(string id);
};