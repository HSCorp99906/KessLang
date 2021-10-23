#include "../include/Core.hpp"
#include "../include/Token.hpp"

/* CORE FILE TO HANDLE SYNTAX
 * STUFF AND EXECUTION*/


unsigned int lineNum = 0;
bool possibleVar = false;
bool outStringCalled = false;
std::map<unsigned int, std::string> outs;
std::vector<std::string> lines;


enum BuiltIn : unsigned int {
    OUT = 0,
    BACKWARD = 1,
    INT_DEF = 2,
    EMPTY = 3,
    PRE_INC = 4,
    POST_INC = 5,
    PRE_DEC = 6,
    POST_DEC = 7,
    FILE_READ_OUT = 8,
    STRING_DEF = 9
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
            break;
        } else if (parse == "__EMPTY__") {
            builtInUsed = EMPTY;
            break;
        } else if (parse == "++") {
            builtInUsed = PRE_INC;
            break;
        } else if (regex_match(parse, std::regex("[a-zA-Z]\\d?+\\+\\+"))) {
            builtInUsed = POST_INC;
            break;
        } else if (parse == "--") {
            builtInUsed = PRE_DEC;
            break;
        } else if (std::regex_match(parse, std::regex("[A-Za-z]+(\\d?)+--"))) {
            builtInUsed = POST_DEC;
            break;
        } else if (parse == "__file_read_out__") {
            builtInUsed = FILE_READ_OUT;
            break;
        } else if (parse == "str") {
            builtInUsed = STRING_DEF;
            break;
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

    std::string noCommentLine = "";
    bool endOfLine = false;

    for (int i = 0; i < line.size(); ++i) {

        if (line[i] == ';') {
            endOfLine = true;
        }

        if (line[i] == ' ' && endOfLine) {
            noCommentLine = line.substr(0, i);
            line = noCommentLine;
            break;
        }
    }

    if (std::regex_match(line, std::regex("^//.*"))) {
        line = "__EMPTY__;";
    }

    lines.push_back(line);
    ++lineNum;

    unsigned int builtInUsed;
    Token ptToken;

    ptToken << line;
    ptToken.setBuiltIn(builtInUsed);

    if (line[line.size() - 1] != ';') {
        exit_err("ERROR: Missing semicolen on line: " + std::to_string(lineNum));
    }

    bool openParen = false;
    bool closedParen = false;
    bool openQuote = false;
    bool closedQuote = false;

    bool varDefined = false;

    std::string parsed = "";

    int varInt;
    std::stringstream intStream;
    std::string varBuffer = "";
    std::regex findInt("[0-9\\-]+");

    switch (builtInUsed) {
        case OUT:
            for (int i = 0; i < line.size(); ++i) {
                if (line[i] == '"' && !(openQuote)) {
                    openQuote = true;
                    outStringCalled = true;
                    ptToken ^ outs[lineNum];
                } else if (line[i] == '"' && openQuote) {
                    closedQuote = true;
                } else if (line[i] == '"' && closedQuote) {
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

            if (!(openQuote) && !(closedQuote)) {
                possibleVar = true;
            } else {
                possibleVar = false;
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
        case PRE_INC:
            for (int i = 2; i < line.size() - 1; ++i) {
                parsed += line[i];
            }

            if (!(std::regex_match(parsed, std::regex("[A-Za-z]\\d?+")))) {
                exit_err("ERROR: Expected varname after \"++\" token.");
            }

            break;
        case PRE_DEC:
            for (int i = 2; i < line.size() - 1; ++i) {
                parsed += line[i];
            }

            if (!(std::regex_match(parsed, std::regex("[A-Za-z]\\d?+")))) {
                exit_err("ERROR: Expected varname after \"++\" token.");
            }

            break;
        case FILE_READ_OUT:
            {
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
        case STRING_DEF:
            {
                bool defined = false;
                unsigned short int quotes = 0;

                for (int i = line.size() - 2; i > -1; --i) {
                    if (line[i] == '"') {
                        ++quotes;
                    }

                    if (line[i] == '=' && !(defined)) {
                        defined = true;
                    } else if (line[i] == '=' && defined) {
                        exit_err("ERROR: Too many equal operators on line: " + std::to_string(lineNum));
                    }
                }

                if (!(defined)) {
                    exit_err("ERROR: Implicit variable found on line: " + std::to_string(lineNum));
                }

                if (quotes > 2) {
                    exit_err("ERROR: Too many quotes on line: " + std::to_string(lineNum));
                } else if (quotes == 0) {
                    exit_err("ERROR: No quotes on line: " + std::to_string(lineNum));
                }
            }

            break;
        }

        if (!(openParen) || !(closedParen)) {
            exit_err("ERROR: Missing parenthesis on line: " + std::to_string(lineNum));
        }

        {
            std::string filename = "";

            for (int i = 19; i < line.size() - 3; ++i) {
                filename += line[i];
            }

            std::ifstream readfile(filename);

            if (!(readfile)) {
                readfile.close();
                exit_err("ERROR: File \"" + filename + "\"" + " does not exist on line: " + std::to_string(lineNum));
            } else {
                readfile.close();
            }
        }

    }

    if (openParen && !(closedParen) || !(openParen) && closedParen) {
        exit_err("ERROR: Lingering parenthesis on line: " + std::to_string(lineNum));
    } else if (openQuote && !(closedQuote) || !(openQuote) && closedQuote) {
        exit_err("ERROR: Lingering quotes on line: " + std::to_string(lineNum));
    }

}


void execute() {
    std::map<std::string, short int> intVars;
    std::map<std::string, std::string> stringVars;

    Token rtToken;
    unsigned int internalLineNum = 0;
    bool canPrintVar = true;
    bool canPrint = true;

    for (int i = 0; i < lines.size(); ++i) {
        unsigned int biu;  // Built in used.
        rtToken << lines[i];
        rtToken.setBuiltIn(biu);

        switch (biu) {
            // INT DEF is up here due to bugs.
            case INT_DEF:
                {

                    std::string varKey = "";
                    std::string varVal = "";
                    short int varValInt;

                    for (int j = 4; j < lines[i].size(); j++) {
                        if (lines[i][j] != ' ') {
                            varKey += lines[i][j];
                        } else {
                            for (int e = j + 3; e < lines[i].size() - 1; ++e) {
                                varVal += lines[i][e];
                            }

                            break;
                        }
                    }

                    std::stringstream toInt(varVal);
                    toInt >> varValInt;

                    intVars[varKey] = varValInt;
                }

                break;
        }
    }

    std::vector<std::string> to_ignore;  // Lines to ignore.
    for (int _line = 0; _line < lines.size(); ++_line) {
        canPrintVar = true;

        ++internalLineNum;

        std::string varKey = "";
        std::string varValString = "";
        std::stringstream toInt;
        int varVal;

        std::string line = lines[_line];
        unsigned int builtInUsed;

        rtToken << line;
        rtToken.setBuiltIn(builtInUsed);

        std::string stdoutBuffer;
        std::string stdoutBakBuffer = "";

        bool openQuote = false;
        bool closedQuote = false;

        bool quote = false;

        if (builtInUsed)

        if (possibleVar) {
            canPrintVar = true;
        }

        bool ignore = false;

        switch (builtInUsed) {
            case OUT:
                if (lines[_line][4] == '"') {
                    possibleVar = false;
                } else {
                    possibleVar = true;
                }

                if (!(possibleVar)) {
                    rtToken ^ stdoutBuffer;
                    std::cout << stdoutBuffer << std::endl;
                } else {
                    rtToken ^ varKey;
                    if (!(intVars.count(varKey))) {
                        if (!(stringVars.count(varKey))) {
                            exit_err("RUNTIME ERROR: Trying to output non-existing var on line: " + std::to_string(internalLineNum));
                        } else {
                            std::cout << stringVars[varKey] << std::endl;
                        }
                    } else {
                        std::cout << intVars[varKey] << std::endl;
                    }
                }

                break;
            case BACKWARD:
                rtToken ^ stdoutBuffer;

                for (int i = stdoutBuffer.size() - 1; i > -1; --i) {
                    stdoutBakBuffer += stdoutBuffer[i];
                }

                std::cout << stdoutBakBuffer << std::endl;
                break;
            case PRE_INC:
                for (int i = 2; i < line.size() - 1; ++i) {
                    varKey += line[i];
                }

                if (intVars.count(varKey)) {
                    if (intVars[varKey] + 1 > 32765) {
                        exit_err("RUNTIME ERROR: Integer overflow on line: " + std::to_string(internalLineNum));
                    } else {
                        ++intVars[varKey];
                    }
                } else {
                    exit_err("RUNTIME ERROR: Trying to increment non-existing var on line: " + std::to_string(internalLineNum));
                }

                break;
            case POST_INC:
                for (int i = 0; i < line.size() - 3; ++i) {
                    varKey += line[i];
                }

                if (intVars.count(varKey)) {
                    if (intVars[varKey] + 1 > 32765) {
                        exit_err("RUNTIME ERROR: Integer overflow on line: " + std::to_string(internalLineNum));
                    } else {
                        ++intVars[varKey];
                    }
                } else {
                    exit_err("RUNTIME ERROR: Trying to increment non-existing var on line: " + std::to_string(internalLineNum));
                }

                break;
            case PRE_DEC:
                for (int i = 2; i < line.size() - 1; ++i) {
                    varKey += line[i];
                }

                if (intVars.count(varKey)) {
                    if (intVars[varKey] - 1 < -32765) {
                        exit_err("RUNTIME ERROR: Integer underflow on line: " + std::to_string(internalLineNum));
                    } else {
                        --intVars[varKey];
                    }
                } else {
                    exit_err("RUNTIME ERROR: Trying to decrement non-existing var on line: " + std::to_string(internalLineNum));
                }

                break;
            case POST_DEC:
                for (int i = 0; i < line.size() - 3; ++i) {
                    varKey += line[i];
                }

                if (intVars.count(varKey)) {
                    if (intVars[varKey] - 1 < -32765) {
                        exit_err("RUNTIME ERROR: Integer underflow on line: " + std::to_string(internalLineNum));
                    } else {
                        --intVars[varKey];
                    }
                } else {
                    exit_err("RUNTIME ERROR: Trying to decrement non-existing var on line: " + std::to_string(internalLineNum));
                }

                break;
            case FILE_READ_OUT:
                {

                    std::string filename = "";

                    for (int i = 19; i < line.size() - 3; ++i) {
                        filename += line[i];
                    }

                    std::string readpath = "~/KessLang/src/FileHandling/READ ";
                    readpath += filename;
                    const char* ccharp_readpath = readpath.c_str();
                    std::string fileContents = std::to_string(system(ccharp_readpath));
                    fileContents = std::regex_replace(fileContents, std::regex("[0-9]$"), "");

                    std::cout << fileContents << std::endl;
                }

                break;

            case STRING_DEF:
                {
                    std::string varKey = "";
                    std::string varVal = "";

                    for (int i = 4; i < line.size() - 1; ++i) {
                        if (line[i] != ' ') {
                            varKey += line[i];
                        } else {
                            for (int j = i + 3; j < line.size() - 1; ++j) {
                                if (line[j] != '"') {
                                    varVal += line[j];
                                }
                            }

                            break;
                        }
                    }

                    stringVars[varKey] = varVal;
                }

                break;

        }
    }
}


// END OF CORE FILE.
