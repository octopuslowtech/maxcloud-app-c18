#include "JSON.h"


JSON::JSON(std::string jsonString) {
    try {
        json = nlohmann::json::parse(jsonString);
    }
    catch (const std::exception &e) {
    }
}


JSON::JSON(nlohmann::json _json) {
    try {
        json = _json;
    }
    catch (const std::exception &e) {
    }
}

JSON JSON::set(std::string name, const nlohmann::json &value) {
    json[name] = value;
    return *this;
}

JSON JSON::set(std::string name, const std::vector<std::string> &value) {
    json[name] = nlohmann::json(value); // Chuyển vector<string> thành JSON array
    return *this;
}


nlohmann::json JSON::getInstance(std::string name) {
    if(name.empty()) return json;
    nlohmann::json current = json;
    std::vector<std::string> arrPath = strSplit(name, ".");
    for (const auto &key: arrPath) {
        if (strIsNumber(key))
            current = current[strToInt(key)];
        else if (current.contains(key))
            current = current[key];
        else
            return nlohmann::json();
    }
    return current;
}


bool JSON::isContains(std::string name) {
    try {
        return json.contains(name);
    } catch (const std::exception &e) {
    }
    return false;
}


std::string JSON::getString(std::string name) {
    try {
        nlohmann::json result = getInstance(name);
        if (result.is_number())
            return to_string(result.get<int>());
        else if (result.is_boolean())
            return to_string(result.get<bool>());
        return result.get<string>();
    } catch (const std::exception &e) {
    }
    return "";
}

vector<string> JSON::getList(std::string name) {
    try {
        nlohmann::json result = getInstance(name);
        return result.get<std::vector<std::string >>();
    } catch (const std::exception &e) {
    }
    return {};
}

vector<string> JSON::getListSplit(std::string name, int typeSplitString) {
    vector<string> lst;
    try {
        string value = getString(name);
        string delimiter = typeSplitString == 0 ? "\n" : "\n|\n";
        lst = strSplit(value, delimiter, true);
    } catch (const json::parse_error &) {
    }
    return lst;
}

bool JSON::isExist(string name){
    try {
       return json.contains(name);
    } catch (const json::parse_error &) {
    }
    return false;
}


bool JSON::getBool(std::string name) {
    try {
        nlohmann::json result = getInstance(name);
        if (result.is_string())
            return strToBool(result.get<string>());
        return result.get<bool>();
    } catch (const std::exception &e) {
    }
    return false;
}

int JSON::getInt(std::string name) {
    try {
        nlohmann::json result = getInstance(name);
        if (result.is_string())
            return strToInt(result.get<string>());
        return result.get<int>();
    } catch (const std::exception &e) {
    }
    return 0;
}

std::vector<JSON> JSON::getArray(std::string name) {
    std::vector<JSON> result;
    try {
        if (name.empty()) {
            for (const auto &item: json)
                result.emplace_back(item.dump());
        } else {
            for (const auto &item: getInstance(name))
                result.emplace_back(item.dump());
        }
    } catch (const std::exception &e) {
    }
    return result;
}

std::string JSON::toString() {
    return json.dump(5);
}

vector<string>
JSON::strSplit(const string &s, const string &delimiter, bool isRemoveEmptyItem) {
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

int JSON::strToInt(const string &str) {
    if (str.empty())
        return -1;
    try {
        return stoi(str);
    } catch (const std::exception &e) {
    }
    return -1;
}

bool JSON::strToBool(string input) {
    return input.find("true") != string::npos;
}

bool JSON::strIsNumber(const string &str) {
    for (char c: str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool JSON::isEmpty() {
    return json.empty() || json.dump() == "{}";
}


string JSON::jsonToString(string key, nlohmann::json jsonData) {
    try {
        return jsonData[key];
    }
    catch (const std::exception &e) {
        return "";
    }
}

bool JSON::isJSON(const std::string &jsonString) {
    if(jsonString.empty()) return false;
    try {
        (void) nlohmann::json::parse(jsonString);
        return true; // Nếu parse thành công, chuỗi là JSON hợp lệ
    } catch (const json::parse_error &) {
        return false; // Nếu có lỗi parse, chuỗi không phải là JSON hợp lệ
    }
}
