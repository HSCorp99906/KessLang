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
    std::string parse = "";

    while (std::getline(src, line)) {
        if (line == "") {
            ++lineNum;
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

        parseAndPrepare(line);
    }

    src.close();

    execute();
}
