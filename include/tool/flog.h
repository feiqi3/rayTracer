#include "../Macro.h"
#include <Windows.h>
#include <cmath>
#include <ctime>
#include <direct.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#define USE_STATIC_PATH "using_static_path"
#define STR std::to_string
#ifndef FLOG_H
#define FLOG_H
using std::string;

enum TYPE {
  OFF = 0,
  TRACE = BIT(1),
  INFO = BIT(2),
  WARN = BIT(3),
  ERRO = BIT(4)
};

class Flog {

private:
  string logName;
  string LogPath;
  bool hasRedirect;
  std::ofstream logFile;
  std::streambuf *oldbuf; // restore old streambuf

  static string static_path;
  static string static_logName;
  // static std::mutex* mtx;
  static std::shared_ptr<std::mutex> mtx;
  static TYPE GLOB_LOG_LEVEL;

  static string TimeForPath() {
    tm *time = getNow();
    string day = STR(time->tm_mday);
    string year = STR(time->tm_year + 1900);
    string mon = STR(time->tm_mon + 1);
    string hour =
        time->tm_hour < 10 ? "0" + STR(time->tm_hour) : STR(time->tm_hour);
    string min =
        time->tm_min < 10 ? "0" + STR(time->tm_min) : STR(time->tm_min);
    string sec =
        time->tm_hour < 10 ? "0" + STR(time->tm_sec) : STR(time->tm_sec);
    return year + mon + day + hour + min + sec;
  }

  static void outLogTitle(TYPE type) {
    switch (type) {
    case TRACE:
      std::cout << "[TRACE] ";
      break;
    case INFO:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              FOREGROUND_INTENSITY | FOREGROUND_GREEN);
      std::cout << "[INFO] ";
      break;
    case WARN:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                  FOREGROUND_RED);
      std::cout << "[WARN] ";
      break;
    case ERRO:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              FOREGROUND_INTENSITY | FOREGROUND_RED);
      std::cout << "[ERROR]";
      break;
    default:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                              FOREGROUND_INTENSITY);
      break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_INTENSITY);
  }

  void createFold(const string &pathName) {
    string path = "";
    if (_access(pathName.c_str(), 0) == 0) {
      flog(INFO, ("Path: " + pathName + " existed!").c_str());
      return;
    }
    for (int i = 0; i < pathName.length(); i++) {
      if (pathName[i] != '/') {
        path += pathName[i];
      }
      if (pathName[i] == '/') {
        if (pathName != "") {
          if (_access(pathName.c_str(), 0) == -1) {
            _mkdir(path.c_str());
            path += "\\";
          }
        }
      }
    }
    if (_access(pathName.c_str(), 0) == 0) {
      flog(INFO, ("Path: " + pathName + " has created!").c_str());
    }
  }

  static tm *getNow() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return ltm;
  }

public:
  static void set_glob_log_level(TYPE t);

  Flog() {
    hasRedirect = false;
    std::ios_base::sync_with_stdio(false);
    GLOB_LOG_LEVEL = INFO; // default level
  }

  // formate : "{path}/"
  Flog(string path, string _logName = "log.txt") {
    std::ios_base::sync_with_stdio(false);
    GLOB_LOG_LEVEL = INFO; // default level
    logName = _logName;
    LogPath = path;
    string realLogName;
    realLogName = TimeForPath() + logName;
    if (LogPath == "" || LogPath == "/") {
      logFile = std::ofstream(realLogName);
      return;
    }
    createFold(LogPath);
    logFile = std::ofstream(LogPath + "/" + realLogName);
    std::cout.rdbuf(logFile.rdbuf());
    hasRedirect = true;
  }

  static void flog(TYPE type, string s) {
    if (type < GLOB_LOG_LEVEL)
      return;
    string time = getTime();
    Flog::mtx->lock();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_INTENSITY | FOREGROUND_GREEN |
                                FOREGROUND_RED | FOREGROUND_BLUE);
    std::cout << time;
    std::cout << " ";
    outLogTitle(type);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_GREEN | FOREGROUND_RED |
                                FOREGROUND_BLUE);
    std::cout << s << "\n";
    Flog::mtx->unlock();
  }

  // make log print in console
  void reDirectLog() {
    if (hasRedirect) {
      std::cout.rdbuf(oldbuf);
      hasRedirect = false;
      flog(INFO, "Now is console out!");
    } else {
      flog(ERRO, "Redirect false!");
    }
  }

  static long getTick() {
    clock_t tick;
    tick = clock();
    return tick;
  }

  static float deltaTick(double start, double end, int c = 10000) {
    if (c > 10 && c <= 10000)
      c = log10(c);
    else
      c = 1;
    c = c * pow(10, c);
    float delTick = (end - start) / CLOCKS_PER_SEC;
    delTick = (delTick * c);
    delTick = (int)(delTick);
    delTick /= c;
    return delTick;
  }

  static string getTime() {
    auto time = getNow();
    string day = STR(time->tm_mday);
    string year = STR(time->tm_year + 1900);
    string mon = STR(time->tm_mon + 1);
    string hour =
        time->tm_hour < 10 ? "0" + STR(time->tm_hour) : STR(time->tm_hour);
    string min =
        time->tm_min < 10 ? "0" + STR(time->tm_min) : STR(time->tm_min);
    string sec =
        time->tm_sec < 10 ? "0" + STR(time->tm_sec) : STR(time->tm_sec);
    return year + "-" + mon + "-" + day + " " + hour + ":" + min + ":" + sec;
  }

  static void logOut(string s) { flog(INFO, s); }

  static void logWarn(string s) { flog(WARN, s); }

  static void logError(string s) { flog(ERRO, s); }
};

inline std::shared_ptr<std::mutex> Flog::mtx = std::make_shared<std::mutex>();

class FTick : public Flog {
private:
  float StartTick;
  float EndTick;
  float DeltaTick;

public:
  FTick() {
    StartTick = -1;
    EndTick = -1;
  }

  void tickStart() { StartTick = getTick(); }

  void tickEnd() { EndTick = getTick(); }

  float tickDelta() {
    DeltaTick = deltaTick(StartTick, EndTick);
    return DeltaTick;
  }

  void dltTmLog(string logString) {
    char *str = new char;
    sprintf(str, "%g", DeltaTick);
    string dltS(str);
    logOut(logString + dltS + "s");
    delete str;
  }
};
inline TYPE Flog::GLOB_LOG_LEVEL = INFO;
inline void Flog::set_glob_log_level(TYPE t) { Flog::GLOB_LOG_LEVEL = t; }
#endif
