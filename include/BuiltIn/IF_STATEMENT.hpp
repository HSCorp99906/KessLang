#ifndef IF_STATEMENT_HPP
#define IF_STATEMENT_HPP

#include <vector>
#include <string>


class IfStatement {
    private:
        std::vector<std::string> lines;

    public:
        void push_line(const char* line);

        bool conditionIsTrue();

        void execute();
};


#endif
