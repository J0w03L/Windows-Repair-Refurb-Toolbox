@echo off

set "CWD=%~dp0"
set "CWD=%CWD:~0,-1%"

cd "%CWD%"
rmdir /s /q "./external/FTXUI/build"

cmake -S external/FTXUI -B external/FTXUI/build ^
	-A x64 ^
	-DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" ^
	-DCMAKE_POLICY_DEFAULT_CMP0091=NEW

cmake --build external/FTXUI/build --config Debug
cmake --build external/FTXUI/build --config Release