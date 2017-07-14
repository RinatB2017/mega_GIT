#!/bin/bash

nasm -f elf64 hello.asm -o hello
ld hello
./a.out