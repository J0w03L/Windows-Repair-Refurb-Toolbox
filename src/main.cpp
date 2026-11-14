// Windows-Repair-Refurb-Toolbox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.hpp"

int main()
{
    EnvironmentType envType = CurrentEnv.GetType();
    auto document =
        vbox({
            text(
                envType == EnvironmentType::NORMAL          ? "Running in NORMAL mode!" :
                envType == EnvironmentType::RECOVERY        ? "Running in RECOVERY mode!" :
                envType == EnvironmentType::WINPE           ? "Running in WINPE mode!" :
                envType == EnvironmentType::SAFE_MODE       ? "Running in SAFEMODE mode!" :
                envType == EnvironmentType::SAFE_MODE_NET   ? "Running in SAFEMODENET mode!" :
                "Unable to detect environment for running mode; YMMV!"
            ) | border,

            hbox({
                text("one") | border,
                text("two") | border | flex,
                text("three") | border | flex,
            }),

            gauge(0.25) | color(Color::Red),
            gauge(0.50) | color(Color::White),
            gauge(0.75) | color(Color::Blue),
        });

    auto screen = Screen::Create(Dimension::Full());
    Render(screen, document);
    screen.Print();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
