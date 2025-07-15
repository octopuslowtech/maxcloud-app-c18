//
// Created by Admin on 12/28/2024.
//

#ifndef MAXCLOUD_ADB_H
#define MAXCLOUD_ADB_H

#include <Base.h>
#include <Storage.h>

class ADB {
public:
    static bool isRoot;

    static bool isLSPosed;

    static bool isConnected;

    static void init();

    static bool isRootAccess();

    static void chmodFile(string path);

    static void copyFile(string srcPath, string destPath);

    static string runShell(string command, int timeoutSeconds = 10);

    static void grantPermission(string packageName, vector<string> permissions);
};


#endif //MAXCLOUD_ADB_H
