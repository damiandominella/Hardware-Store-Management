#!/bin/bash
echo "Generating data"
rm data_generator
gcc -ansi -Wall -O data_generator.c -o data_generator
./data_generator
echo "Compiling program"
rm hardware_store_management
gcc -ansi -Wall -O hardware_store_management.c -o hardware_store_management	
echo "Executing"
./hardware_store_management