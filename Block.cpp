#include <iostream>
#include <cmath>
#include "Block.hpp"
#include <ctime>

Func func;

Block::Block() {}

Block::Block(int head,std::string parent,Block_info transaction) {
    this->length = sizeof(transaction.Amount) +transaction.From.size() + transaction.To.size();
    this->head = head;
    this->data = transaction;
    this->timestamp = time(NULL);
    this->parent = parent;
    this->hash = sha256(std::to_string(head) + parent + std::to_string(this->timestamp));
}

Block::~Block() {
    func.Console_Log("Exit Block.",func.type_msg::info);
}

bool Block::Mining(std::string public_key) {

    int nonce = rand() % 0x3B9ACA00;
    std::string input = this->data.From + this->data.To + std::to_string(this->data.Amount) + this->parent;

    std::string s = sha256(public_key + input + std::to_string(nonce));


    std::string difficult = "";
    int pos;

    for(int i = 1; i <= this->Diff; i++) {
        difficult += "1";
        pos = i;
    }

    // std::printf("\r⛏️\tHead=%i\tdiff=%i\ttimestamp=%i\tNonce=%i\tHash=%s",this->head,this->Diff,this->get_Timestamp(),nonce,s.substr(0,8).c_str());
    if(s.substr(0,pos) == difficult) {
        this->Nonce = nonce;
        this->Sign = s;

        std::cout << std::endl;
        func.Console_Log("Transaction Certified.",func.type_msg::info);
        
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

Block_info Block::get_Data() const {
    return this->data;
}

int Block::get_Diff() const {
    return this->Diff;
}

void Block::set_Diff(int dif) {
    this->Diff = dif;
}

Json::Value Block::get_Transaction() const {
    Json::Value js;

    js["Head"] = this->get_head();
    js["Length"] = this->get_Lenght();
    js["Data"]["To"] = this->get_Data().To;
    js["Data"]["From"] = this->get_Data().From;
    js["Data"]["Amount"] = this->get_Data().Amount;
    js["Hash"] = this->get_Hash();
    js["Timestamp"] = this->get_Timestamp();
    js["Nonce"] = this->get_Nonce();
    js["Sign"] = this->get_Signed();
    js["Parent"] = this->get_Parent();
    js["Diff"] = this->get_Diff();
    
    return js;
}

void Block::set_Transaction(Json::Value js) {

    this->head = js["Head"].asInt();
    this->length = js["Length"].asInt();
    this->data.To = js["Data"]["To"].asString();
    this->data.From = js["Data"]["From"].asString();
    this->data.Amount = js["Data"]["Amount"].asDouble();
    this->hash = js["Hash"].asString();
    this->timestamp = js["Timestamp"].asInt();
    this->Nonce = js["Nonce"].asInt();
    this->Sign = js["Sign"].asString();
    this->parent = js["Parent"].asString();
    this->Diff = js["Diff"].asInt();
}