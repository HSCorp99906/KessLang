#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <vector>


class Function {
    private:
        std::vector<std::string> lines;

    public:
        void execute();

        void push_line(const char* line);
};


#endif
