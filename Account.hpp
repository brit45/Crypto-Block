#include <iostream>
#include "Block.hpp"

#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

class Account {

    public:
        Account();
        ~Account();
        void Create_Seed();
        std::string get_Seed() const;
        std::string get_Account() const;
        std::string get_Address() const;
        double Balance(Block* Blockchain);
        double get_Balance() const;

        void Remake_seed(std::string seed);

        bool mining = false;

    private:

        std::string word[39] = {
        "apple",
        "gold",
        "house",
        "horse",
        "file",
        "mouse",
        "silver",
        "kid",
        "king",
        "queen",
        "president",
        "snake",
        "dog",
        "cat",
        "bird",
        "orange",
        "purple",
        "red",
        "blue",
        "child",
        "children",
        "about",
        "alone",
        "everyone",
        "standalone",
        "standbye",
        "hello",
        "paper",
        "child",
        "parent",
        "key",
        "sample",
        "value",
        "hat",
        "car",
        "bus",
        "herro",
        "villainess",
        "fish"
    };

        std::string seed;
        std::string account;
        std::string address;
        double balance = 0;
        int version;
};
#endif // __ACCOUNT_H__