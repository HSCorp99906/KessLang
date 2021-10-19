#ifndef TOKEN_HPP
#define TOKEN_HPP


class Token {
    private:
        std::string line;

    public:
        void operator<<(std::string input);

        void operator>>(std::string& output);

        void operator^(std::string& output);  // Gets paren content.

        void setBuiltIn(unsigned int& builtInUsed);
};


#endif
