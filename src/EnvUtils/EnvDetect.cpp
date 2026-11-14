#include "EnvDetect.hpp"

namespace Environment {
    Environment::Environment() {
        // Hopefully this is fine in CRT init..
        this->type = DetectEnvType();
    }

    EnvironmentType Environment::DetectEnvType() {
        EnvironmentType ret = EnvironmentType::UNKNOWN;

        // Check for WinRE/WinPE first..
        RegKey miniNTKey;
        RegResult miniNTRes = miniNTKey.TryOpen(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\MiniNT", KEY_READ);
        DWORD miniNTResCode = static_cast<DWORD>(miniNTRes.Code());

        switch (miniNTResCode) {
        case ERROR_SUCCESS:
        case ERROR_ACCESS_DENIED: {
            // If somehow we don't have access to this, this error still confirms that
            // the key actually exists in my testing, because Windows is fun like that.
            // Might be possible for this to happen in custom WinPE images though...
            if (miniNTResCode == ERROR_ACCESS_DENIED)
                DbgPrint("We do not have permission to access MiniNT, but it does exist...");

            // There are other ways to check for this, like checking boot flags in
            // registry (SystemStartOptions), but this feels like the most reliable
            // way to do it.
            //
            // Otherwise, there's a chance someone could launch into WinRE through
            // non-standard means (eg. direct from EFI Shell), which wouldn't set
            // those boot flags. That feels slightly more likely than someone
            // running something called RecEnv.exe in a WinPE image outside of actual
            // recovery...
            //
            // (Ab)using tlhelp32 here because it only uses kernel32 and should be
            // available anywhere.
            HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (ss == INVALID_HANDLE_VALUE) {
                DbgPrint("Couldn't get a handle for tlhelp32 process check; assuming we're in WinPE!");
                assert(0);
                ret = EnvironmentType::WINPE;
            }

            PROCESSENTRY32W entry{};
            entry.dwSize = sizeof(entry);

            if (Process32FirstW(ss, &entry)) {
                do {
                    if (_wcsicmp(entry.szExeFile, L"recenv.exe") == 0) {
                        DbgPrint("Seems like we're in a WinRE session!");
                        ret = EnvironmentType::RECOVERY;
                        break;
                    }
                } while (Process32NextW(ss, &entry));
            }

            if (ss != INVALID_HANDLE_VALUE)
                CloseHandle(ss);

            if (ret != EnvironmentType::RECOVERY) {
                DbgPrint("Seems like we're in a WinPE image!");
                ret = EnvironmentType::WINPE;
            }
            break;
        }

        case ERROR_FILE_NOT_FOUND:
            // We have access and didn't find it; shouldn't happen in recovery mode or
            // in WinPE.
            // Therefore, we should be in either normal boot, or one of the safe modes.
            DbgPrint("Didn't find MiniNT; we aren't in Recovery or WinPE...");
            break;

        default:
            // Hopefully never happens! :)
            DbgPrint("Not sure about MiniNT... Got LSTATUS {}!", miniNTResCode);
            assert(0);
            break;
        }
        
        if (ret != EnvironmentType::UNKNOWN)
            return ret;

        // Otherwise, check for safe modes...
        // See https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
        switch (GetSystemMetrics(SM_CLEANBOOT)) {
        case 0:
            DbgPrint("We're booted in normally!");
            ret = EnvironmentType::NORMAL;
            break;

        case 1:
            DbgPrint("We're booted into safe mode!");
            ret = EnvironmentType::SAFE_MODE;
            break;

        case 2:
            DbgPrint("We're booted into safe mode with networking!");
            ret = EnvironmentType::SAFE_MODE_NET;
            break;

        default:
            DbgPrint("Windows is drunk.");
            assert(0);
            break;
        }
        
        return ret;
    }
}