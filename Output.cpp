#include "Output.h"

void Output::writeTheFile(std::string input)
{
    if (output.is_open()) {
        output << input << std::endl;
    }
    else {
        std::cerr << "Writing mode cannot work correctly." << std::endl;
    }
}
