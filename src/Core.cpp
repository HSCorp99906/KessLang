#include "../include/Core.hpp"
#include "../include/Token.hpp"


/* CORE FILE TO HANDLE SYNTAX
 * STUFF AND EXECUTION*/

 extern std::ifstream src;


unsigned int lineNum = 0;
bool possibleVar = false;
bool outStringCalled = false;
std::map<unsigned int, std::string> outs;
std::vector<std::string> lines;
std::ofstream outFile;
std::vector<std::string> cb_files;

unsigned int c_blocks = 0;


void exit_err(std::string message) {
    outFile.close();
    std::cout << message << std::endl;
    src.close();
    exit(1);
}


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
    STRING_DEF = 9,
    C_CODE = 10,
    END = 11,
    NEWLINE = 12,
    IF_STATEMENT = 13,
    IN = 14,
    STOP = 15,
    STRING_ADDON = 16,
    FUNCTION_DEF = 17,
    FUNCTION_CALL = 18,
    GAP = 19,
    VAR_DUMP = 20,
    COMMENT = 21,
    RE_ASSIGN = 22
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
    bool tokenFound = false;

    for (int i = 0; i < this -> line.size(); ++i) {
        parse += this -> line[i];
        if (parse == "out") {
            tokenFound = true;
            builtInUsed = OUT;
            break;
        } else if (parse == "bak") {
            tokenFound = true;
            builtInUsed = BACKWARD;
            break;
        } else if (parse == "int") {
            tokenFound = true;
            builtInUsed = INT_DEF;
            break;
        } else if (parse == "__EMPTY__") {
            tokenFound = true;
            builtInUsed = EMPTY;
            break;
        } else if (parse == "++") {
            tokenFound = true;
            builtInUsed = PRE_INC;
            break;
        } else if (regex_match(parse, std::regex("[a-zA-Z]\\d?+\\+\\+"))) {
            tokenFound = true;
            builtInUsed = POST_INC;
            break;
        } else if (parse == "--") {
            tokenFound = true;
            builtInUsed = PRE_DEC;
            break;
        } else if (std::regex_match(parse, std::regex("[A-Za-z]+(\\d?)+--"))) {
            tokenFound = true;
            builtInUsed = POST_DEC;
            break;
        } else if (std::regex_match(parse, std::regex("__file_read_out__\\(\"[A-Za-z0-9]+\"\\);"))) {
            tokenFound = true;
            builtInUsed = FILE_READ_OUT;
            break;
        } else if (parse == "str") {
            tokenFound = true;
            builtInUsed = STRING_DEF;
            break;
        } else if (parse == "C_START:") {
            tokenFound = true;
            builtInUsed = C_CODE;
            break;
        } else if (parse == "______END______") {
            tokenFound = true;
            builtInUsed = END;
            break;
        } else if (parse == "\n") {
            tokenFound = true;
            builtInUsed == NEWLINE;
            break;
        } else if (parse == "if") {
            tokenFound = true;
            builtInUsed = IF_STATEMENT;
            break;
        } else if (parse == "stop") {
            tokenFound = true;
            builtInUsed = STOP;
            break;
        } else if (std::regex_match(parse, std::regex("([a-zA-Z]+\\d*\\s*+\\+=\\s*\"{1}[^\"]+\";|[a-zA-Z]+\\d*\\s*+\\+=\\s*\[A-Za-z0-9];)"))) {
            tokenFound = true;
            builtInUsed = STRING_ADDON;
            break;
        } else if (std::regex_match(parse, std::regex("func\\s{1}[A-Za-z]+[A-Z0-9a-z]*\\(\\)\\s*\\{"))) {
            tokenFound = true;
            builtInUsed = FUNCTION_DEF;
            break;
        } else if (std::regex_match(parse, std::regex("[A-Za-z][A-Za-z0-9]*=>\\(\\)"))) {
            tokenFound = true;
            builtInUsed = FUNCTION_CALL;
            break;
        } else if (parse == "______GAP______;") {
            tokenFound = true;
            builtInUsed = GAP;
            break;
        } else if (parse == "__VAR_DUMP__") {
            tokenFound = true;
            builtInUsed = VAR_DUMP;
            break;
        } else if (parse == "//") {
            tokenFound = true;
            builtInUsed = COMMENT;
            break;
        } else if (std::regex_match(parse, std::regex("\\w+(\\d\\w)*\\s*=\\s*(\\d+|\".*\")"))) {
            tokenFound = true;
            builtInUsed = RE_ASSIGN;
            break;
        } else if (std::regex_match(parse, std::regex("if\\s+\\(.+\\)\\s*\\{"))) {
            tokenFound = true;
            builtInUsed = IF_STATEMENT;
            break;
        }
    }
}


void parseAndPrepare(std::string line, std::string ed) {
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

    static bool c_def = false;
    static bool c_def_end = false;

    typedef unsigned short int short_uint;

    static std::string current_c_block = "";

    static bool ifBlock = false;
    static bool ifBlockBegin = false;
    static short_uint indentLevel = 0;

    static bool functionActive = false;
    static bool functionBegin = false;
    static short_uint fIndentLevel = 0;

    ptToken << line;
    ptToken.setBuiltIn(builtInUsed);

    if (c_def && !(c_def_end)) {
        builtInUsed = C_CODE;
    } else if (c_def_end) {
        c_def = false;
        builtInUsed = NEWLINE;
        line = "\n;";
    }

    if (functionActive) {
        builtInUsed = FUNCTION_DEF;
    }

    if (ifBlock || ifBlockBegin) {
        builtInUsed = IF_STATEMENT;
    }

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
        case IF_STATEMENT:
            if (!(ifBlockBegin) && !(ifBlock)) {
                ifBlockBegin = true;
                break;
            } else if (ifBlockBegin) {
                ifBlockBegin = false;
                ifBlock = true;


                for (int i = 0; line[i] == ' ' && i < line.size(); ++i) {
                    ++indentLevel;
                }
            } else {
                short_uint indentMatch = 0;

                for (int i = 0; i < line.size() && line[i] == ' '; ++i) {
                    ++indentMatch;
                }

                if (line[0] == '}') {
                    indentLevel = 0;
                    ifBlock = false;
                    break;
                }

                if (indentMatch < indentLevel && indentMatch > 0 && line[0] != '}') {
                    exit_err("ERROR: Indent error on line: " + std::to_string(lineNum));
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
                    std::string parse = "";
                    bool error = true;

                    for (int i = 8; i < line.size() - 1; ++i) {
                        parse += line[i];
                        if (parse == "in") {
                            error = false;
                        }
                    }

                    if (error) {
                        std::string plusEq = "";

                        std::smatch m;

                        std::regex_search(line, m, std::regex("\\+="));

                        for (auto i: m) {
                            plusEq = i;
                            break;
                        }

                        if (plusEq != "+=") {
                            exit_err("ERROR: No quotes on line: " + std::to_string(lineNum));
                        }
                    }
                }
            }

            break;
        case C_CODE:
            {
                c_def_end = false;

                if (!(c_def)) {
                    c_def = true;
                    ++c_blocks;
                    std::string fileName = ".kesslangC_";
                    fileName += std::to_string(c_blocks);
                    cb_files.push_back(fileName);

                    outFile.open(fileName);
                }

                line += "\n";

                std::string lineCopy = std::regex_replace(line, std::regex("C_START:"), "");
                lineCopy = std::regex_replace(lineCopy, std::regex("C_END"), "");

                outFile << lineCopy;

                std::string _line = "";
                std::smatch match;

                std::regex_search(line, match, std::regex("C_END"));

                for (auto i: match) {
                    if (i == "C_END") {
                        c_def_end = true;
                        break;
                    }
                }
            }

            break;
        case FUNCTION_DEF:
            if (!(functionBegin)) {
                functionBegin = true;
            }

            if (functionBegin) {
                if (!(functionActive)) {
                    functionActive = true;

                    for (int i = 0; line[i] == ' ' && i < line.size(); ++i) {
                        ++fIndentLevel;
                    }
                } else {
                    unsigned short int indentMatch = 0;

                    for (int i = 0; line[i] == ' ' && i < line.size(); ++i) {
                        ++indentMatch;
                    }

                    if (std::regex_match(line.substr(indentMatch, line.size() - 1), std::regex("int\\s+[a-zA-Z]+[a-zA-Z0-9]*\\s*=\\s*\\d+;"))) {
                        exit_err("ERROR: Sorry, we currently do not support variables inside of functions.");
                    } else if (std::regex_match(line.substr(indentMatch, line.size() - 1), std::regex("str\\s+[a-zA-Z]+[a-zA-Z0-9]*\\s*=\\s*\"{1}.*\"{1};"))) {
                        exit_err("ERROR: Sorry, we currently do not support variables inside of functions.");
                    }

                    if (line.substr(indentMatch, line.size() - 1) == "C_START:") {
                        exit_err("ERROR: Sorry, cannot have C Blocks in a function.");
                    }

                    if (indentMatch < fIndentLevel && indentMatch != 0) {
                        exit_err("ERROR: Indent error on line: " + std::to_string(lineNum));
                    }
                }
            }

            break;
        }

    }

    if (openParen && !(closedParen) || !(openParen) && closedParen) {
        exit_err("ERROR: Lingering parenthesis on line: " + std::to_string(lineNum));
    } else if (openQuote && !(closedQuote) || !(openQuote) && closedQuote) {
        exit_err("ERROR: Lingering quotes on line: " + std::to_string(lineNum));
    }

    bool functionEndCaught = false;

    if (builtInUsed == FUNCTION_DEF && line[0] == '}') {
        functionBegin = false;
        functionActive = false;
        functionEndCaught = true;
    }

    if (line == "______END______;" && functionActive) {
        exit_err("ERROR: Expected \"}\" after function definition.");
    }

    if (line == "______END______;" && ifBlock) {
        exit_err("ERROR: Expected \"}\" after if statement.");
    }

    functionEndCaught = false;

    if (line[line.size() - 1] != ';' && !(c_def) && builtInUsed != IF_STATEMENT && !(ifBlock) && builtInUsed != FUNCTION_DEF && line != "}") {
        exit_err("ERROR: Missing semicolen on line: " + std::to_string(lineNum));
    }

    if (c_def_end) {
        c_def = false;
        outFile.close();
        std::string command = "python3 ~/KessLang/src/Utility/CleanCBlock.py ";
        command += ed.c_str();
        command += " ";
        command += " .kesslangC_";
        command += std::to_string(c_blocks).c_str();
        system(command.c_str());
    }

    std::smatch match;

    std::regex_search(line, match, std::regex("______END______;"));

    for (auto i: match) {
        if (c_def && !(c_def_end) && i == "______END______;")  {
            outFile.close();
            exit_err("ERROR: Expected \"C_END\" on line: " + std::to_string(lineNum - 1));
        }
    }

}


void execute() {
    std::map<std::string, short int> intVars;
    std::map<std::string, std::string> stringVars;
    std::map<std::string, std::vector<std::string>> functions;
    std::vector<std::string> ifExecBuffer;

    typedef std::vector<std::string> stringVector;
    stringVector::const_iterator ifExecBufferIt;

    Token rtToken;
    unsigned int internalLineNum = 0;
    bool canPrintVar = true;
    bool canPrint = true;
    bool c_block = false;

    bool readIfBlockCode = false;
    bool ifBlock = false;
    bool pauseIfRead = false;
    std::string ifLine;
    bool ifIsTrue = false;
    unsigned int ifLineNum = 0;

    bool functionActive = false;
    bool funcExec = false;
    bool execEnd = false;
    std::string funcExecKey;
    std::string lastFuncKey = "";
    unsigned int lastFuncLines;
    std::string curFuncLine;
    unsigned int _lineNum = 0;

    for (int i = 0; i < lines.size(); ++i) {
        unsigned int biu;  // Built in used.
        ++_lineNum;

        if (!(funcExec)) {
            rtToken << lines[i];
            rtToken.setBuiltIn(biu);
        }

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

                    if (!(intVars.count(varKey) && !(stringVars.count(varKey)))) {
                        intVars[varKey] = varValInt;
                    } else {
                        exit_err("RUNTIME ERROR: Var re-declared or var already exists as another type on line: " + std::to_string(_lineNum));
                    }
                }

                break;
        }
    }

    unsigned int builtInUsed;

    std::vector<std::string> to_ignore;  // Lines to ignore.
    for (int _line = 0; _line < lines.size(); ++_line) {
        canPrintVar = true;

        ++internalLineNum;

        std::string varKey = "";
        std::string varValString = "";
        std::stringstream toInt;
        int varVal;

        if (funcExec) {
            if (lastFuncLines < functions[lastFuncKey].size()) {
                rtToken << functions[funcExecKey][lastFuncLines];

                if (functions[funcExecKey][lastFuncLines][functions[funcExecKey][lastFuncLines].size() - 1] != ';') {
                    exit_err("RUNTIME ERROR: Missing semicolen in function: " + lastFuncKey + "\n\nLINE: " + functions[funcExecKey][lastFuncLines]);
                }

                rtToken.setBuiltIn(builtInUsed);
                ++lastFuncLines;
            } else {
                funcExec = false;
                execEnd = true;
            }
        }

        std::string line = lines[_line];


        if (!(funcExec)) {
            rtToken << line;
            rtToken.setBuiltIn(builtInUsed);
        }

        std::string stdoutBuffer;
        std::string stdoutBakBuffer = "";

        bool openQuote = false;
        bool closedQuote = false;

        bool quote = false;

        if (c_block && lines[_line] == "C_START:") {
            c_block = true;
        }

        if (builtInUsed)

        if (possibleVar) {
            canPrintVar = true;
        }

        bool ignore = false;

        if (ifBlock && !(pauseIfRead)) {
            builtInUsed = IF_STATEMENT;
        }

        if (pauseIfRead && ifIsTrue) {
            if (ifExecBufferIt != ifExecBuffer.end()) {
                rtToken << *ifExecBufferIt;
                rtToken.setBuiltIn(builtInUsed);
                ++ifExecBufferIt;
            } else {
                ifExecBuffer.clear();
            }
        }

        switch (builtInUsed) {
            case OUT:
                if (!(readIfBlockCode)) {
                    if (lines[_line][4] == '"') {
                        possibleVar = false;
                    } else {
                        possibleVar = true;
                    }
                } else {
                    bool quote = false;

                    for (int i = 0; i < line.size(); ++i) {
                        if (line[i] == '"') {
                            quote = true;
                            break;
                        }
                    }

                    if (quote) {
                        possibleVar = false;
                    }

                    pauseIfRead = false;
                    ifBlock = true;
                }

                {
                    std::string parenContent;
                    rtToken ^ parenContent;
                    if (std::regex_match(parenContent, std::regex("(\"[a-zA-Z0-9]+\"{1}\\s*==\\s*\"{1}[A-Za-z0-9]+\"{1}|\"[A-Za-z0-9]+\"{1}\\s*!=\\s*\"{1}[A-Za-z0-9]+\"{1}|\\d+\\s*==\\s*\\d+|\\d+\\s*!=\\s*\\d+|\\d+\\s*>\\s*\\d+|\\d+\\s*<\\s*\\d+)"))) {
                        possibleVar = false;
                    }
                }

                if (!(funcExec)) {
                    if (std::regex_match(line, std::regex("out\\([a-zA-Z0-9]+\\);"))) {
                        possibleVar = true;
                    } else {
                        possibleVar = false;
                    }
                } else {
                    if (std::regex_match(curFuncLine, std::regex("out\\([a-zA-Z0-9]+\\);"))) {
                        possibleVar = true;
                    } else {
                        possibleVar = false;
                    }
                }

                if (!(possibleVar)) {
                    rtToken ^ stdoutBuffer;
                    std::string parsed = "";


                    if (std::regex_match(stdoutBuffer, std::regex("[a-zA-Z0-9]\\s*==\\s*[a-zA-Z0-9]"))) {
                        std::string val1 = "";
                        std::string val2 = "";

                        unsigned short int quotes = 0;

                        for (int i = 0; i < lines[_line].size(); ++i) {
                            if (lines[_line][i] == '"') {
                                ++quotes;
                            }
                        }

                        if (quotes > 4 || quotes < 4 && quotes > 0) {
                            exit_err("RUNTIME ERROR CODE 564: Syntax error on line " + std::to_string(internalLineNum));
                        }

                        std::smatch match;

                        std::regex_search(stdoutBuffer, match, std::regex("^[a-zA-Z0-9]+"));

                        for (auto i: match) {
                            val1 += i;
                        }

                        std::regex_search(stdoutBuffer, match, std::regex("[a-zA-Z0-9]+$"));

                        for (auto i: match) {
                            val2 += i;
                        }

                        if (val1 == val2) {
                            std::cout << "true" << std::endl;
                        } else {
                            std::cout << "false" << std::endl;
                        }
                    } else if (std::regex_match(lines[_line], std::regex("out\\(\"{1}[a-zA-Z!@#$%^&*\\-<>,.';:\\[\\]\\s]+\"{1}\\s*!=\\s*\"{1}[a-zA-Z!@#$%^&*\\-<>,.';:\\[\\]\\s]+\"\\)"))) {
                        std::string val1 = "";
                        std::string val2 = "";

                        unsigned short int quotes = 0;

                        for (int i = 0; i < lines[_line].size(); ++i) {
                            if (lines[_line][i] == '"') {
                                ++quotes;
                            }
                        }

                        if (quotes > 4 || quotes < 4 && quotes > 0) {
                            exit_err("RUNTIME ERROR CODE 600: Syntax error on line " + std::to_string(internalLineNum));
                        }

                        std::smatch match;

                        std::regex_search(stdoutBuffer, match, std::regex("^[a-zA-Z0-9]+"));

                        for (auto i: match) {
                            val1 += i;
                        }

                        std::regex_search(stdoutBuffer, match, std::regex("[a-zA-Z0-9]+$"));

                        for (auto i: match) {
                            val2 += i;
                        }

                        if (val1 != val2) {
                            std::cout << "true" << std::endl;
                        } else {
                            std::cout << "false" << std::endl;
                        }
                    } else if (std::regex_match(lines[_line], std::regex("out\\(\\d+\\s*>\\s*\\d+\\);"))) {
                        std::string val1 = "";
                        std::string val2 = "";

                        std::smatch match;

                        std::regex_search(stdoutBuffer, match, std::regex("^[a-zA-Z0-9]+"));

                        for (auto i: match) {
                            val1 += i;
                        }

                        std::regex_search(stdoutBuffer, match, std::regex("[a-zA-Z0-9]+$"));

                        for (auto i: match) {
                            val2 += i;
                        }

                        if (val1 > val2) {
                            std::cout << "true" << std::endl;
                        } else {
                            std::cout << "false" << std::endl;
                        }
                    } else if (std::regex_match(lines[_line], std::regex("out\\(\\d+\\s*<\\s*\\d+\\);"))) {
                        std::string val1 = "";
                        std::string val2 = "";

                        std::smatch match;

                        std::regex_search(stdoutBuffer, match, std::regex("^[a-zA-Z0-9]+"));

                        for (auto i: match) {
                            val1 += i;
                        }

                        std::regex_search(stdoutBuffer, match, std::regex("[a-zA-Z0-9]+$"));

                        for (auto i: match) {
                            val2 += i;
                        }

                        if (val1 < val2) {
                            std::cout << "true" << std::endl;
                        } else {
                            std::cout << "false" << std::endl;
                        }
                    } else if (std::regex_match(line, std::regex("out\\(\"?.*\"?\\s*\\+\\s*\"?.*.\"?\\);"))) {
                        std::string concatenated = "";

                        std::smatch m;

                        std::string::const_iterator searchStart(line.cbegin());

                        while (regex_search(searchStart, line.cend(), m, std::regex("[^;out()+\\s]+"))) {
                            for (auto i: m) {
                                std::string __i = i;
                                if (std::regex_match(__i, std::regex("\".*\""))) {
                                    std::smatch __m;
                                    std::regex_search(__i, __m, std::regex("[^\"]+"));

                                    for (auto i: __m) {
                                        __i = i;
                                    }

                                    concatenated += __i;
                                } else {
                                    if (!(stringVars.count(__i))) {
                                        std::string error = "RUNTIME ERROR: String var: ";
                                        error = error + "\"" + __i + "\" not found on line: " + std::to_string(internalLineNum);
                                        exit_err(error);
                                    } else {
                                        concatenated += stringVars[__i];
                                    }
                                }
                            }

                            searchStart = m.suffix().first;
                        }

                        std::cout << concatenated << std::endl;


                    } else {
                        std::cout << stdoutBuffer << std::endl;
                        possibleVar = false;
                    }
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
                    exit_err("RUNTIME ERROR: Either incrementing non-existing var or var is not int. Issue on line: " + std::to_string(internalLineNum));
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
                    exit_err("RUNTIME ERROR: Either incrementing non-existing var or var is not int. Issue on line: " + std::to_string(internalLineNum));
                }

                break;
            case IF_STATEMENT:
                {
                    if (!(ifBlock)) {
                        /*
                        * This section prepares
                        * the if statement.
                        */

                        ifExecBuffer.clear();
                        ifBlock = true;

                        std::string condition = "";

                        for (int i = 4; i < line.size() - 1 && line[i] != ')'; ++i) {
                            condition += line[i];
                        }

                        if (std::regex_match(condition, std::regex("[a-zA-Z]+[0-9A-ZA-Z]*\\s*==\\s*\".*\""))) {
                            unsigned short int quotes = 0;

                            for (int i = 0; i < condition.size(); ++i) {
                                if (condition[i] == '"') {
                                    ++quotes;
                                }
                            }

                            if (quotes > 2) {
                                exit_err("RUNTIME ERROR: Too many quotes on line: " + std::to_string(internalLineNum));
                            }

                            std::string varKey = "";

                            for (int i = 0; i < condition.size() && condition[i] != ' ' && condition[i] != '='; ++i) {
                                varKey +=  condition[i];
                            }


                            // WHEN MORE TYPES ADD ON HERE.
                            if (intVars.count(varKey)) {
                                exit_err("RUNTIME ERROR LINE " + std::to_string(internalLineNum) + ": Cannot compare an integer with a string.");
                            }

                            if (!(stringVars.count(varKey))) {
                                exit_err("RUNTIME ERROR: Trying to compare non-existing string var on line: " + std::to_string(internalLineNum));
                            } else {
                                std::smatch m;

                                std::regex_search(condition, m, std::regex("\".*\""));

                                std::string __i;

                                for (auto i: m) {
                                    __i = i;
                                    break;
                                }

                                __i = std::regex_replace(__i, std::regex("\""), "");

                                ifIsTrue = stringVars[varKey] == __i;
                            }
                        } else if (std::regex_match(condition, std::regex("\"*.+\"\\s*==\\s*\\w+\\d*"))) {
                            exit_err("RUNTIME ERROR LINE " + std::to_string(internalLineNum) + " Sorry but first of all comparing like that is weird, second of all that doesn't work yet.");
                        }

                    } else {
                        unsigned short int indentLevel = 0;

                        for (int i = 0; i < line.size() && line[i] == ' '; ++i) {
                            ++indentLevel;
                        }

                        if (line[0] != '}') {
                            ifExecBuffer.push_back(line.substr(indentLevel, line.size() - 1));
                        } else {
                            ifBlock = false;
                            ifExecBufferIt = ifExecBuffer.begin();
                            pauseIfRead = true;
                        }
                    }
                }

                break;
            case VAR_DUMP:
                {
                    std::string vars = "";

                    typedef std::map<std::string, short int> intVar;
                    typedef std::map<std::string, std::string> stringVar;

                    for (intVar::const_iterator it = intVars.begin(); it != intVars.end(); ++it) {
                        std::string kv = "INT_VARS[";
                        kv += it -> first;
                        kv += "] = ";
                        kv += std::to_string(it -> second);
                        vars += kv;
                        vars += "\n\n";
                    }

                    for (stringVar::const_iterator it = stringVars.begin(); it != stringVars.end(); ++it) {
                        std::string kv = "STRING_VARS[";
                        kv += it -> first;
                        kv += "] = ";
                        kv += it -> second;
                        vars += kv;
                        vars += "\n\n";
                    }

                    std::cout << vars << std::endl;
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
                    exit_err("RUNTIME ERROR: Either decrementing non-existing var or var is not int. Issue on line: " + std::to_string(internalLineNum));
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
                    exit_err("RUNTIME ERROR: Either decrementing non-existing var or var is not int. Issue on line: " + std::to_string(internalLineNum));
                }

                break;
            case FUNCTION_DEF:
                {

                    if (!(functionActive)) {
                        functionActive = true;
                        std::string functionKey = "";
                        lastFuncKey = "";


                        std::smatch m;

                        std::regex_search(line, m, std::regex("\\s[a-zA-Z][A-Za-z0-9]*"));

                        for (auto i: m) {
                            functionKey = i;
                            break;
                        }

                        for (int i = 0; i < functionKey.size(); ++i) {
                            if (functionKey[i] != ' ') {
                                lastFuncKey += functionKey[i];
                            }
                        }

                        if (functions.count(lastFuncKey)) {
                            exit_err("RUNTIME ERROR: \"" + lastFuncKey + "\" defined again on line: " + std::to_string(internalLineNum));
                        }

                        functionActive = true;
                        functions[lastFuncKey] = {};
                    } else {
                        std::string _line = "";

                        bool restrictSpaces = true;

                        for (int i = 0; i < line.size(); ++i) {
                            if (line[i] != ' ') {
                                restrictSpaces = false;
                            }

                            if (!(restrictSpaces)) {
                                _line += line[i];
                            }
                        }

                        if (_line != "}") {
                            functions[lastFuncKey].push_back(_line);
                            curFuncLine = _line;
                        } else {
                            functionActive = false;
                        }
                    }
                }
                break;
            case FUNCTION_CALL:
                {
                    std::smatch m;

                    std::regex_search(line, m, std::regex("^[a-zA-Z][A-Za-z0-9]*"));

                    // lines.insert(lines.begin() + internalLineNum, "______GAP______;");  // Fixes bug that ignores lines after function call.

                    for (auto i: m) {
                        funcExecKey = i;
                        break;
                    }

                    if (!(functions.count(funcExecKey))) {
                        exit_err("RUNTIME ERROR LINE " + std::to_string(internalLineNum) + ": Function \"" + funcExecKey + "\" does not exist.");
                    } else {
                        lastFuncLines = 0;
                        funcExec = true;
                    }

                }
                break;
            case RE_ASSIGN:
                {
                    std::string varKey = "";

                    for (int i = 0; line[i] != '=' && line[i] != ' ' && i < line.size(); ++i) {
                        varKey += line[i];
                    }

                    if (!(stringVars.count(varKey)) && !(intVars.count(varKey))) {
                        exit_err("RUNTIME ERROR: Trying to re-assign non-existing var on line: " + std::to_string(internalLineNum));
                    } else if (intVars.count(varKey)) {
                        std::smatch m;
                        std::string lineCopy = std::regex_replace(line, std::regex(";"), "");
                        std::regex_search(lineCopy, m, std::regex("\\d+$"));

                        std::string __i;
                        short int newVal;

                        for (auto i: m) {
                            __i = i;
                            break;
                        }

                        if (!(std::regex_match(__i, std::regex("\\d+")))) {
                            exit_err("RUNTIME ERROR: Re-assigning with invalid type on line: " + std::to_string(internalLineNum));
                        }

                        std::stringstream ss(__i);
                        ss >> newVal;
                        intVars[varKey] = newVal;
                    } else if (stringVars.count(varKey)) {
                        unsigned short int quotes = 0;

                        for (int i = 0; i < line.size(); ++i) {
                            if (line[i] == '"') {
                                ++quotes;
                            }
                        }

                        if (quotes > 2) {
                            exit_err("RUNTIME ERROR: Syntax error on line: " + std::to_string(internalLineNum));
                        } else if (quotes < 2 && quotes != 0) {
                            exit_err("RUNTIME ERROR: Missing quotes on line: " + std::to_string(internalLineNum));
                        } else if (quotes == 0) {
                            exit_err("RUNTIME ERROR: Re-assigning with invalid type on line: " + std::to_string(internalLineNum));
                        }

                        std::smatch m;
                        std::regex_search(line, m, std::regex("\".*\""));

                        for (auto i: m) {
                            stringVars[varKey] = i;
                            break;
                        }

                        stringVars[varKey] = std::regex_replace(stringVars[varKey], std::regex("\""), "");
                    }
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
                    std::string inputVarVal = "";
                    bool setVarDefault = true;
                    bool quotes = false;


                    if (!(funcExec)) {
                        for (int i = 4; i < line.size() - 1; ++i) {
                            if (line[i] != ' ') {
                                varKey += line[i];
                            } else {
                                for (int j = i + 3; j < line.size() - 1; ++j) {
                                    if (line[j] == '"') {
                                        quotes = true;
                                    }

                                    if (line[j] != '"') {
                                        varVal += line[j];

                                        if (varVal == "in" && !(quotes)) {
                                            setVarDefault = false;

                                            bool openParen = false;
                                            bool closedParen = false;
                                            unsigned short int quoteCount = 0;

                                            for (int e = j + 1; e < line.size() - 1; ++e) {
                                                if (line[e] == '(') {
                                                    if (openParen) {
                                                        exit_err("RUNTIME ERROR: Too many parenthesis on line: " + std::to_string(internalLineNum));
                                                    }

                                                    openParen = true;
                                                } else if (line[e] == '"') {
                                                    if (quoteCount > 2) {
                                                        exit_err("RUNTIME ERROR: Too many quotes on line: " + std::to_string(internalLineNum));
                                                    }

                                                    ++quoteCount;
                                                } else if (line[e] == ')') {
                                                    if (closedParen) {
                                                        exit_err("RUNTIME ERROR: Too many parenthesis on line: " + std::to_string(internalLineNum));
                                                    }

                                                    closedParen = true;
                                                } else {
                                                    inputVarVal += line[e];
                                                }
                                            }

                                            if (quoteCount == 0) {
                                                exit_err("RUNTIME ERROR: Variables as input title is not supported currently, error on line: " + std::to_string(internalLineNum));
                                            } else if (openParen && !(closedParen) || !(openParen) && closedParen) {
                                                exit_err("RUNTIME ERROR: Missing parenthesis on line " + std::to_string(internalLineNum));
                                            }
                                        }
                                    }
                                }

                                break;
                            }
                        }

                    } else {
                        for (int i = 4; i < curFuncLine.size() - 1; ++i) {
                            if (curFuncLine[i] != ' ') {
                                varKey += curFuncLine[i];
                            } else {
                                for (int j = i + 3; j < curFuncLine.size() - 1; ++j) {
                                    if (curFuncLine[j] == '"') {
                                        quotes = true;
                                    }

                                    if (curFuncLine[j] != '"') {
                                        varVal += curFuncLine[j];

                                        if (varVal == "in" && !(quotes)) {
                                            setVarDefault = false;

                                            bool openParen = false;
                                            bool closedParen = false;
                                            unsigned short int quoteCount = 0;

                                            for (int e = j + 1; e < curFuncLine.size() - 1; ++e) {
                                                if (curFuncLine[e] == '(') {
                                                    if (openParen) {
                                                        exit_err("RUNTIME ERROR: Too many parenthesis on line: " + std::to_string(internalLineNum));
                                                    }

                                                    openParen = true;
                                                } else if (curFuncLine[e] == '"') {
                                                    if (quoteCount > 2) {
                                                        exit_err("RUNTIME ERROR: Too many quotes on line: " + std::to_string(internalLineNum));
                                                    }

                                                    ++quoteCount;
                                                } else if (curFuncLine[e] == ')') {
                                                    if (closedParen) {
                                                        exit_err("RUNTIME ERROR: Too many parenthesis on line: " + std::to_string(internalLineNum));
                                                    }

                                                    closedParen = true;
                                                } else {
                                                    inputVarVal += curFuncLine[e];
                                                }
                                            }

                                            if (quoteCount == 0) {
                                                exit_err("RUNTIME ERROR: Variables as input title is not supported currently, error on line: " + std::to_string(internalLineNum));
                                            } else if (openParen && !(closedParen) || !(openParen) && closedParen) {
                                                exit_err("RUNTIME ERROR: Missing parenthesis on line " + std::to_string(internalLineNum));
                                            }
                                        }
                                    }
                                }

                                break;
                            }
                        }

                    }


                    if (stringVars.count(varKey) || intVars.count(varKey)) {
                        exit_err("RUNTIME ERROR: Var re-declared or var already exists as another type on line: " + std::to_string(internalLineNum));
                    }

                    if (setVarDefault) {
                        stringVars[varKey] = varVal;
                    } else {
                        std::cout << inputVarVal;
                        std::cin >> stringVars[varKey];
                    }
                }

                break;
            case C_CODE:
                outFile.close();
                if (!(c_block)) {
                    c_block = true;
                    for (int i = 0; i < cb_files.size(); ++i) {
                        std::string command = "mv ";
                        command += cb_files[i];
                        command += " .kesslangC.c";
                        system(command.c_str());
                        system("gcc .kesslangC.c -o .kesslangC");

                        std::string _exec = std::to_string(system("./.kesslangC"));
                        _exec = std::regex_replace(_exec, std::regex("\\d$"), "");
                        std::cout << _exec << std::endl;
                        system("rm .kesslangC .kesslangC.c");
                    }
                }

                break;
            case STOP:
                exit(0);
                break;
            case STRING_ADDON:
                {
                    std::string toBeParsed;
                    std::string parsed = "";
                    std::string key;
                    bool quote = false;

                    std::smatch m;

                    std::regex_search(line, m, std::regex("(\"[^\"]+\"|\\s+[A-Za-z]+)"));

                    for (auto i: m) {
                        toBeParsed = i;
                        break;
                    }

                    std::regex_search(line, m, std::regex("^[a-zA-Z0-9]+"));

                    for (auto i: m) {
                        key = i;
                        break;
                    }

                    for (int i = 0; i < toBeParsed.size() && toBeParsed[i] != ';'; ++i) {
                        if (toBeParsed[i] == '"') {
                            quote = true;
                        }

                        if (toBeParsed[i] != '"') {
                            parsed += toBeParsed[i];
                        }
                    }

                    if (quote) {
                        if (!(stringVars.count(key))) {
                            exit_err("RUNTIME ERROR: Trying to add-on to non-existing string var on line: " + std::to_string(internalLineNum));
                        } else {
                            stringVars[key] = stringVars[key] + parsed;
                        }
                    } else if (!(quote) && stringVars.count(parsed)) {
                        if (!(stringVars.count(key))) {
                            exit_err("RUNTIME ERROR: Trying to add-on to non-existing string var on line: " + std::to_string(internalLineNum));
                        } else {
                            stringVars[key] = stringVars[key] + stringVars[parsed];
                        }
                    } else {
                        exit_err("RUNTIME ERROR LINE " + std::to_string(internalLineNum) + ": No variable named " + parsed);
                    }
                }

                break;
        }
    }

    outFile.close();
}


// END OF CORE FILE.
