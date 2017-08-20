#!/bin/bash
echo "rm hardware_store_management"
rm hardware_store_management
echo "gcc hardware_store_management.c"
gcc -ansi -Wall -O hardware_store_management.c -o hardware_store_management	
echo "executing"
./hardware_store_management