#include <iostream>
#include "Block.hpp"

Block::Block(int head,std::string parent,Block_info transaction) {
    this->length = sizeof(transaction);
    this->head = head;
    this->data = transaction;
    this->timestamp = time(NULL);
    this->parent = parent;
    this->hash = sha256(std::to_string(head) + parent);
}

Block::~Block() {
    // TODO exit
}

bool Block::Mining(std::string public_key) {

    int nonce = rand() % 0x3B9ACA00;
    std::string input = this->data.From + this->data.To + std::to_string(this->data.Amount) + std::to_string(this->data.timestamp) + this->parent;

    std::string s = sha256(public_key + input + std::to_string(nonce));


    std::string difficult = "";
    int pos;

    for(int i = 1; i <= this->Diff; i++) {
        difficult += "0";
        pos = i;
    }

    std::printf("\r[ MiNING ] diff=%i timestamp=%i Nonce=%i Search=%s Hash=%s",this->Diff,this->get_Timestamp(),nonce,difficult.c_str(),s.substr(0,8).c_str());
    if(s.substr(0,pos) == difficult) {
        this->Nonce = nonce;
        this->Sign = s;

        std::printf("New block detected\n");
        return true;

    }

    return false;
}

bool Block::Signed(std::string public_key,std::string private_key) {

    if("0X"+sha256(private_key.substr(0,14)).substr(0,43) == public_key) {
        return true;
    }
    return false;
}

int Block::get_Nonce() const {
    return this->Nonce;
}

int Block::get_Lenght() const {
    return this->length;
}

int Block::get_Timestamp() const {
    return this->timestamp;
}

std::string Block::get_Parent() const {
    return this->parent;
}

std::string Block::get_Signed() const {
    return this->Sign;
}

std::string Block::get_Hash() const {
    return this->hash;
}

int Block::get_head() const {
    return this->head;
}