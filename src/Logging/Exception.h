#ifndef GLOBALEXCEPTIONHANDLER_H
#define GLOBALEXCEPTIONHANDLER_H

#include <csignal>
#include <iostream>
#include <windows.h>
#include <sstream>

/**
 * Helper class for handling exceptional events that occur during the operation
 * of Drautos.
 */
class Exception
{
private:
    inline static auto isUsingConsole_ = false;

public:
    static void SetIsUsingConsole(const bool isUsingConsole)
    {
        isUsingConsole_ = isUsingConsole;
    }

    /**
     * Informs the user of the fatal event, then terminates the game.
     */
    static void Fatal()
    {
        // Inform the user, then shut the game down
        const std::string empty;
        OnBeforeTerminate(empty);
        std::_Exit(EXIT_FAILURE);
    }

    /**
     * Informs the user of a fatal event, then terminates the game.
     * @param message The message to display before terminating.
     */
    static void Fatal(const std::string& message)
    {
        OnBeforeTerminate(message);
        std::_Exit(EXIT_FAILURE);
    }

private:
    /**
     * Runs before the application is terminated to inform the user of what went
     * wrong.
     * @param message The message to display before terminating the process.
     * @remarks When console is enabled, waits for the user to read the console
     * and press a key before terminating. Otherwise, a message box is displayed
     * informing the user that the mod loader has failed.
     */
    static void OnBeforeTerminate(const std::string& message)
    {
        // TODO: Revert this function once we solve the bug with logging

        if (isUsingConsole_)
        {
            if (!message.empty())
            {
                std::cerr << message << std::endl;
            }

            // Wait for user to press a key in the console before terminating
            std::cerr << "Press any key to exit...\n";
            std::cerr.flush();
            std::cin.get();
        }
        else
        {
            std::stringstream stream;
            stream << "An unexpected error occurred in the mod loader.";

            if (!message.empty())
            {
                stream << "\n\n";
                stream << message;
            }

            // Wait for the user to dismiss the message box before terminating
            MessageBox(nullptr,
                stream.str().c_str(),
                "Error", MB_OK);
        }
    }
};

#endif // GLOBALEXCEPTIONHANDLER_H
