#include "../include/Core.hpp"
#include "../include/Token.hpp"

/* CORE FILE TO HANDLE SYNTAX
 * STUFF AND EXECUTION*/


unsigned int lineNum = 0;


enum BuiltIn : unsigned int {
    OUT = 0,
    BACKWARD = 1,
    INT_DEF = 2
};


void Token::operator<<(std::string input) {
    this -> line = input;
}


void Token::operator>>(std::string& output) {
    this -> line != "" ? output = this -> line : output = "__EMPTY__";
}


void Token::operator^(std::string& output) {
    std::string _output = "";
    bool openParen = false;

    for (int i = 0; i < this -> line.size() - 1; ++i) {
        if (this -> line[i] == '(') {
            openParen = true;
            ++i;
        }

        if (openParen && this -> line[i] != ')' && line[i] != '"') {
            _output += this -> line[i];
        } else if (this -> line[i] == ')' || this -> line[i] == '(' && openParen) {
            break;
        }
    }

    output = _output;
}


void Token::setBuiltIn(unsigned int& builtInUsed) {
    std::string parse = "";

    for (int i = 0; i < this -> line.size(); ++i) {
        parse += this -> line[i];
        if (parse == "out") {
            builtInUsed = OUT;
            break;
        } else if (parse == "bak") {
            builtInUsed = BACKWARD;
            break;
        } else if (parse == "int") {
            builtInUsed = INT_DEF;
        }
    }
}

void exit_err(std::string message) {
    extern std::ifstream src;
    std::cout << message << std::endl;
    src.close();
    exit(1);
}


void parseAndPrepare(std::string line) {
    ++lineNum;

    unsigned int builtInUsed;
    Token ptToken;

    ptToken << line;
    ptToken.setBuiltIn(builtInUsed);

    if (line[line.size() - 1] != ';' && line != "") {
        exit_err("ERROR: Missing semicolen on line: " + std::to_string(lineNum));
    }


    bool openParen = false;
    bool closedParen = false;
    bool openQuote = false;
    bool closedQuote = false;

    bool varDefined = false;

    int varInt;
    std::stringstream intStream;
    std::string varBuffer = "";
    std::regex findInt("[0-9\\-]+");

    switch (builtInUsed) {
        case OUT:
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == '"' && !(openQuote)) {
                    openQuote = true;
                } else if (line[i] == '"' && openQuote) {
                    closedQuote = true;
                } else if (line[i] == '"' && openQuote && closedQuote) {
                    exit_err("ERROR: Too many quotes on line: " + std::to_string(lineNum));
                }

                if (line[i] == '(' && !(openParen)) {
                    openParen = true;
                } else if (line[i] == ')' && !(closedParen)) {
                    closedParen = true;
                } else if (line[i] == '(' && openParen || line[i] == ')' && closedParen) {
                    exit_err("ERROR: Too many parenthesis on line: " + std::to_string(lineNum));
                }
            }

            if (!(openParen) || !(closedParen)) {
                exit_err("ERROR: Missing parenthesis on line: " + std::to_string(lineNum));
            }

            break;
        case BACKWARD:
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == '"' && !(openQuote)) {
                    openQuote = true;
                } else if (line[i] == '"' && openQuote) {
                    closedQuote = true;
                } else if (line[i] == '"' && openQuote && closedQuote) {
                    exit_err("ERROR: Too many quotes on line: " + std::to_string(lineNum));
                }

                if (line[i] == '(' && !(openParen)) {
                    openParen = true;
                } else if (line[i] == ')' && !(closedParen)) {
                    closedParen = true;
                } else if (line[i] == '(' && openParen || line[i] == ')' && closedParen) {
                exit_err("ERROR: Too many parenthesis on line: " + std::to_string(lineNum));
                }
            }

            break;
        case INT_DEF:

            for (int i = 4; i < line.size() - 1 && line[i] != ' '; ++i) {
                varBuffer += line[i];
            }

            std::cout << varBuffer << std::endl;

            if (varBuffer == "out") {
                exit_err("ERROR: Variable with name of a reserved keyword on line: " + std::to_string(lineNum));
            } else if (varBuffer == "int") {
                exit_err("ERROR: Variable with name of a reserved keyword on line: " + std::to_string(lineNum));
            } else if (varBuffer == "bak") {
                exit_err("ERROR: Variable with name of a reserved keyword on line: " + std::to_string(lineNum));
            }

            varBuffer = "";

            if (line[4] == ' ') {
                exit_err("ERROR: Too many whitespaces on line: " + std::to_string(lineNum));
            }

            for (int i = 3; i < line.size(); ++i) {
                if (line[i] == '=') {
                    varDefined = true;

                    for (int j = i + 1; j < line.size() - 1; ++j) {
                        if (line[j] != ' ') {   // TODO: Make spaces in var name illegal.
                            varBuffer += line[j];
                        }
                    }
                }
            }

            if (!(std::regex_match(varBuffer, findInt))) {  // Allow vars to be equal to each other.
                exit_err("ERROR: Invalid data for type int on line: " + std::to_string(lineNum));
            }

            if (!(varDefined)) {
                exit_err("ERROR: Implicit variable found on line: " + std::to_string(lineNum));
            }

            if (varBuffer == "-") {
                exit_err("ERROR: Expected numeric value after \"-\" on line: " + std::to_string(lineNum));
            }

            intStream << varBuffer;
            intStream >> varInt;

            if (varInt > 32765) {
                exit_err("ERROR: Integer overflow on line: " + std::to_string(lineNum));
            } else if (varInt < -32765) {
                exit_err("ERROR: Integer underflow on line: " + std::to_string(lineNum));
            }

            break;
    }

    if (openParen && !(closedParen) || !(openParen) && closedParen) {
        exit_err("ERROR: Lingering parenthesis on line: " + std::to_string(lineNum));
    } else if (openQuote && !(closedQuote) || !(openQuote) && closedQuote) {
        exit_err("ERROR: Lingering quotes on line: " + std::to_string(lineNum));
    }


}


void execute(std::string line) {
    std::map<std::string, short int> intVars;
    unsigned int builtInUsed;

    Token rtToken;
    rtToken << line;
    rtToken.setBuiltIn(builtInUsed);

    std::string stdoutBuffer;
    std::string stdoutBakBuffer = "";

    std::string varKey = "";
    std::string varValString = "";
    std::stringstream toInt;
    int varVal;

    switch (builtInUsed) {
        case OUT:
            rtToken ^ stdoutBuffer;
            std::cout << stdoutBuffer << std::endl;
            break;
        case BACKWARD:
            rtToken ^ stdoutBuffer;

            for (int i = stdoutBuffer.size() - 1; i > -1; --i) {
                stdoutBakBuffer += stdoutBuffer[i];
            }

            std::cout << stdoutBakBuffer << std::endl;
            break;
        case INT_DEF:
            for (int i = 4; line[i] != ' ' && i < line.size() - 1; ++i) {
                varKey += line[i];
            }

            for (int i = 4; i < line.size() - 1; ++i) {
                if (line[i] == '=') {
                    for (int j = i; j < line.size() - 1; ++j) {
                        if (line[j] != '=' && line[j] != ' ') {
                            varValString += line[j];
                        }
                    }

                    break;
                }
            }

            toInt << varValString;
            toInt >> varVal;

            intVars[varKey] = varVal;

            break;
    }
}
