#pragma once

namespace Environment {
    enum class EnvironmentState {
        NOT_DETECTED = -2,  // We haven't tried to detect the environment state yet.
        UNKNOWN = -1,       // Unknown environment. Probably booted into some custom WinPE image.
        NORMAL = 0,         // We're running in a standard, fully-booted Windows installation.
        RECOVERY,           // We're running in a WinRE session.
        SAFE_MODE,          // We're running in minimal safe mode; without networking.
        SAFE_MODE_NET,      // We're running in safe mode, with networking.
        WINPE               // We're running in a WinPE session, i.e Shift+F10 from Windows Setup.
    };

    enum class EnvironmentOSRelease {
        NOT_DETECTED = -2,  // We haven't tried to detect the OS release yet.
        UNKNOWN = -1,       // Unknown OS release.
        WINVISTA = 0,       // Windows Vista. (untested)
        WIN7,               // Windows 7.     (untested)
        WIN8,               // Windows 8.     (untested)
        WIN8_1,             // Windows 8.1.   (untested)
        WIN10,              // Windows 10.
        WIN11               // Windows 11.
    };

    enum class EnvironmentOSEdition {
        NOT_DETECTED = -2,
        UNKNOWN = -1,

        /* -- Vista/7-specific editions -- */
        STARTER = 0,
        ULTIMATE,

        /* -- Win10+ -- */
        EDUCATION,          // Standalone Education; generally check against IS_EDUCATION instead!
        IOT,                // May have IS_ENTERPRISE on Win10, always on Win11.
        
        /* -- Win11 only -- */
        SE,                 // Special Edition; heavily stripped down and app installs are cloud-managed.

        /* -- Server-specific editions (you should also check for IS_SERVER flag!) -- */
        FOUNDATION,         // 2008 (R2)/2012 (R2).
        DATACENTER,         // 2008 (R2)/2012 (R2) and later.
        ITANIUM,            // 2008 (R2).
        STORAGE,            // 2008 (R2).
        WEB,                // 2008 (R2).
        STANDARD,           
        ESSENTIALS,         // May have IS_BUSINESS_SVR for Windows Essential Business Server 2008.
        HPC,                // 2008 (R2).
        MULTIPOINT,         // 2010/2011/2012.
        HYPERV,             // Azure Stack HCI and Hyper-V Server

        /* -- Release-agnostic editions -- */
        HOME,               // Also "core" versions. Vista/7 should check against IS_BASIC and IS_PREMIUM. May have IS_SERVER for Home Server.
        PROFESSIONAL,       // Also Vista/7 Business.
        ENTERPRISE,         // Standalone Enterprise; generally check against IS_ENTERPRISE instead!

        /* -- Other misc editions... -- */
        EMBEDDED            // POSReady 7/Standard 7/8 Industry/8.1 Industry.
    };

    enum class EnvironmentOSEditionFlags : DWORD {
        NONE                = 0,

        /* -- Release-agnostic variants -- */
        IS_ENTERPRISE       = 1 << 0,

        /* -- Vista/7's Home variants -- */
        IS_BASIC            = 1 << 1,   // Vista/7 Home Basic.
        IS_PREMIUM          = 1 << 2,   // Vista/7 Home Premium.

        /* -- Win8+ variants -- */
        IS_EDUCATION        = 1 << 3,
        IS_WORKSTATION      = 1 << 4,
        IS_LTSC             = 1 << 5,
        IS_WITH_BING        = 1 << 6,   // For Windows 8.1 with Bing.
        IS_RT               = 1 << 7,   // Win8 RT/Win8.1 RT on some Surface tablets.

        /* -- Server variants -- */
        IS_SERVER           = 1 << 8,
        IS_SMALL_SVR        = 1 << 9,
        IS_BUSINESS_SVR     = 1 << 10,

        /* -- Misc flags... -- */
        IS_CHINA            = 1 << 11,  // China-licensed install.
        IS_SINGLE_LANG      = 1 << 12,  // China + OEMs for a few emerging markets..
        IS_MULTI_SESSION    = 1 << 13,  // For Azure Virtual Desktop (technically a client OS), and server editions.
        IS_N                = 1 << 14,  // Special EU edition which lacks some pre-installed stuff like Windows Media Player.
        IS_E                = 1 << 15,  // Special EU edition which lacks Internet Explorer.
        IS_IN_S_MODE        = 1 << 16,  // Currently restricted to Microsoft Store apps.
        IS_PE               = 1 << 17
    };
    DEFINE_ENUM_FLAG_OPERATORS(EnvironmentOSEditionFlags);

    struct EnvironmentOSEditionInfo {
        EnvironmentOSEdition edition    = EnvironmentOSEdition::NOT_DETECTED;
        EnvironmentOSEditionFlags flags = EnvironmentOSEditionFlags::NONE;
    };

    struct EnvironmentVersionInfo {
        EnvironmentOSRelease     osRelease       = EnvironmentOSRelease::NOT_DETECTED;
        EnvironmentOSEditionInfo osEdition;
        DWORD                    osVerMajor      = 0;
        DWORD                    osVerMinor      = 0;
        DWORD                    osBuildNumber   = 0;
        DWORD                    osUpdBuildRev   = 0;
        DWORD                    osServerYear    = 0;
        DWORD                    osServerRelease = 0;
        std::wstring             osDisplayVer;
        std::wstring             osRawEditionID;        
    };

    class Environment {
    public:
        Environment();

    private:
        std::filesystem::path winDrive;
        std::filesystem::path winRoot;
    };

    class RuntimeEnvironment : public Environment {
    public:
        RuntimeEnvironment();

        const EnvironmentState GetState() const { return state; }
        
    private:
        EnvironmentState DetectEnvState();
        
        EnvironmentState state = EnvironmentState::NOT_DETECTED;
    };

    inline RuntimeEnvironment CurrentRtEnv;
}

#include "../main.hpp"