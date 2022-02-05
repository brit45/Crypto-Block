#!/bin/sh

echo "Compile  Linux 'crypto'"
g++ Function.cpp main.cpp Account.cpp sha256.cpp Block.cpp Base64.cpp -o crypto -ljsoncpp -pthread
