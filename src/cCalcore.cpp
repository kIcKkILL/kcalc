#include "cCalcore.h"
#include <cstdlib>
#include <cctype>
#include <stdexcept>
#include <cmath>

using namespace std;

string doubleToString(double in) {
  stringstream convert;
  convert << in;
  string out;
  convert >> out;
  return out;
}

double cCore::calculate(){
    try{
        return expression();
    }catch(const runtime_error &e){
        clean_up_mess();
        throw e;
    }
}

void cCore::Token_stream::ignore(){
    if(full && buffer.kind == Token::equal){
        full = false;
        return;
    }
    full = false;
    char ch;
    while(stream >> ch)
        if(ch == '=')
            return;
}

cCore::cCore(const string &is):ts(is){ }

double cCore::expression(){
    double left = term();
    Token t = ts.get();
    while(true){
        switch(t.kind){
            case Token::plus:
                left += term();
                t = ts.get();
                break;
            case Token::minus:
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}

double cCore::term(){
    double left = pre_primary();
    Token t = ts.get();
    while(true){
        switch(t.kind){
            case Token::multiply:
                left *= pre_primary();
                t = ts.get();
                break;
            case Token::division: {
                double d = pre_primary();
                if (d==0)
                    throw runtime_error("Divided by zero!");
                left /= d;
                t = ts.get();
                break;
            }
            case Token::mod: {
                double d = pre_primary();
                int i1 = static_cast<int>(left);
                if( i1 != left)
                    throw runtime_error("Left-hand operand of % is not an integer");
                int i2 = static_cast<int>(d);
                if(i2 != d)
                    throw runtime_error("Right-hand operand of % is not an integer");
                if( i2 == 0 )
                    throw runtime_error("Divided by zero!");
                left = i1%i2;
                t = ts.get();
                break;
           }
           default:
               ts.putback(t);
               return left;
        }
    }
}

double cCore::pre_primary(){
    double left = primary();
    Token t = ts.get();
    switch(t.kind){
        case Token::factorial: {
            int i = static_cast<int>(left);
            if( i != left || left < 0)
                throw runtime_error("Operand of ! must be an non-negative integer!");
            if( i == 0 )
                left = 1;
            else
                for(int j = i - 1; j > 1; --j)
                    left *= j;
            return left;
        }
        case Token::power:
            return pow(left,primary());
        default:
            ts.putback(t);
            return left;
    }
}

double cCore::primary(){
    Token t = ts.get();
    switch(t.kind){
        case Token::left: {
            double d = expression();
            t = ts.get();
            if (t.kind != Token::right)
                throw runtime_error("Illegal expression!");
            return d;
        }
        case Token::number:
            return t.num;
        case Token::minus:
            return -primary();
        case Token::plus:
            return primary();
        default:
            throw runtime_error("Invalid expression!");
    }
}
Token cCore::Token_stream::get(){
    if( full ){
        full = false;
        return buffer;
    }
    char ch;
    stream >> ch;
    if(isdigit(ch) || ch == '.') {
        stream.putback(ch);
        double val;
        stream >> val;
        return Token(Token::number,val);
    }
    else {
        switch(ch){
            case '(':
                return Token(Token::left);
            case ')':
                return Token(Token::right);
            case '+':
                return Token(Token::plus);
            case '-':
                return Token(Token::minus);
            case '*':
                return Token(Token::multiply);
            case '/':
                return Token(Token::division);
            case '=':
                return Token(Token::equal);
            case '!':
                return Token(Token::factorial);
            case '%':
                return Token(Token::mod);
            case '^':
                return Token(Token::power);
            case 'q':
                exit(0);
            default:
                throw std::runtime_error("Invalid expression!");
        }
    }
}

void cCore::Token_stream::putback(Token t){
    if( full )
        throw std::logic_error("Putback() into a full buffer");
    buffer = t;
    full = true;
}
