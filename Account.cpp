#include <random>
#include "Account.hpp"
#include "sha256.hpp"
#include <jsoncpp/json/json.h>
#include <fstream>

const int VERSION = 1;

std::string link;

Account::Account() {
    
    srand(time(NULL));

    link = "/home/"+(std::string)std::getenv("USER")+"/.crypto/wallet/ids";    

    std::printf("[ \033[34;1mi\033[0m ] Load wallet file ...\n\t- %s\n", link.c_str());
    
    std::ifstream ifs(link);

    if(ifs.is_open()) {
        
        std::printf("[ \033[34;1mi\033[0m ] Read wallet config ...\n");

        Json::Reader reader;
        Json::Value obj;
        reader.parse(ifs,obj);

        this->account = obj["account"].asString();
        this->seed = obj["seed"].asString();
        this->address = obj["address"].asString();
        this->version = obj["version"].asInt();
        this->mining = obj["mining"].asBool();
    } else {
        std::printf("[ \033[31;1m!\033[0m ] Not found wallet file ...\n");
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

    std::printf("\n\n[ \033[34;1mi\033[0m ] Exit Account.\n");
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

    std::printf("[ \033[34;1mi\033[0m ] Create wallet ...\n");
    std::printf("[ \033[32;1m+\033[0m ] - Create directory :\n");
    std::printf("[ \033[32;1m+\033[0m ]\t - .crypto\n");
    
    std::string directory = "mkdir /home/"+(std::string)std::getenv("USER")+"/.crypto -p";

    std::system(directory.c_str());

    std::printf("[ \033[32;1m+\033[0m ]\t\t - wallet\n");
    
    directory = "mkdir /home/"+(std::string)std::getenv("USER")+"/.crypto/wallet -p";
    std::system(directory.c_str());

    std::printf("[ \033[32;1m+\033[0m ] - File '%s'\n",link.c_str());
    
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

    Json::Value User;

    User["seed"] = this->seed;
    User["account"] = this->get_Account();
    User["address"] = this->get_Address();
    User["mining"] = this->mining;
    User["version"] = VERSION;

    std::ofstream fs;

    std::printf("[ \033[34;1mi\033[0m ] Remake wallet ...\n");
    
    fs.open(link);
    fs << User;
    fs.close();

    return;
}