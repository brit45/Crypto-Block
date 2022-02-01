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
Account *account;

void mining(Block* b) {
    if(b->get_Signed() == "") {
        b->Mining(account->get_Address());
    }
}

int main(int argc, char* argv[]) {
    
    bool run = true;

    if(account->mining) {
        string chainmain_dir = "/home/"+(string)getenv("USER")+"/.crypto/chainmain";

        bool dir_s = opendir(chainmain_dir.c_str());
        if(dir_s) {
            fstream fs;

            fs.open(chainmain_dir.append("/index.dat"));
            for(int i(0); i < Blockchain.size();i++) {
                thread _mining(mining,account->get_Address());
                _mining.join();
                fs << Blockchain.data();
            }
            fs.close();
        }
        else {
            string mkdir = "mkdir "+chainmain_dir;
            system(mkdir.c_str());
            
            fstream fs;

            fs.open(chainmain_dir.append("/index.dat"));
            for(int i(0); i < Blockchain.size();i++) {
                thread _mining(mining,account->get_Address());
                _mining.join();
                fs << Blockchain.data();
            }
            fs.close();
        }
    }
    
    do {}
    while(run);

    return 0;
}