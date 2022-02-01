#include <iostream>
#include "sha256.hpp"
#include "Block.hpp"
#include "Account.hpp"
#include <vector>
#include <thread>
#include <dirent.h>
#include <jsoncpp/json/json.h>
#include <fstream>

using namespace std;

vector<Block*> Blockchain;
Account account;

enum type_msg {
    info,
    alert,
    error
};

void Console_Log(string msg,type_msg type) {
    string banner;
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
    }
    banner += msg;
    cout << banner << endl;
}

void Mining(Block* b) {
    while(!b->Mining(account.get_Address())){}
}

int main() {

    Block_info info_block;
    info_block.Amount = 0;
    info_block.From = "";
    info_block.To = "";

    Block *genesis = new Block(0,"0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",info_block);
    Blockchain.push_back(genesis);
    Console_Log("Creation de la Genesis",info);

    //? TEST --------------------------------------------------------------------
    for(int i = 1; i < 10;i++) {
        info_block.Amount = rand()%1000;
        info_block.From = account.get_Address();
        info_block.To = "0X"+sha256(account.get_Address()).substr(0,42);
        int size = Blockchain.size();
        Blockchain.push_back(new Block(size,Blockchain[i-1]->get_Hash(),info_block));
    }

    for(int i = 0; i < Blockchain.size();i++) {
        if(Blockchain[i]->get_Signed() == "") {
            thread _mining(Mining,Blockchain[i]);
            _mining.join();
        }
    }

    //? END TEST --------------------------------------------------------------------

    Json::Value t;
    string num;
    for(int i = 0; i < Blockchain.size(); i++) {
        num = "Block #"+ to_string(i);
        t[num]["Hash"] = Blockchain[i]->get_Hash();
        t[num]["Parent"] = Blockchain[i]->get_Parent();
        t[num]["Head"] = Blockchain[i]->get_head();
        t[num]["Lenght"] = Blockchain[i]->get_Lenght();
        t[num]["Timestamp"] = Blockchain[i]->get_Timestamp();
        if(Blockchain[i]->get_Signed() == "") {
            t[num]["transaction_id"] = "Unknow";
        }
        else {
            t[num]["transaction_id"] = Blockchain[i]->get_Signed();
        }
        t[num]["Data"]["Amount"] = Blockchain[i]->get_Data().Amount;
        t[num]["Data"]["From"] = Blockchain[i]->get_Data().From;
        t[num]["Data"]["To"] = Blockchain[i]->get_Data().To;
    }

    cout << t << endl;


    return 0;
}