//
// Created by Admin on 12/28/2024.
//

#include "ADB.h"

bool ADB::isConnected = false;
bool ADB::isRoot = false;
bool ADB::isLSPosed = false;

string ADB::runShell(string command, int timeoutSeconds) {
    try {
        if (isRoot) {
            command = "su -c \"" + command + "\"";
        } else {
            if (command.rfind("adb ", 0) == 0) {
                command = command.substr(4);
            } else
                command = "shell " + command;

            command = Storage::nativePath + "/libadb.so " + command;

            setenv("HOME", Storage::filesPath.c_str(), 1);
            setenv("TMPDIR", Storage::cachePath.c_str(), 1);
        }

//        Common::NLog("Command: " + command, "ADB");


        // Tạo pipe để giao tiếp giữa tiến trình cha và tiến trình con
        int pipefd[2];
        if (pipe(pipefd) == -1)
            throw std::runtime_error("Failed to create pipe");

        // Tạo tiến trình con
        pid_t pid = fork();
        if (pid == -1) {
            // Lỗi khi fork
            close(pipefd[0]);
            close(pipefd[1]);
            throw std::runtime_error("Failed to fork process");
        } else if (pid == 0) {
            // Tiến trình con
            close(pipefd[0]); // Đóng đầu đọc

            // Thiết lập pipe để ghi output vào stdout
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                _exit(1); // Thoát với lỗi
            }
            close(pipefd[1]); // Đóng đầu ghi sau khi chuyển

            // Gọi lệnh phù hợp
            if (isRoot) {
                execl("/system/bin/sh", "sh", "-c", command.c_str(), NULL);
            } else {
                execl("/bin/sh", "sh", "-c", command.c_str(), NULL);
            }
            _exit(1); // Thoát nếu execl thất bại
        } else {
            // Tiến trình cha
            close(pipefd[1]); // Đóng đầu ghi

            char buffer[128];
            std::string result;
            ssize_t bytesRead;

            // Đọc dữ liệu từ pipe với timeout
            int timeRemaining = timeoutSeconds * 100; // Timeout tính bằng đơn vị 10ms
            while (timeRemaining > 0) {
                bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0'; // Kết thúc chuỗi
                    result += buffer;
                } else if (bytesRead == 0) {
                    break; // Kết thúc nếu không còn dữ liệu
                } else {
                    break; // Lỗi khi đọc
                }
                usleep(10000); // Chờ 10ms
                timeRemaining--;
            }

            close(pipefd[0]); // Đóng đầu đọc

            // Chờ tiến trình con hoàn tất
            int status;
            waitpid(pid, &status, 0);

            // Kiểm tra trạng thái tiến trình
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                Common::NLog("Failed to execute command: " + command, "ADB");
                return "";
            }
            return Common::strTrim(result);
        }
    } catch (const std::exception &e) {
        Common::NLog(e.what(), "Ex: executeCommand", LogLevel::ERROR);
    }

    return "";
}

bool ADB::isRootAccess() {
    isRoot = true;

    bool isRooted = !runShell("echo octopus").empty();

    isRoot = isRooted;

    return isRooted;
}

void ADB::init() {
    if (isConnected) return;
    isRoot = isConnected = isRootAccess();

    if (isConnected) return;

    runShell("adb kill-server", 10);

    runShell("adb start-server", 15);

    isConnected = !runShell("echo 'octopuslowtech'", 10).empty();
}

void ADB::chmodFile(std::string path) {
    runShell("chmod -R 777 " + path, 5);
}

void ADB::copyFile(std::string srcPath, std::string destPath) {
    runShell("cp -r " + srcPath + " " + destPath, 20);
}


void ADB::grantPermission(string packageName, vector<string> permissions) {
    if (!ADB::isConnected) return;

    for (const auto &permission: permissions) {
        runShell("pm grant " + packageName + " android.permission." + permission, 5);
    }
}
