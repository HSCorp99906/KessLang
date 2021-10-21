#include <fstream>
#include <string>
#include "../include/Core.hpp"

// Made By Ian Moffett 10/19/21

std::ifstream src("kesslang.kess");
extern unsigned int lineNum;

int main(int argc, char* argv[]) {
    src = std::ifstream(argv[1]);

    if (!(src)) {
        std::cout << "File does not exist. (Terminated)" << std::endl;
        src.close();
        exit(1);
    }

    std::string line;

    while (std::getline(src, line)) {
        if (line == "") {
            ++lineNum;
            continue;
        }

        parseAndPrepare(line);
    }

    src.close();
    
    execute();
}
