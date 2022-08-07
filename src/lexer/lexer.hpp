#include "../token.h"

struct Lexer{
    Lexer(const std::string& in) 
        : input(in), 
          pos(0),
          readPos(1),
          ch(input[pos]),
          line(1)
    {}

    void readChar(){
        if(readPos >= input.size())
            ch = 0;
        else
            ch = input[readPos];
        pos++;
        readPos++;
    }

    char peekChar(){
        if(readPos >= input.size())
            return 0;
        return input[readPos];
    }

    void skipWriteSpace(){
        for(; ch=='\t' || ch==' ' || ch=='\r' || ch=='\n'; ){
            if(ch == '\n')
                line++;
            readChar();
        }
    }

    Token nextToken();

    std::string input;
    int pos;
    int readPos;
    char ch;
    int line;  //记录行数，方便报错
};

bool isLetter(char c){
    return c>='a'&&c<='z' || c>='A'&&c<='Z' || c=='_';
}

bool isDigit(char c){
    return c>='0' && c<='9';
}

std::string readIdentifier(Lexer& lex){
    int pos = lex.pos;
    for(; isLetter(lex.ch); )
        lex.readChar();
    return lex.input.substr(pos, lex.pos-pos);
}

std::string readNumber(Lexer& lex, bool& isINT){
    int pos = lex.pos;
    for(; isDigit(lex.ch); )
        lex.readChar();
    if(lex.ch == '.'){
        lex.readChar();
        isINT = false;
        for(; isDigit(lex.ch); )
            lex.readChar();
    }
    return lex.input.substr(pos, lex.pos-pos);
}

std::string readString(Lexer& lex){
    int pos = lex.pos + 1;
    for(;;){
        lex.readChar();
        if(lex.ch == '"' || lex.ch == 0)
            break;
    }
    return lex.input.substr(pos, lex.pos-pos);
}

Token Lexer::nextToken(){
    Token tok;
    this->skipWriteSpace();
    switch(this->ch){
        case '=':
            if(this->peekChar() == '='){
                tok.TokenType = EQUAL;
                tok.Literal = "==";
                this->readChar();
            }else{
                tok.TokenType = ASSIGN;
                tok.Literal = "=";
            }
            break;
        case ';':
            tok.TokenType = SEMICOLON;
            tok.Literal = ";";
            break;
        case '(':
            tok.TokenType = LPAREN;
            tok.Literal = "(";
            break;
        case ')':
            tok.TokenType = RPAREN;
            tok.Literal = ")";
            break;
        case '{':
            tok.TokenType = LBRACE;
            tok.Literal = "{";
            break;
        case '}':
            tok.TokenType = RBRACE;
            tok.Literal = "}";
            break;
        case ',':
            tok.TokenType = COMMA;
            tok.Literal = ",";
            break;
        case '+':
            tok.TokenType = PLUS;
            tok.Literal = "+";
            break;
        case '-':
            tok.TokenType = MINUS;
            tok.Literal = "-";
            break;
        case '*':
            tok.TokenType = ASTERISK;
            tok.Literal = "*";
            break;
        case '/':
            tok.TokenType = SLASH;
            tok.Literal = "/";
            break;
        case '<':
            tok.TokenType = LT;
            tok.Literal = "<";
            break;
        case '>':
            tok.TokenType = GT;
            tok.Literal = ">";
            break;
        case '!':
            if(this->peekChar() == '='){
                tok.TokenType = NOT_EQUAL;
                tok.Literal = "!=";
                this->readChar();
            }else{
                tok.TokenType = BANG;
                tok.Literal = "!";
            }
            break;
        case '"':
            tok.TokenType = STRING;
            tok.Literal = readString(*this);
            break;
        case '[':
            tok.TokenType = LBRACKET;
            tok.Literal = "[";
            break;
        case ']':
            tok.TokenType = RBRACKET;
            tok.Literal = "]";
            break;
        case ':':
            tok.TokenType = COLON;
            tok.Literal = ":";
            break;
        case 0:
            tok.TokenType = END;
            break;
        default:
            //判断是否为标识符
        if(isLetter(this->ch)){
            std::string ident = readIdentifier(*this);
            tok.TokenType = lookUpIdent(ident);
            tok.Literal = ident;
            return tok;
        }else if(isDigit(this->ch)){
            bool isINT = true;
            std::string ident = readNumber(*this, isINT);
            if(isINT){
                tok.TokenType = INT;
                tok.Literal = ident;
            }else{
                tok.TokenType = DOUBLE;
                tok.Literal = ident;
            }
            return tok;
        }else{
            tok.TokenType = ILLEGAL;
            tok.Literal = std::to_string(this->ch);
        }
    }
    this->readChar();    
    return tok;
}
