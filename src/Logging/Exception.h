#ifndef GLOBALEXCEPTIONHANDLER_H
#define GLOBALEXCEPTIONHANDLER_H

#include <csignal>
#include <exception>
#include <iostream>
#include <windows.h>

#include <cpptrace.hpp>
#include <from_current.hpp>
#include <spdlog/spdlog.h>

#include "../Configuration.h"
#include "Logging.h"

/**
 * Helper class for handling exceptional events that occur during the operation
 * of Drautos.
 */
class Exception
{
public:
    /**
     * Logs a critical error message, informs the user of the fatal event, then
     * terminates the game.
     * @param fmt The `fmt` string to use as the log message for why the event
     * was fatal.
     * @param args Arguments to pass to the format string.
     * @remarks Will attempt to get a stack trace from the current exception if
     * one exists, otherwise will get the stack trace from the current call
     * stack.
     */
    template <typename... Args>
    static void Fatal(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::critical(fmt, std::forward<Args>(args)...);

        // Log the stack trace
        const auto exceptionTrace = cpptrace::from_current_exception();
        if (exceptionTrace.empty())
        {
            spdlog::critical("Stack trace: {}",
                             cpptrace::generate_trace().to_string());
        }
        else
        {
            spdlog::critical("Stack trace: {}",
                             cpptrace::from_current_exception().to_string());
        }

        // Inform the user, then shut the game down
        OnBeforeTerminate();
        std::_Exit(EXIT_FAILURE);
    }

    /**
     * Logs a critical error message, exception message, and stack trace.
     * Finally, informs the user of the fatal event, then terminates the game.
     * @param exception The fatal exception that caused the game to need to
     * terminate.
     * @param fmt The `fmt` string to use as the log message for why the event
     * was fatal.
     * @param args Arguments to pass to the format string.
     */
    template <typename... Args>
    static void Fatal(const cpptrace::exception& exception,
                      spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::critical(fmt, std::forward<Args>(args)...);
        spdlog::critical(exception.what());

        // Inform the user, then shut the game down
        OnBeforeTerminate();
        std::_Exit(EXIT_FAILURE);
    }

    /**
     * Logs a critical error message, exception message, and stack trace.
     * Finally, informs the user of the fatal event, then terminates the game.
     * @param exception The fatal exception that caused the game to need to
     * terminate.
     * @param fmt The `fmt` string to use as the log message for why the event
     * was fatal.
     * @param args Arguments to pass to the format string.
     */
    template <typename... Args>
    static void Fatal(const std::exception& exception,
                      spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::critical(fmt, std::forward<Args>(args)...);
        spdlog::critical("Fatal exception occurred: {}", exception.what());

        // Log the stack trace
        spdlog::critical("Stack trace:");
        for (auto frame : cpptrace::generate_trace())
        {
            spdlog::critical("    {}", frame.to_string());
        }

        // Inform the user, then shut the game down
        OnBeforeTerminate();
        std::_Exit(EXIT_FAILURE);
    }

private:
    /**
     * Runs before the application is terminated to inform the user of what went
     * wrong.
     * @remarks When console is enabled, waits for the user to read the console
     * and press a key before terminating. Otherwise, a message box is displayed
     * informing the user where the log file was dumped.
     */
    static void OnBeforeTerminate()
    {
        if (Configuration::GetInstance().EnableConsole)
        {
            // Wait for the user to press a key in the console before
            // terminating
            std::cerr << "Press any key to exit...\n";
            std::cerr.flush();
            std::cin.get();
        }
        else
        {
            // Wait for the user to dismiss the message box before terminating
            const auto message =
                fmt::format("{}\n\n{} {}.",
                            "An unexpected error occurred in the mod loader.",
                            "A log has been saved to", Logging::LogFilePath);

            MessageBox(nullptr, message.c_str(), "Error", MB_OK);
        }
    }
};

#endif // GLOBALEXCEPTIONHANDLER_H
