#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <iostream>
#include "sha256.hpp"

struct Block_info {

    std::string From;
    std::string To;
    double Amount;
    int timestamp;
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

    private:

        int head = 0;
        int length = 0;
        int timestamp;
        int Nonce;
        std::string Sign;
        int Diff = 2;
        Block_info data = {};
        std::string parent;
        std::string hash;
};


#endif // __BLOCK_H__