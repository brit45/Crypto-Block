#include <iostream>

using namespace std;

class Func {
    public:
        enum type_msg {
            info,
            alert,
            error,
            add,
            remove,
            ask
        };
        Func();
        ~Func();
        void Console_Log(std::string msg,type_msg type);
};