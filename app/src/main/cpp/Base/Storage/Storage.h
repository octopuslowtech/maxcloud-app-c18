//
// Created by Admin on 12/22/2024.
//

#ifndef MAXCLOUD_STORAGE_H
#define MAXCLOUD_STORAGE_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <Base.h>
#include <ADB.h>
#include <zib/zip.h>


class Storage : public Base {
public:
    static string filesPath, cachePath, nativePath;

    static bool isSdCard;

    static void init(jobject jcontext = nullptr);

    static jobject newPref(jobject jcontext, const string &name);

    static bool setPrefValue(jobject appPref, const string &key, const string &value);

    static void clearPref(jobject appPref);

    static string getPrefValue(jobject appPref, const string &key);

    static void scanMedia(string path);

    static bool isSdCardAccess();

    static bool copyAssetsToFiles(const string &input, jobject jContext = nullptr);

    static bool createFolder(const string &path);

    static std::vector<std::string> getFiles(string &folderPath);

    static bool existPath(const string &path);

    static bool writeFile(const string &filePath, const string &content, bool append = false);

    static string readFile(const string &filePath);

    static void copyFile(const string &sourceFile, const string &destFile);

    static string getFolderPath(const std::string &path);

    static string getFileName(const std::string &path);

    static double getFileSize(const string filePath);

    static bool removePath(string path);

    static string getFilesPath(const string &name = "", jobject jContext = nullptr);

    static string getCachePath(jobject jcontext = nullptr);

    static string getNativePath(jobject jContext);

    static vector<string> getListFiles(const string &folderPath);

    static bool zipFolder(const string &folderPath, const string &zipPath);

    static bool unzipFile(const string &folderPath, const string &zipPath);

    static void clearMCPTemp();
};


#endif //MAXCLOUD_STORAGE_H
