#pragma once

namespace Environment {
    enum class EnvironmentType {
        NOT_DETECTED = -2,  // We haven't tried to detect the environment type yet.
        UNKNOWN = -1,       // Unknown environment. Probably booted into some custom WinPE image.
        NORMAL = 0,         // We're running in a standard, fully-booted Windows installation.
        RECOVERY,           // We're running in a WinRE session.
        SAFE_MODE,          // We're running in minimal safe mode; without networking.
        SAFE_MODE_NET,      // We're running in safe mode, with networking.
        WINPE               // We're running in a WinPE session, i.e Shift+F10 from Windows Setup.
    };

    class Environment {
    public:
        Environment();

        const EnvironmentType GetType() const { return type; }
        
    private:
        EnvironmentType DetectEnvType();
        
        EnvironmentType type = EnvironmentType::NOT_DETECTED;
    };

    inline Environment CurrentEnv;
}

#include "../main.hpp"