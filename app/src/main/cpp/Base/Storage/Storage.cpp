//
//
// Created by Admin on 12/22/2024.

#include "Storage.h"

string Storage::filesPath = "";
string Storage::cachePath = "";
string Storage::nativePath = "";


jobject Storage::newPref(jobject jcontext, const string &name) {
    JNIEnv *env = JExt::getEnv();

    jstring jName = JExt::stringToJstring(name);
    jint mode = 0x0000;

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID getSharedPreferencesMethod = JExt::findMethod(contextClass,
                                                            "getSharedPreferences",
                                                            "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");

    jobject appPref = env->CallObjectMethod(jcontext, getSharedPreferencesMethod, jName, mode);

    env->DeleteLocalRef(jName);
    env->DeleteLocalRef(contextClass);

    return appPref;
}

bool Storage::setPrefValue(jobject appPref, const string &key, const string &value) {
    JNIEnv *env = JExt::getEnv();

    jclass sharedPrefClass = JExt::findClass("android/content/SharedPreferences");

    jclass editorClass = JExt::findClass("android/content/SharedPreferences$Editor");

    try {
        if (appPref == nullptr) return false;

        jmethodID editMethod = JExt::findMethod(sharedPrefClass, "edit",
                                                "()Landroid/content/SharedPreferences$Editor;");

        jobject sharedPrefEditor = env->CallObjectMethod(appPref, editMethod);

        string encrypted_key = cryptor::encrypt(key);
        string encrypted_value = cryptor::encrypt(value);

        jstring jKey = JExt::stringToJstring(encrypted_key);
        jstring jValue = JExt::stringToJstring(encrypted_value);

        jmethodID putStringMethod = JExt::findMethod(editorClass, "putString",
                                                     "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");

        jmethodID applyMethod = JExt::findMethod(editorClass, "apply", "()V");
        env->CallObjectMethod(sharedPrefEditor, putStringMethod, jKey, jValue);

        env->CallVoidMethod(sharedPrefEditor, applyMethod);

        env->DeleteLocalRef(sharedPrefEditor);
        env->DeleteLocalRef(jKey);
        env->DeleteLocalRef(jValue);
        return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: setPrefValue", LogLevel::ERROR);
        return false;
    }
}


void Storage::clearPref(jobject appPref) {
    JNIEnv *env = JExt::getEnv();

    jclass sharedPrefClass = JExt::findClass("android/content/SharedPreferences");

    jclass editorClass = JExt::findClass("android/content/SharedPreferences$Editor");

    jmethodID editMethod = JExt::findMethod(sharedPrefClass, "edit",
                                            "()Landroid/content/SharedPreferences$Editor;");
    jmethodID clearMethod = JExt::findMethod(editorClass, "clear",
                                             "()Landroid/content/SharedPreferences$Editor;");
    jmethodID applyMethod = JExt::findMethod(editorClass, "apply", "()V");

    jobject sharedPrefEditor = env->CallObjectMethod(appPref, editMethod);

    jobject clearedEditor = env->CallObjectMethod(sharedPrefEditor, clearMethod);

    env->CallVoidMethod(clearedEditor, applyMethod);

    env->DeleteLocalRef(sharedPrefEditor);
    env->DeleteLocalRef(clearedEditor);
}

string Storage::getPrefValue(jobject appPref, const string &key) {
    JNIEnv *env = JExt::getEnv();

    jclass sharedPrefClass = JExt::findClass("android/content/SharedPreferences");

    try {
        if (appPref == nullptr) return "";
        string encrypted_key = cryptor::encrypt(key);
        jstring jKey = JExt::stringToJstring(encrypted_key);
        jmethodID getStringMethod = JExt::findMethod(sharedPrefClass, "getString",
                                                     "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        jstring value = static_cast<jstring>(env->CallObjectMethod(appPref, getStringMethod,
                                                                   jKey, JExt::stringToJstring("")));
        string decrypt_value = cryptor::decrypt(JExt::jstringToString(value));

        env->DeleteLocalRef(jKey);
        env->DeleteLocalRef(value);
        return decrypt_value;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getValuePref", LogLevel::ERROR);
        return "";
    }
}

bool Storage::isSdCard;

bool Storage::isSdCardAccess() {
    try {
        struct stat info;

        if (stat("/sdcard", &info) == 0 && (info.st_mode & S_IFDIR) &&
            access("/sdcard", W_OK | R_OK) == 0)
            return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: isSdCardAccess", LogLevel::ERROR);
    }
    return false;
}

void Storage::scanMedia(std::string path) {
    if (path.empty() || !existPath(path)) {
        Common::NLog("Path not found : " + path, "scanMedia");
        return;
    }

    if(ADB::isConnected) {
        ADB::runShell("am broadcast -a android.intent.action.MEDIA_SCANNER_SCAN_FILE -d file:///mnt" + path);
        return;
    }

    // !isSdCard

    JNIEnv *env = JExt::getEnv();
    jobject jContext = getContext();
    jclass contextClass = JExt::findClass("android/content/Context");

    // Gọi MediaScannerConnection.scanFile
    jclass mediaScannerClass = JExt::findClass("android/media/MediaScannerConnection");
    jmethodID scanFileMethod = JExt::findMethod(
            mediaScannerClass,
            "scanFile",
            "(Landroid/content/Context;[Ljava/lang/String;[Ljava/lang/String;Landroid/media/MediaScannerConnection$OnScanCompletedListener;)V",
            true);

    jstring jFilePath = JExt::stringToJstring(path);

    jobjectArray filePathsArray = env->NewObjectArray(1, JExt::findClass("java/lang/String"),
                                                      nullptr);
    env->SetObjectArrayElement(filePathsArray, 0, jFilePath);

    env->CallStaticVoidMethod(mediaScannerClass, scanFileMethod, jContext, filePathsArray, nullptr,
                              nullptr);

    jclass intentClass = JExt::findClass("android/content/Intent");
    jmethodID intentConstructor = JExt::findMethod(intentClass, "<init>", "(Ljava/lang/String;)V");

    jstring actionMediaScanner = env->NewStringUTF("android.intent.action.MEDIA_SCANNER_SCAN_FILE");
    jobject scanIntent = env->NewObject(intentClass, intentConstructor, actionMediaScanner);

    jclass uriClass = JExt::findClass("android/net/Uri");
    jmethodID fromFileMethod = JExt::findMethod(uriClass, "fromFile",
                                                "(Ljava/io/File;)Landroid/net/Uri;", true);

    jclass fileClass = JExt::findClass("java/io/File");
    jmethodID fileConstructor = JExt::findMethod(fileClass, "<init>", "(Ljava/lang/String;)V");
    jobject fileObject = env->NewObject(fileClass, fileConstructor, jFilePath);

    jobject contentUri = env->CallStaticObjectMethod(uriClass, fromFileMethod, fileObject);

    jmethodID setDataMethod = JExt::findMethod(intentClass, "setData",
                                               "(Landroid/net/Uri;)Landroid/content/Intent;");
    env->CallObjectMethod(scanIntent, setDataMethod, contentUri);

    jmethodID sendBroadcastMethod = JExt::findMethod(contextClass, "sendBroadcast",
                                                     "(Landroid/content/Intent;)V");

    env->CallVoidMethod(jContext, sendBroadcastMethod, scanIntent);

    env->DeleteLocalRef(jFilePath);
    env->DeleteLocalRef(filePathsArray);
    env->DeleteLocalRef(mediaScannerClass);
    env->DeleteLocalRef(actionMediaScanner);
    env->DeleteLocalRef(scanIntent);
    env->DeleteLocalRef(uriClass);
    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(fileObject);
    env->DeleteLocalRef(contentUri);
    env->DeleteLocalRef(intentClass);
    env->DeleteLocalRef(contextClass);
}

bool Storage::copyAssetsToFiles(const std::string &input, jobject jContext) {
    if (input.empty()) {
        Common::NLog("Input is empty", "copyAssetsToFiles");
        return false;
    }

    if (jContext == nullptr)
        jContext = getContext();

    JNIEnv *env = JExt::getEnv();

    jclass contextClass = nullptr;
    jobject assetManagerObj = nullptr;
    AAsset *asset = nullptr;

    try {
        contextClass = env->GetObjectClass(jContext);
        if (!contextClass) {
            throw std::runtime_error("Failed to get Context class");
        }

        jmethodID getAssetsMethod = JExt::findMethod(contextClass, "getAssets",
                                                     "()Landroid/content/res/AssetManager;");
        if (!getAssetsMethod) {
            throw std::runtime_error("Failed to get getAssets method");
        }
        assetManagerObj = env->CallObjectMethod(jContext, getAssetsMethod);
        if (!assetManagerObj) {
            throw std::runtime_error("Failed to retrieve AssetManager");
        }

        std::string output = getFilesPath(input, jContext);

        AAssetManager *assetManager = AAssetManager_fromJava(env, assetManagerObj);
        if (!assetManager) {
            throw std::runtime_error("Failed to initialize AAssetManager");
        }

        asset = AAssetManager_open(assetManager, input.c_str(), AASSET_MODE_STREAMING);
        if (!asset) {
            throw std::runtime_error("Failed to open asset: " + input);
        }

        size_t assetSize = AAsset_getLength(asset);
        std::vector<char> buffer(assetSize);

        if (AAsset_read(asset, buffer.data(), assetSize) != assetSize) {
            throw std::runtime_error("Failed to read asset: " + input);
        }

        std::ofstream outFile(output, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Failed to open output file: " + output);
        }
        outFile.write(buffer.data(), assetSize);
        outFile.close();

        return true;
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: copyAssetsToFiles", LogLevel::ERROR);
    }
    if (asset) {
        AAsset_close(asset);
        asset = nullptr;
    }
    if (contextClass) {
        env->DeleteLocalRef(contextClass);
    }
    if (assetManagerObj) {
        env->DeleteLocalRef(assetManagerObj);
    }
    return false;
}

bool Storage::createFolder(const string &path) {
    if (!isSdCard) return false;

    if (existPath(path)) return true;

    int result = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return (result == 0);
}

vector<std::string> Storage::getFiles(string &folderPath) {
    if (!isSdCard) return {};
    std::vector<std::string> files;
    try {
        folderPath = Common::strTrim(folderPath, "/");
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(folderPath.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG) { // Kiểm tra xem nó là một tệp thực sự
                    files.push_back(folderPath + "/" + ent->d_name);
                }
            }
            closedir(dir);
        }
    }
    catch (const std::exception &e) {
    }
    return files;
}

bool Storage::existPath(const string &path) {
    JNIEnv *env = JExt::getEnv();
    jclass fileClass = env->FindClass("java/io/File");
    jmethodID constructor = env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
    jmethodID existsMethod = env->GetMethodID(fileClass, "exists", "()Z");

    jstring jPath = env->NewStringUTF(path.c_str());
    jobject fileObject = env->NewObject(fileClass, constructor, jPath);

    jboolean exists = env->CallBooleanMethod(fileObject, existsMethod);

    env->DeleteLocalRef(jPath);
    env->DeleteLocalRef(fileObject);
    return exists == JNI_TRUE;
}

bool Storage::writeFile(const string &filePath, const string &content, bool append) {
    try {
        std::ofstream outFile(filePath, append ? std::ios::app : std::ios::out);
        if (outFile.is_open()) {
            outFile << content;
            outFile.close();
            return true;
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: writeFile", LogLevel::ERROR);
    }
    return false;
}

string Storage::readFile(const string &filePath) {
    if (!isSdCard) return "";
    string content;
    try {
        std::ifstream file(filePath);
        if (file) {
            string line = "";
            while (std::getline(file, line)) {
                content += line;
            }
            file.close();
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: readFile", LogLevel::ERROR);
    }
    return content;
}

void Storage::copyFile(const string &srcPath, const string &destPath) {
    if (!isSdCard) return;
    try {
        std::ifstream srcFile(srcPath, std::ios::binary);
        if (!srcFile) return;

        std::ofstream destFile(destPath, std::ios::binary);
        if (!destFile) return;
        destFile << srcFile.rdbuf();

        srcFile.close();
        destFile.close();
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: copyFile", LogLevel::ERROR);
    }
}

std::string Storage::getFolderPath(const std::string &path) {
    try {
        size_t pos = path.find_last_of('/');
        if (pos != std::string::npos) {
            return path.substr(0, pos);
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getFolderPath", LogLevel::ERROR);
    }
    return "";
}

bool Storage::removePath(string path) {
    if(ADB::isConnected)
        return ADB::runShell("rm -rf " + path).find("No such file or directory") == std::string::npos;

    if (!isSdCard) return false;

    try {
        std::filesystem::path fsPath(path);
        if (std::filesystem::is_regular_file(fsPath)) {
            std::remove(path.c_str());
        } else if (std::filesystem::is_directory(fsPath)) {
            std::filesystem::remove_all(path.c_str());
        } else {
            if (Common::strEndsWith(path, "*")) {
                path = getFolderPath(path);
                for (const auto &entry: std::filesystem::directory_iterator(path)) {
                    if (entry.is_regular_file())
                        std::remove(entry.path().string().c_str());
                    else if (entry.is_directory())
                        std::filesystem::remove_all(entry.path());
                }
            } else {
                std::remove(path.c_str());
            }
        }
        return !existPath(path);
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: removePath", LogLevel::ERROR);
    }
    return false;
}

void Storage::init(jobject jcontext) {
    isSdCard = isSdCardAccess();

    if(isSdCard){
        createFolder(APP_FOLDER);
        createFolder(MEDIA_FOLDER);
        createFolder(APK_FOLDER);
        createFolder(TEMP_FOLDER);
        createFolder(DEBUG_FOLDER);

        createFolder(APP_FOLDER "native");
        createFolder(APP_FOLDER "java");
    }

    if(jcontext == nullptr)
        jcontext = getContext();

    if(filesPath.empty()) {
        Storage::copyAssetsToFiles("cacert.pem", jcontext);

        filesPath = getFilesPath("", jcontext);
        cachePath = getCachePath(jcontext);
        nativePath = getNativePath(jcontext);
    }
}


string Storage::getFilesPath(const string &name, jobject jContext) {
    if (jContext == nullptr)
        jContext = getContext();

    JNIEnv *env = JExt::getEnv();

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID getFilesDirMethod = JExt::findMethod(contextClass, "getFilesDir", "()Ljava/io/File;");
    jobject filesDirObj = env->CallObjectMethod(jContext, getFilesDirMethod);

    jclass fileClass = env->GetObjectClass(filesDirObj);
    jmethodID getAbsolutePathMethod = JExt::findMethod(fileClass, "getAbsolutePath",
                                                       "()Ljava/lang/String;");
    jstring filesDirPath = (jstring) env->CallObjectMethod(filesDirObj, getAbsolutePathMethod);

    string filesDir = JExt::jstringToString(filesDirPath);

    env->DeleteLocalRef(filesDirPath);
    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(filesDirObj);
    env->DeleteLocalRef(contextClass);

    if (name.empty())
        return filesDir;

    return filesDir + "/" + name;
}

string Storage::getCachePath(jobject jContext) {
    if(jContext == nullptr)
        jContext = getContext();

    JNIEnv *env = JExt::getEnv();

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID getCacheDirMethod = JExt::findMethod(contextClass, "getCacheDir", "()Ljava/io/File;");

    jobject filesDirObj = env->CallObjectMethod(jContext, getCacheDirMethod);

    jclass fileClass = env->GetObjectClass(filesDirObj);
    jmethodID getAbsolutePathMethod = JExt::findMethod(fileClass, "getAbsolutePath",
                                                       "()Ljava/lang/String;");

    jstring filesDirPath = (jstring) env->CallObjectMethod(filesDirObj, getAbsolutePathMethod);

    string cacheDir = JExt::jstringToString(filesDirPath);

    env->DeleteLocalRef(filesDirPath);
    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(filesDirObj);
    env->DeleteLocalRef(contextClass);

    return cacheDir;
}


string Storage::getNativePath(jobject jContext) {
    JNIEnv *env = JExt::getEnv();

    jclass contextClass = JExt::findClass("android/content/Context");

    jmethodID getApplicationInfoMethod = JExt::findMethod(contextClass, "getApplicationInfo",
                                                          "()Landroid/content/pm/ApplicationInfo;");
    jobject applicationInfoObj = env->CallObjectMethod(jContext, getApplicationInfoMethod);

    jclass applicationInfoClass = env->GetObjectClass(applicationInfoObj);

    jfieldID nativeLibraryDirField = env->GetFieldID(applicationInfoClass, "nativeLibraryDir",
                                                     "Ljava/lang/String;");
    jstring nativeLibraryDirPath = (jstring) env->GetObjectField(applicationInfoObj,
                                                                 nativeLibraryDirField);

    string nativeLibraryPath = JExt::jstringToString(nativeLibraryDirPath);

    env->DeleteLocalRef(nativeLibraryDirPath);
    env->DeleteLocalRef(applicationInfoClass);
    env->DeleteLocalRef(applicationInfoObj);
    env->DeleteLocalRef(contextClass);

    return nativeLibraryPath;
}

string Storage::getFileName(const std::string &path) {
    try {
        size_t pos = path.find_last_of('/'); // Tìm vị trí ký tự '/' cuối cùng
        if (pos != std::string::npos) {
            return path.substr(pos + 1); // Lấy phần sau ký tự '/'
        }
    }
    catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: getFileName", LogLevel::ERROR);
    }
    return ""; // Trả về chuỗi rỗng nếu thất bại
}

double Storage::getFileSize(const string filePath) {
    if(!existPath(filePath)) return -1.0;
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == 0) {
        return static_cast<double>(fileStat.st_size) / (1024.0 * 1024.0);
    } else {
        perror("stat");
        return -1.0;
    }
}

vector<string> Storage::getListFiles(const string &folderPath) {
    std::vector<std::string> files;

    DIR *dir = opendir(folderPath.c_str());
    if (!dir) {
        return files;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name = entry->d_name;

        if (name == "." || name == "..") {
            continue;
        }

        files.emplace_back(folderPath + "/" + name);
    }

    closedir(dir);
    return files;
}

bool Storage::zipFolder(const std::string &folderPath, const std::string &zipFilePath) {
    zip_t *zip = zip_open(zipFilePath.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

    if (!zip) {
        Common::NLog("Failed to create zip file: " + zipFilePath, "zipFolder");
        return false;
    }

    try {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string relativePath = entry.path().string().substr(folderPath.size());
                if (relativePath[0] == '/' || relativePath[0] == '\\') {
                    relativePath.erase(0, 1); // Xóa ký tự '/' hoặc '\\' đầu tiên
                }

                zip_entry_open(zip, relativePath.c_str());
                std::ifstream file(entry.path(), std::ios::binary | std::ios::ate);
                if (file.is_open()) {
                    auto fileSize = file.tellg();
                    file.seekg(0, std::ios::beg);

                    std::vector<char> buffer(fileSize);
                    file.read(buffer.data(), fileSize);
                    zip_entry_write(zip, buffer.data(), fileSize);
                }
                zip_entry_close(zip);
            }
        }
    } catch (const std::exception &e) {
        zip_close(zip);
        return false;
    }

    zip_close(zip);
    return true;
}
bool Storage::unzipFile(const std::string &zipFilePath, const std::string &outputFolderPath) {
    zip_t *zip = zip_open(zipFilePath.c_str(), 0, 'r');
    if (!zip) {
        Common::NLog("Failed to open zip file: " + zipFilePath, "zipFolder");
        return false;
    }

    try {
        for (int i = 0; i < zip_entries_total(zip); i++) {
            zip_entry_openbyindex(zip, i);

            const char *name = zip_entry_name(zip);
            std::string outputPath = outputFolderPath + "/" + name;

            if (zip_entry_isdir(zip)) {
                std::filesystem::create_directories(outputPath);
            } else {
                std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());
                std::ofstream file(outputPath, std::ios::binary);
                if (file.is_open()) {
                    size_t fileSize = zip_entry_size(zip);
                    std::vector<char> buffer(fileSize);
                    zip_entry_noallocread(zip, buffer.data(), fileSize);
                    file.write(buffer.data(), fileSize);
                    file.close();
                }
            }

            zip_entry_close(zip);
        }
    } catch (const std::exception &e) {
        zip_close(zip);
        return false;
    }

    zip_close(zip);
    return true;
}


void Storage::clearMCPTemp(){
    removePath(TEMP_FOLDER "*");
}

