#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <iostream>
#include "sha256.hpp"
#include "Function.hpp"
#include <jsoncpp/json/json.h>

struct Block_info {

    std::string From;
    std::string To;
    double Amount;
};

class Block {

    public:

        Block(int head,std::string parent,Block_info transaction);
        ~Block();
        bool Signed(std::string public_key,std::string private_key);
        int get_Timestamp() const;
        int get_Lenght() const;
        int get_Nonce() const;
        bool Mining(std::string public_key);
        std::string get_Parent() const;
        std::string get_Signed() const;
        std::string get_Hash() const;
        int get_head() const;
        Block_info get_Data() const;
        int get_Diff() const;
        void set_Diff(int dif);
        Json::Value get_Transaction() const;

    private:

        int head = 0;
        int length = 0;
        int timestamp;
        int Nonce;
        std::string Sign;
        int Diff = 1;
        Block_info data = {};
        std::string parent;
        std::string hash;
};


#endif // __BLOCK_H__