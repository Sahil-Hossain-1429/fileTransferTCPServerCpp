#ifndef DESIGNS_H
#define DESIGNS_H

#include <string>
#include <iostream>

namespace MenuDesigns
{
    const std::string TOPDESIGN = R"(╔═══════════════════════════════════════╗)";

    const std::string BOTTOMDESIGN = R"(╚═══════════════════════════════════════╝)";

    inline void printLineWithDesign(const std::string &text)
    {
        int totalWidth = 39; // Total space between the borders
        int textLength = text.length();
        int leftPadding = totalWidth / 4;
        int rightPadding = totalWidth - textLength - leftPadding;

        std::cout << "║";
        for (int i = 0; i < leftPadding; i++)
            std::cout << " ";
        std::cout << text;
        for (int i = 0; i < rightPadding; i++)
            std::cout << " ";
        std::cout << "║\n";
    }

    inline void printInfoMessage(const std::string &message)
    {
        std::cout << "\n";
        std::cout << "►►► INFO: " << message << " ◄◄◄\n";
        std::cout << "\n";
    }
}

#endif