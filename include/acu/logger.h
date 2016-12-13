//
// Created by dave on 07/12/16.
//

#ifndef ACU_FW_LOGGER_H
#define ACU_FW_LOGGER_H

#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>

namespace acu {

    class Logger {
    public:
        // The Logger's name
        std::string name;

        // Available log levels
        // NOTE: The Enum-Values seem to be exposed at class level (Logger::DEBUG, etc.).
        enum Level { DEBUG, INFO, WARNING, ERROR, CRITICAL };

        // TODO handle filter levels
        // TODO add variable output (file-output, cerr, ...)

        /// Returns the corresponding Logger.
        /// \param name     The name of the logger
        ///                 (most times the class it is used in).
        /// \return         The requested Logger-Instance.
        static Logger* GetLogger(const std::string name) {
            if (Logger::loggers.count(name) == 0) {
                // This inserts the Logger in-place and allows us to
                // not provide a default constructor
                Logger::loggers.emplace(std::make_pair(name, new Logger(name)));
            }
            // Similarly `at` does not construct the element if it is missing
            // (it throws instead) and as such works without providing a default constructor
            return Logger::loggers.at(name);
        }

        /// Logs the given arguments at the the given log level.
        /// \param level    The Log::Level to log at.
        /// \param args     The stuff to log.
        template<typename... Args>
        void Log(Level level, Args... args) {
            // date/time formatting
            char strbuf[20];
            std::time_t tm = std::time(nullptr);
            std::strftime(strbuf, sizeof(strbuf), Logger::TIME_FORMAT.c_str(), std::localtime(&tm));

            printf("[ %s | %10s | %-8s ] ", strbuf, this->name.c_str(), Logger::LEVELS[level].c_str());
            RecursiveArgs(args...);
            std::cout << std::endl;
        }

        // Log at the corresponding level.
        template<typename... Args> void Debug(Args... args) { Log(Level::DEBUG, args...); }
        template<typename... Args> void Info(Args... args) { Log(Level::INFO, args...); }
        template<typename... Args> void Warn(Args... args) { Log(Level::WARNING, args...); }
        template<typename... Args> void Error(Args... args) { Log(Level::ERROR, args...); }
        template<typename... Args> void Critical(Args... args) { Log(Level::CRITICAL, args...); }

    private:
        typedef std::unordered_map<std::string, Logger*> LoggerMap;

        // String representation of the log levels
        const static std::string LEVELS[5];
        // Format string for the date/time in the log output
        const static std::string TIME_FORMAT;

        // The available Loggers mapped by their name
        static LoggerMap loggers;

        // Private constructor prevents creation of Loggers
        Logger(std::string name) : name(name) {};

        // Utility function to recursively print arguments
        // See also: http://stackoverflow.com/a/16338804
        template<typename T>
        static void RecursiveArgs(T arg) { std::cout << arg; }
        template<typename T, typename... Args>
        static void RecursiveArgs(T arg, Args... args) {
            RecursiveArgs(arg);
            std::cout << " ";
            RecursiveArgs(args...);
        }
    };

    // C++ prevents us from doing this in-class because the expression are not `constexpr`..
    const std::string Logger::LEVELS[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    const std::string Logger::TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
    Logger::LoggerMap Logger::loggers = Logger::LoggerMap();

}

#endif //ACU_FW_LOGGER_H
