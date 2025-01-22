#ifndef LOGGING_H
#define LOGGING_H

#include <ShlObj_core.h>
#include <fmt/chrono.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "../Configuration.h"

/**
 * Helper class that sets up logging for the application.
 */
class Logging
{
public:
    static inline std::string LogFilePath;

    /**
     * Initializes logging for the program.
     * @remarks Console logging will only be enabled if set in the
     * configuration.
     */
    static void Initialize()
    {
        // TODO: Delete old logs, as currently they will continue to stack up
        //       forever. Also consider preventing the log file getting to big
        //       in the event that there is spam from a detour function
        LogFilePath = CreateLogPath();

        if (Configuration::GetInstance().EnableConsole)
        {
            InitializeConsole();

            // Create a logger that logs to both a file and the console
            const auto fileSink =
                std::make_shared<spdlog::sinks::basic_file_sink_mt>
                    (LogFilePath, true);

            const auto consoleSink =
                std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

            std::vector<spdlog::sink_ptr> sinks{fileSink, consoleSink};
            const auto logger = std::make_shared<spdlog::logger>(
                "default", begin(sinks), end(sinks));

            set_default_logger(logger);
            spdlog::set_level(spdlog::level::trace);
        }
        else
        {
            // Create a logger that logs to a file
            const auto logger =
                spdlog::basic_logger_mt("default", LogFilePath, true);
            set_default_logger(logger);
            spdlog::set_level(spdlog::level::info);
        }

        // See https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        spdlog::set_pattern("[%H:%M:%S.%e] [thread %t] %^%-8l%$  %v  %@");
        spdlog::flush_on(spdlog::level::trace);

        SPDLOG_INFO("Initialized logging with file {}", LogFilePath);
    }

private:
    /**
     * Creates a file path string for a new log file, where the name contains a
     * timestamp for when the game was launched.
     * @return Absolute file path to the log file.
     */
    static std::string CreateLogPath()
    {
        // Get local app data path
        char path[MAX_PATH];
        if (SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, path) !=
            S_OK)
        {
            throw std::runtime_error("Unable to create log file.");
        }

        // Append the current date/time and return the final path
        const auto now = std::time(nullptr);
        return fmt::format(R"({}\Flagrum\logs\game_{:%Y%m%d%H%M%S}.log)", path,
                           fmt::localtime(now));
    }

    /**
     * Opens a console and redirects standard output and standard error output
     * to it.
     */
    static void InitializeConsole()
    {
        AllocConsole();

        FILE* pCout;
        (void)freopen_s(&pCout, "CONOUT$", "w", stdout);
        (void)freopen_s(&pCout, "CONOUT$", "w", stderr);
        (void)freopen_s(&pCout, "CONIN$", "r", stdin);

        const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleMode(hConsole,
                       ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
    }
};

#endif // LOGGING_H
