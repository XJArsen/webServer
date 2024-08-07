// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: XJArsen
// Date: 2024-01-25

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdarg>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "Blockqueue.h"
#include "Buffer.h"
class Log {
  public:
    void init(int _level, const char* _path = "../log", const char* _suffix = ".log",
              int maxQueueCapacity = 1024);
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
    static Log* Instance();
    static void FlushLogThread();
    void write(int _level, const char* _format, ...);
    void flush();

    int GetLevel();
    void SetLevel(int);
    bool IsOpen();

  private:
    Log();
    virtual ~Log();
    void AppendLogLevelTitle(int);
    void AsyncWrite();

    static const int LOG_PATH_LEN = 256;  // 日志文件最长文件名
    static const int LOG_NAME_LEN = 256;  // 日志最长名字
    static const int MAX_LINES = 50000;   // 日志文件内的最长日志条数

    const char* path_;    //路径名
    const char* suffix_;  //后缀名

    int lineCount_;  //日志行数记录
    int toDay_;      //按当天日期区分文件

    bool isOpen_;

    Buffer buf_;    // 输出的内容，缓冲区
    int level_;     // 日志等级
    bool isAsync_;  // 是否开启异步日志

    FILE* fp_;                                      //打开log的文件指针
    std::unique_ptr<BlockQueue<std::string>> deq_;  //阻塞队列
    std::unique_ptr<std::thread> writeThread_;      //写线程的指针
    std::mutex mtx_;                                //同步日志必需的互斥量
};
#define LOG_BASE(level, format, ...)                     \
    do {                                                 \
        Log* log = Log::Instance();                      \
        if (log->IsOpen() && log->GetLevel() <= level) { \
            log->write(level, format, ##__VA_ARGS__);    \
            log->flush();                                \
        }                                                \
    } while (0);

#define LOG_DEBUG(format, ...)             \
    do {                                   \
        LOG_BASE(0, format, ##__VA_ARGS__) \
    } while (0);

#define LOG_INFO(format, ...)              \
    do {                                   \
        LOG_BASE(1, format, ##__VA_ARGS__) \
    } while (0);

#define LOG_WARN(format, ...)              \
    do {                                   \
        LOG_BASE(2, format, ##__VA_ARGS__) \
    } while (0);

#define LOG_ERROR(format, ...)             \
    do {                                   \
        LOG_BASE(3, format, ##__VA_ARGS__) \
    } while (0);

#endif
