#!/bin/sh

g++ main.cpp Account.cpp sha256.cpp Block.cpp -o test -ljsoncpp -pthread
