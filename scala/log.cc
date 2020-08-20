#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

LogMessage::LogMessage(const char *file, int line, int level)
    : line_(line), level_(level), file_(file) {}

void LogMessage::GenLogMessage() {
  const int time_buffer_size = 1024;
  char time_str[time_buffer_size];

  struct tm tm_now;
  time_t now = time(NULL);
  localtime_r(&now, &tm_now);

  strftime(time_str, time_buffer_size, "%Y-%m-%d %H:%M:%S", &tm_now);

  if (level_ == LOG_LEVEL_INFO) {
    fprintf(stderr, "[%s I %s:%d] %s\n", time_str, file_, line_, str().c_str());
  } else if (level_ == LOG_LEVEL_ERROR) {
    fprintf(stderr, "[%s E %s:%d] %s\n", time_str, file_, line_, str().c_str());
  } else if (level_ == LOG_LEVEL_WARN) {
    fprintf(stderr, "[%s W %s:%d] %s\n", time_str, file_, line_, str().c_str());
  } else if (level_ == LOG_LEVEL_DEBUG) {
    if (enable_debug)
      fprintf(stderr, "[%s D %s:%d] %s\n", time_str, file_, line_,
              str().c_str());
  }
}

LogMessage::~LogMessage() { GenLogMessage(); }

int LogMessage::enable_debug = 0;

void EnableDebugLevelLog(void) { LogMessage::enable_debug = 1; }

void DisableDebugLevelLog(void) { LogMessage::enable_debug = 0; }
