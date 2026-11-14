#pragma once

#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

#include <TlHelp32.h>
#include <assert.h>

#include <WinReg.hpp>
using winreg::RegKey;
using winreg::RegResult;

#include <ftxui/ftxui.hpp>
using namespace ftxui;

#include <iostream>
#include <cwctype>

// TODO: Move to a debug header.
template <typename... Args>
void DbgPrint(std::format_string<Args...> fmt, Args&&... args) {
    std::string msg = std::format(fmt, std::forward<Args>(args)...) + "\n";
    OutputDebugStringA(msg.c_str());
}
    

#include "EnvUtils/EnvDetect.hpp"
using Environment::EnvironmentType;
using Environment::CurrentEnv;
