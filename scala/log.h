#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <sstream>

const int LOG_LEVEL_INFO = 1;
const int LOG_LEVEL_ERROR = 2;
const int LOG_LEVEL_WARN = 3;
const int LOG_LEVEL_DEBUG = 4;

class LogMessage : public std::basic_ostringstream<char> {
 public:
  LogMessage(const char *file, int line, int level);
  ~LogMessage();

  static int enable_debug;

 private:
  void GenLogMessage();

  int line_;
  int level_;
  const char *file_;
};

void EnableDebugLevelLog(void);
void DisableDebugLevelLog(void);

#define LOG_INFO LogMessage(__FILE__, __LINE__, LOG_LEVEL_INFO)
#define LOG_WARN LogMessage(__FILE__, __LINE__, LOG_LEVEL_WARN)
#define LOG_ERROR LogMessage(__FILE__, __LINE__, LOG_LEVEL_ERROR)
#define LOG_DEBUG LogMessage(__FILE__, __LINE__, LOG_LEVEL_DEBUG)

#define LOG(level) LOG_##level

#endif
