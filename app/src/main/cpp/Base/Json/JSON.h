//
// Created by Admin on 08/12/2023.
//

#include <jni.h>
#include <string>
#include <json.hpp>
#pragma once


using std::string, std::vector;
using std::to_string;
using json = nlohmann::json;


class JSON {
public:
    JSON(string jsonString = "{}");

    JSON(nlohmann::json _json);

    nlohmann::json getInstance(std::string name = "");

    JSON set(std::string name, const nlohmann::json &value);

    JSON set(std::string name, const std::vector<std::string> &value);

    bool isContains(std::string name);

    std::string getString(std::string name);

    vector<string> getList(std::string name);

    vector<string> getListSplit(std::string name, int typeSplitString = 0);

    bool getBool(std::string name);

    int getInt(std::string name);

    bool isExist(std::string name);

    std::vector<JSON> getArray(std::string name="");

    std::string toString();

    nlohmann::json toNlohmannJson() const{
        return json;
    }

    bool isEmpty();

    vector<string> strSplit(const string &s, const string &delimiter="\n", bool isRemoveEmptyItem=false);

    static string jsonToString(string key, json jsonData);

    static bool isJSON(const std::string &jsonString);
private:
    nlohmann::json json = nlohmann::json::object();

    static int strToInt(const string &str);

    static bool strToBool(string input);

    static bool strIsNumber(const string &str);
};


