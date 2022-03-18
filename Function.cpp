#include "Function.hpp"

Func::Func() {}
Func::~Func() {}

void Func::Console_Log(std::string msg,type_msg type) {

    std::string banner;
    
    switch(type) {
        case 0: {
            banner += "[ \033[34;1m I \033[0m ] ";
            break;
        }
        case 1: {
            banner += "[ \033[33;1m W \033[0m ] ";
            break;
        }
        case 2: {
            banner += "[ \033[31;1m E \033[0m ] ";
            break;
        }
        case 3: {
            banner += "[ \033[32;1m + \033[0m ] ";
            break;
        }
        case 4: {
            banner += "[ \033[31;1m - \033[0m ] ";
            break;
        }
        case 5: {
            banner += "[ \033[36;1m ? \033[0m ] ";
            break;
        }

    }
    banner += msg;

    if(type != type_msg::ask) {
        std::cout << banner << std::endl;
    }
    else {
        std::cout << banner << " :";
    }
}