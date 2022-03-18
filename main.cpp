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
#include <thread>
#include <jsoncpp/json/json.h>
#include "Network.hpp"

using namespace std;

Func function;

vector<Block*> Blockchain;
Account account;

int list_block_file = 0;
int lenght_block = 0;
bool run = true;


void Mining(Block* b) {
    int start = time(NULL);
    int end;
    bool s;

    while(!(s = b->Mining(account.get_Address()) && b->get_Signed() != "")) {}

    end = time(NULL);

    std::string input = b->get_Data().From + b->get_Data().To + std::to_string(b->get_Data().Amount) + b->get_Parent();

    std::string de = sha256(account.get_Address() + input + std::to_string(b->get_Nonce()));

    if(s){

        int pos = b->get_head() + 1;

        if(de == b->get_Signed()) {

            Block_info info_block;
            info_block.Amount = (b->get_Diff()+(end-start))+(rand()%b->get_Diff()*50);
            info_block.From = "Reward";
            info_block.To = account.get_Address();
            int size = Blockchain.size();
            Blockchain.push_back(new Block(size,Blockchain[size-1]->get_Hash(),info_block));
            account.Balance(Blockchain);
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


void quit() {

    string r = "n";
    std::cout << endl << endl;
    std::cout << "Quit (y/N) : ";
    getline(cin,r);
    cin.ignore(10,'\n');

    if(r == "y" || r == "Y") {
        std::cout << endl << endl;
        function.Console_Log("Wait for quit programme ...",Func::type_msg::alert);
        run = false;
        return;
    }
    else {}
}




int main(int argc, char* argv[]) {

    // Server *server = new Server();

    // server->Bind("127.0.0.1",8000);
    // server->Listen();


    // return 0;

    fstream fsx, fs_index;

    fs_index.open("dev/index");

    if(fs_index.is_open()) {
        
        function.Console_Log("Open List of Transaction ...",function.type_msg::info);
        Json::Reader read_data;
        Json::Value obj;

        string index;

        while(!getline(fs_index,index).eof()) {
            list_block_file++;
            fsx.open(index);
            read_data.parse(fsx,obj);

            function.Console_Log("Read Transaction : "+to_string(obj["Tx"].size()),Func::type_msg::info);

            for(int i=0;i < obj["Tx"].size();i++) {

                Block* blk = new Block();
                blk->set_Transaction(obj["Tx"][i]);
                
                function.Console_Log("Load Transaction "+index.substr(4,9)+" "+obj["Tx"][i]["Hash"].asString().substr(0,8),function.type_msg::add);
                Blockchain.push_back(blk);
            }

            fsx.close();
        }

        lenght_block = Blockchain.size();

        std::cout << endl << endl;
        function.Console_Log("Lenght Transaction Loaded : "+to_string(lenght_block),Func::info);

    }
    else {

        Block_info info_block;
        info_block.Amount = 0;
        info_block.From = "0x0000000000000000000000000000000000000000000";
        info_block.To = "Reward";

        function.Console_Log("Creation de la Genesis",function.type_msg::info);

        Block *genesis = new Block(0,"the sunday 5th on the moon paradise is fake.",info_block);
        Blockchain.push_back(genesis);
    }

    fsx.close();
    
    std::cout << endl << endl;
    function.Console_Log("- Address : "+account.get_Address(),function.type_msg::info);

    //? ========================================================================================= TEST

    Block_info info_block;
    info_block.Amount = 20;
    info_block.From = "0X39216175d066d5ca660308ca0ee5f25cc8ee91fd7f0";
    info_block.To = "0x0000000000000000000000000000000000000000000";
    Blockchain.push_back(new Block(Blockchain[Blockchain.size()-1]->get_head()+1,Blockchain[Blockchain.size()-1]->get_Parent(),info_block));
    
    //? ========================================================================================= END TEST


    int block_size = Blockchain.size();

    thread *_quit = new thread(quit);



    while(run) {

        if(block_size != Blockchain.size()) {
            
            block_size = Blockchain.size();
            
        }

        if(account.mining == true) {

            int limit = block_size + 1;

            for(int i = 0; i < Blockchain.size() && run == true;i++) {
                if(Blockchain[i]->get_Signed() == "" && run == true) {
                    if(Blockchain[i]->Signed(account.get_Address(),account.get_Account())) {
                        if(Blockchain.size() < limit) {
                            Mining(Blockchain[i]);
                            function.Console_Log("Head : "+to_string(Blockchain[i]->get_head()),Func::type_msg::info);
                            function.Console_Log("Diff : "+to_string(Blockchain[i]->get_Diff()),Func::type_msg::info);
                            function.Console_Log("Reward : "+to_string(Blockchain[i]->get_Data().Amount),Func::type_msg::info);
                        }
                    }
                }
            }
        }

        
    }

    account.Balance(Blockchain);

    account.get_Balance();

    if(block_size != lenght_block) {
        Json::Value t;

        int l = 0;
        for(int i = ((lenght_block) == 0)?0:lenght_block; i < block_size; i++) {
            function.Console_Log("Read Transaction on head :"+to_string(Blockchain[i]->get_head()),Func::type_msg::info);
            t["Tx"][l] = Blockchain[i]->get_Transaction();
            l++;
        }

        ofstream fs;

        fs.open("./dev/index",ios_base::app);
        string save = "./dev/test__" + to_string(list_block_file);
        fs << save << endl;
        fs.close();

        fs.open(save);
        fs << t << endl;
        fs.close();
    }

    return 0;
}