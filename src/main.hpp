#pragma once

#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

#include <TlHelp32.h>

#include <iostream>
#include <filesystem>
#include <assert.h>

#include <WinReg.hpp>
using winreg::RegKey;
using winreg::RegResult;

#include <ftxui/ftxui.hpp>
using namespace ftxui;

#include "EnvUtils/EnvDetect.hpp"
using Environment::EnvironmentState;
using Environment::CurrentRtEnv;

#include "Debug/Debug.hpp"