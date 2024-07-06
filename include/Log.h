
#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>

// spdlog added like 6 seconds to compilation on my 14900k, this takes like 10ms
class Log final
{
public:

    enum class Level : int
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };

    static Log& the()
    {
        if (!m_the)
        {
            m_the = new Log;
        }

        return *m_the;
    }

    //TODO: format output with colors, add timestamp, custom log format

    template<typename T, typename... Args>
    void debug(T first, Args... args)
    {
        if(m_log_level > Level::DEBUG) { return; }
        std::cout << "[debug] ";
        log_inner(first, args...);
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    void info(T first, Args... args)
    {
        if(m_log_level > Level::INFO) { return; }
        std::cout << "[info] ";
        log_inner(first, args...);
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    void warn(T first, Args... args)
    {
        if(m_log_level > Level::WARN) { return; }
        std::cout << "[warn] ";
        log_inner(first, args...);
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    void error(T first, Args... args)
    {
        if(m_log_level > Level::ERROR) { return; }
        std::cout << "[error] ";
        log_inner(first, args...);
        std::cout << std::endl;
    }

    template<typename T, typename... Args>
    void critical(T first, Args... args)
    {
        if(m_log_level > Level::CRITICAL) { return; }
        std::cout << "[critical] ";
        log_inner(first, args...);
        std::cout << std::endl;
    }

    void set_level(const Level level)
    {
        m_log_level = level;
    }

private:

    static void log_inner() {}

    template<typename T, typename... Args>
    void log_inner(T first, Args... args)
    {
        std::cout << first;
        log_inner(args...);
    }

    // TODO: add log sinks: stdout, file, etc
    Log() {}
    Log(Log&&) = delete;
    Log(Log&) = delete;

    Level m_log_level {Level::INFO};

    static Log* m_the;
};


#endif //LOGGER_H
