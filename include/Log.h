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
    void init(int _level, const char* _path = "./log", const char* _suffix = ".log",
              int maxQueueCapacity = 1024);

    static Log* Instance();
    static void FlushLogThread();  // 异步写日志公有方法，调用私有方法asyncWrite

    void write(int, const char*, ...);  // 将输出内容按照标准格式整理
    void flush();

    int GetLevel();
    void SetLevel(int);
    bool IsOpen();

  private:
    Log();
    virtual ~Log();
    void AppendLogLevelTitle(int);
    void AsyncWrite();  // 异步写日志方法

    static const int LOG_PATH_LEN = 256;  // 日志文件最长文件名
    static const int LOG_NAME_LEN = 256;  // 日志最长名字
    static const int MAX_LINES = 50000;   // 日志文件内的最长日志条数

    const char* path;    //路径名
    const char* suffix;  //后缀名

    int MAX_LINES;  // 最大日志行数

    int lineCount;  //日志行数记录
    int toDay;      //按当天日期区分文件

    bool isOpen;

    Buffer buf;    // 输出的内容，缓冲区
    int level;     // 日志等级
    bool isAsync;  // 是否开启异步日志

    FILE* fp;                                      //打开log的文件指针
    std::unique_ptr<BlockQueue<std::string>> deq;  //阻塞队列
    std::unique_ptr<std::thread> writeThread;      //写线程的指针
    std::mutex mtx;                                //同步日志必需的互斥量
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
