#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <stdexcept>

/**
 * Configuration for Drautos.
 */
struct Configuration
{
  private:
    inline static HANDLE hMappedFile = nullptr;
    inline static Configuration* pInstance = nullptr;

  public:
    Configuration(const Configuration&) = delete;

    Configuration& operator=(const Configuration&) = delete;

    static Configuration& GetInstance()
    {
        if (!pInstance)
        {
            // Open shared memory that was written by C#
            hMappedFile =
                OpenFileMapping(FILE_MAP_READ, false, "DrautosConfiguration");
            if (!hMappedFile)
            {
                throw cpptrace::runtime_error("Failed to open configuration.");
            }

            // Map this configuration struct from shared memory
            pInstance = static_cast<Configuration*>(MapViewOfFile(
                hMappedFile, FILE_MAP_READ, 0, 0, sizeof(Configuration)));

            // Ensure the configuration was mapped
            if (!pInstance)
            {
                CloseHandle(hMappedFile);
                throw cpptrace::runtime_error("Failed to map configuration.");
            }
        }

        return *pInstance;
    }

    /**
     * Whether to open a console and redirect output to it.
     */
    bool EnableConsole;

    /**
     * Whether ansel should be patched to allow it to function whenever the
     * player likes.
     */
    bool EnableAnselPatch;

    /**
     * Whether unreleased and limited time DLC should be unlocked.
     */
    bool UnlockAdditionalDlc;

    /**
     * Whether the snapshot limit should be increased from 200 to 9999.
     */
    bool IncreaseSnapshotLimit;

  private:
    ~Configuration()
    {
        if (pInstance)
        {
            UnmapViewOfFile(pInstance);
            pInstance = nullptr;
        }

        if (hMappedFile)
        {
            CloseHandle(hMappedFile);
            hMappedFile = nullptr;
        }
    }
};

#endif // CONFIGURATION_H
