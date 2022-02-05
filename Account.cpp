#include <random>
#include "Account.hpp"
#include "sha256.hpp"
#include <fstream>
#include <ctime>
#include <jsoncpp/json/json.h>


Func console;

const int VERSION = 1;

std::string link;

Account::Account() {

    srand(time(NULL));

    link = "/home/"+(std::string)std::getenv("USER")+"/.crypto/wallet/ids";    

    console.Console_Log("Load wallet file ...",console.type_msg::info);
    
    std::ifstream ifs(link);

    if(ifs.is_open()) {
        
        console.Console_Log("Read wallet config ...",console.type_msg::info);

        Json::Reader reader;
        Json::Value obj;
        reader.parse(ifs,obj);

        this->account = obj["account"].asString();
        this->seed = obj["seed"].asString();
        this->address = obj["address"].asString();
        this->version = obj["version"].asInt();
        this->mining = obj["mining"].asBool();
    } else {
        console.Console_Log("Not found wallet file ...",console.type_msg::alert);
    }

}

Account::~Account() {
    Json::Value User;

    User["seed"] = this->seed;
    User["account"] = this->get_Account();
    User["address"] = this->get_Address();
    User["mining"] = this->mining;
    User["version"] = VERSION;


    std::ofstream fs;
    fs.open(link);
    fs << User;
    fs.close();

    console.Console_Log("Exit Account.\n",console.type_msg::info);
}

void Account::Create_Seed() {

    link = "/home/"+(std::string)std::getenv("USER")+"/.crypto/wallet/ids";

    std::string seed;
    int val = 39;
    for(int i = 0; i < 13; i++) {
        seed.append(this->word[(rand()% val)]);
        if(i < 12) {
            seed.append(" ");
        }
    }

    this->seed = seed;
    this->account = sha256(seed);
    this->address = "0X" + sha256(this->account.substr(0,14)).substr(0,43);
    
    Json::Value User;

    User["seed"] = this->seed;
    User["account"] = this->get_Account();
    User["address"] = this->get_Address();
    User["mining"] = this->mining;
    User["version"] = VERSION;

    std::ofstream fs;

    console.Console_Log("Create Wallet ...",console.type_msg::info);

    std::string directory = "mkdir /home/"+(std::string)std::getenv("USER")+"/.crypto -p";

    std::system(directory.c_str());
    
    directory = "mkdir /home/"+(std::string)std::getenv("USER")+"/.crypto/wallet -p";
    std::system(directory.c_str());

    std::printf("[ \033[32;1m+\033[0m ] - Save Wallet '%s'\n",link.c_str());
    
    fs.open(link);
    fs << User;
    fs.close();
}

std::string Account::get_Seed() const {
    return this->seed;
}

std::string Account::get_Account() const {
    return this->account;
}

std::string Account::get_Address() const {
    return this->address;
}

void Account::Remake_seed(std::string seed)  {
    this->seed = seed;
    this->account = sha256(seed);
    this->address = "0X" + sha256(this->account.substr(0,14)).substr(0,43);

    #if __linux
    Json::Value User;

    User["seed"] = this->seed;
    User["account"] = this->get_Account();
    User["address"] = this->get_Address();
    User["mining"] = this->mining;
    User["version"] = VERSION;

    std::ofstream fs;

    console.Console_Log("Remake wallet ...\n",console.type_msg::info);
    
    fs.open(link);
    fs << User;
    fs.close();
    
    #endif

    return;
}


double Account::Balance(Block* Blockchain) {
    if(Blockchain->get_Data().From == this->get_Address()) {
        this->balance -= Blockchain->get_Data().Amount;
    }
    if(Blockchain->get_Data().To == this->get_Address()) {
        this->balance += Blockchain->get_Data().Amount;
    }
    return this->balance;
}

double Account::get_Balance() const {
    return this->balance;
}