#include <fstream>
#include <string>
#include <regex>
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
    std::string parse = "";

    while (std::getline(src, line)) {
        if (line == "") {
            parseAndPrepare("__EMPTY__;", argv[2]);
            continue;
        }

        for (int i = 0; i < line.size(); ++i) {
            parse += line[i];
            if (parse == "__file_read_out__") {
                std::string newFilename = argv[2];
                newFilename += '/';
                bool begin = true;

                for (int j = 19; j < line.size() - 3; ++j) {
                    if (line[j] == '/' && begin) {
                        newFilename = "";
                    } else {
                        begin = false;
                    }

                    newFilename += line[j];
                }

            }
        }

        parseAndPrepare(line, argv[2]);

        if (std::regex_match(line, std::regex("[A-Za-z][A-Za-z0-9]*=>\\(\\);"))) {
            parseAndPrepare(line, argv[2]);
        }
    }

    for (int i = 0; i < 3; ++i) {
        parseAndPrepare("__EMPTY__;", argv[2]);
    }

    parseAndPrepare("______END______;", argv[2]);

    src.close();

    execute();
}
