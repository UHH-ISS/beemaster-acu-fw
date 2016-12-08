//
// Created by dave on 07/12/16.
//

#ifndef ACU_FW_LOGGER_H
#define ACU_FW_LOGGER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <ctime>

namespace acu {

    class Logger {

    const static std::string Levels[5];
    public:
        std::string Name;
        // The Enum-Values seem to be exposed (Logger::DEBUG, etc.).
        enum Level { DEBUG, INFO, WARNING, ERROR, CRITICAL };

        Logger() {};

        // TODO handle filter levels
        // TODO add variable output (file-output, cerr, ...)

        /// Returns the corresponding Logger.
        /// \param name     The name of the logger
        ///                 (most times the class it is used in).
        /// \return         The requested Logger-Instance.
        static Logger GetLogger(const std::string name) {
            if (Logger::loggers.count(name)) {
                auto logger = Logger::loggers[name];
                return logger;
            }
            Logger logger(name);
            Logger::loggers[name] = logger;
            return logger;
        }

        /// Logs everything to the given loglevel.
        /// \tparam Args
        /// \param level    The Log::Level to log to.
        /// \param args     The stuff to log.
        template<typename... Args>
        void Log(Level level, Args... args) {
            // time
            char strbuff[20];
            std::time_t tm = std::time(NULL);
            std::strftime(strbuff, sizeof(strbuff), "%Y-%m-%d %H:%M:%S", std::localtime(&tm));

            printf("[ %s | %10s | %-8s ] ", strbuff, this->Name.c_str(), Logger::Levels[level].c_str());
            RecursiveArgs(args...);
            std::cout << std::endl;
        }

        // Log for the corresponding level.
        template<typename... Args> void Debug(Args... args) { Log(Level::DEBUG, args...); }
        template<typename... Args> void Info(Args... args) { Log(Level::INFO, args...); }
        template<typename... Args> void Warn(Args... args) { Log(Level::WARNING, args...); }
        template<typename... Args> void Error(Args... args) { Log(Level::ERROR, args...); }
        template<typename... Args> void Critical(Args... args) { Log(Level::CRITICAL, args...); }

    private:
        Logger(std::string name) : Name(name) {};

        typedef std::unordered_map<std::string, Logger> loggermap_t;
        static loggermap_t loggers;

        // Recursive printing of arguments
        // - http://stackoverflow.com/a/16338804
        template<typename T>
        static void RecursiveArgs(T arg) { std::cout << arg; }
        template<typename T, typename... Args>
        static void RecursiveArgs(T arg, Args... args) {
            RecursiveArgs(arg);
            std::cout << " ";
            RecursiveArgs(args...);
        }
    };

    const std::string Logger::Levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    Logger::loggermap_t Logger::loggers = Logger::loggermap_t();
}

#endif //ACU_FW_LOGGER_H
