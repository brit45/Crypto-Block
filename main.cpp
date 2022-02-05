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
    
    while(!(s = b->Mining(account.get_Address()) && b->get_Signed() != "")){}

    end = time(NULL);

    std::string input = b->get_Data().From + b->get_Data().To + std::to_string(b->get_Data().Amount) + b->get_Parent();

    std::string de = sha256(account.get_Address() + input + std::to_string(b->get_Nonce()));

    if(s){

        int pos = b->get_head() + 1;

        if(de == b->get_Signed()) {

            Block_info info_block;
            info_block.Amount = b->get_Diff()+(end-start);
            info_block.From = "Reward";
            info_block.To = account.get_Address();
            int size = Blockchain.size();
            Blockchain.push_back(new Block(size,Blockchain[size-1]->get_Hash(),info_block));
            account.Balance(b);
        }
        
        if((end - start) >= 10) {
            if(pos < Blockchain.size()) {
                if(b->get_Diff() > 0) {
                    Blockchain[pos]->set_Diff(b->get_Diff() - 1); 
                }
            }
        }
        else if((end - start) <= 9) {
            if(pos < Blockchain.size()) {
                if(b->get_Diff() < 64) {
                    Blockchain[pos]->set_Diff(b->get_Diff() + 1);
                }
            }
        }
    }

}

bool run = true;

void quit() {

    string r;
    cout << "Quit (n) : ";
    getline(cin,r);
    cin.ignore(10,'\n');

    if(r == "n") {
        function.Console_Log("Wait for quit programme ...",Func::type_msg::info);
        run = false;
        return;
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

    int balance = account.get_Balance();
    function.Console_Log("- Address : "+account.get_Address(),function.type_msg::info);

    //? TEST --------------------------------------------------------------------

    info_block.Amount = 0x174876E800;
    info_block.From = "0X39216175d066d5ca660308ca0ee5f25cc8ee91fd7f0";
    info_block.To = "0X39216175d06785ca660308ca0ee5f25cc8ee91fd7f0";
    Blockchain.push_back(new Block(Blockchain.size(),Blockchain[Blockchain.size()-1]->get_Hash(),info_block));
    
    //? END TEST --------------------------------------------------------------------

    Json::Value t;

    

    int block_size = Blockchain.size();

    while(run) {

        if(block_size != Blockchain.size()) {
            block_size = Blockchain.size();
            balance = account.get_Balance();
        }

        if(account.mining == true) {

            int limit = block_size + 1;

            for(int i = 0; i < Blockchain.size();i++) {
                if(Blockchain[i]->get_Signed() == "") {
                    if(Blockchain[i]->Signed(account.get_Address(),account.get_Account())) {
                        if(Blockchain.size() < limit) {
                            thread *_mining = new thread(Mining,Blockchain[i]);
                            _mining->join();
                            t["Tx"][i] = Blockchain[i]->get_Transaction();
                            function.Console_Log("Diff : "+to_string(Blockchain[i]->get_Diff()),Func::type_msg::info);
                            thread *_quit = new thread(quit);
                        }
                    }
                }
            }
        }
    }

    ofstream fs;
    fs.open("dev/test_tx",ios::binary);
    string save = base64_encode_pem(t.toStyledString());
    fs << t << endl;
    fs.close();


    return 0;
}