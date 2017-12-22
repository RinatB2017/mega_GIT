#!/bin/bash 

g++ -D_REENTRANT -o threads threads.c -lpthread
./threads
