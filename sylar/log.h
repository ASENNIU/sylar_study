//
// Created by 13345 on 2023/9/7.
//

#ifndef SYLAR_STUDY_LOG_H
#define SYLAR_STUDY_LOG_H

#include <string>
#include <sdtint.h>
#include <memory>
#include <list>
#include <stringstream>
#include <fstream>

namespace sylar {

    /***
     * @brief 日志级别
     */
    class LogLevel {
    public:
        enum Level {
            DEFAULT = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };

    /***
     * @brief 日志事件
     */
    class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent();
    private:
        const char* m_file = nullptr;         // 文件名
        int32_t m_line = 0;                   // 行号
        uint32_t m_elapse = 0;                // 程序启动到现在的耗时
        uint32_t m_threadId = 0;              // 线程号
        uint32_t m_fiberId = 0;               // 协程号
        uint64_t m_time;                      // 时间戳
        std::string m_content;                // 消息体
    };

    /***
     * @brief 日志格式化器
     */
    class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        std::string format(LogEvent::ptr event);
    };

    /***
     * @brief 日志输出地
     */
    class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender() {}
        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
        LogFormatter::ptr getFormatter() const { return m_formatter; }
    protected:
        // 不同输出地的级别
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

    /***
     * @brief 日志器
     */
    class Logger {
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string& name = "root");
        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level level) { m_level = level; }
    private:
        std::string m_name;
        // 日志器的级别（满足级别才可以输出）
        LogLevel::Level m_level;
        std::list<LogAppender::ptr> m_appenders;
    };

    /***
     * @brief 输出到控制台的Appender
     */
    class StdoutLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
    };

    /***
     * @brief 输出到文件的Appender
     */
    class FileLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string& filename);
        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
}

#endif //SYLAR_STUDY_LOG_H
