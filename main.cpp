#include <iostream>
#include "sha256.hpp"
#include "Block.hpp"
#include "Account.hpp"
#include <vector>
#include <dirent.h>
#include <fstream>
#include <cmath>
#include <iomanip>
#include "Base64.hpp"

using namespace std;

Func function;

vector<Block*> Blockchain;
Account account;

#include <thread>
#include <jsoncpp/json/json.h>

void Mining(Block* b) {
    int start = time(NULL);
    int end;
    bool s;
    
    while(!(s = b->Mining(account.get_Address()))){}

    end = time(NULL);

    int pos = b->get_head() + 1;

    if(s) {
        Block_info info_block;
        info_block.Amount = b->get_Diff()+(end-start);
        info_block.From = "Reward";
        info_block.To = account.get_Address();
        int size = Blockchain.size();
        Blockchain.push_back(new Block(size,Blockchain[size-1]->get_Hash(),info_block));
        
        account.Balance(b);
        function.Console_Log("Balance : "+to_string(account.get_Balance()),function.type_msg::info);
    }

    if((end - start) >= 10) {
        if(pos < Blockchain.size()) {
            Blockchain[pos]->set_Diff(b->get_Diff() - 1);
        }
    }
    else if((end - start) <= 8) {
        if(pos < Blockchain.size()) {
            Blockchain[pos]->set_Diff(b->get_Diff() + 1);
        }
    }
}




int main() {


    Block_info info_block;
    info_block.Amount = 0x174876E800;
    info_block.From = "";
    info_block.To = "0X39216175d066d5ca660308ca0ee5f25cc8ee91fd7f0";

    function.Console_Log("Creation de la Genesis",function.type_msg::info);

    Block *genesis = new Block(0,"f9261c5b0f73ef5b3d0151ecdc28ac67c17e9cfbe45e63bd1f1e244a21f18779",info_block);
    Blockchain.push_back(genesis);

    function.Console_Log("Balance : "+to_string(account.get_Balance()),function.type_msg::info);
    function.Console_Log("-Address : "+account.get_Address(),function.type_msg::info);
    function.Console_Log("-Account : "+account.get_Account(),function.type_msg::info);


    //? TEST --------------------------------------------------------------------

    if(account.mining == true) {

        for(int i = 0; i < Blockchain.size();i++) {
            if(Blockchain[i]->get_Signed() == "") {
                if(Blockchain[i]->Signed(account.get_Address(),account.get_Account())) {
                    thread *_mining = new thread(Mining,Blockchain[i]);
                    _mining->join();
                }
            }
        }
    }

    info_block.Amount = 10001;
    info_block.From = "0X39216175d066d5ca660308ca0ee5f25cc8ee91fd7f0";
    info_block.To = "0X39216175d06785ca660308ca0ee5f25cc8ee91fd7f0";
    Blockchain.push_back(new Block(Blockchain.size(),Blockchain[Blockchain.size()-1]->get_Hash(),info_block));


    for(int i = 0; i < Blockchain.size();i++) {
        function.Console_Log("Balance : "+to_string(account.Balance(Blockchain[i])),function.type_msg::info);
    }
    //? END TEST --------------------------------------------------------------------

    Json::Value t;

    for(int i = 0; i < Blockchain.size(); i++) {
        t["Tx"][i] = Blockchain[i]->get_Transaction();
    }

    cout << t << endl;

    ofstream fs;
    fs.open("dev/test_tx",ios::binary);
    string save = base64_encode_pem(t.toStyledString());
    fs << save << endl;
    fs.close();


    return 0;
}