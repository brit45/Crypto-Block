#!/bin/sh

g++ main.cpp Account.cpp sha256.cpp Block.cpp -o crypto -ljsoncpp -pthread
