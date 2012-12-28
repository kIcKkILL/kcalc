#ifndef CCALCORE_H
#define CCALCORE_H
#include <string>
#include <sstream>

const double kPi = 3.141592653;
const std::string kPi_s = "3.141592653";

std::string doubleToString(double in);

class Token{
public:
    enum token_type{null,power,equal,number,plus,minus,multiply,mod,division,factorial,left,right};
    token_type kind;
    double num;
    Token(token_type t,double n = 0.0):kind(t),num(n){}
private:
    Token(); // Exhibit no-reg construction
};

class cCore{
private:
    // define class Token_stream to buffer token
    class Token_stream{
    private:
        std::istringstream stream;
        bool full;
        Token buffer;
        Token_stream(); // Exhibit construction without istream bound
    public:
        Token_stream(const std::string &str):stream(str),full(false),buffer(Token::null){}
        Token get();
        void ignore();
        void putback(Token t);
    };
    // Token_stream defined within cCore
private:
    Token_stream ts;
    double expression();
    double term();
    double primary();
    double pre_primary(); // to calculate factorial
    void clean_up_mess(){
        ts.ignore();
    }
    cCore(); // Exhibit no-reg construction
public:
    cCore(const std::string &is);
    double calculate();
};
#endif
