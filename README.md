# Algorithms and data structure project for autumn session of 2017
## University of Urbino

### Project specification
Suppose you are asked to design a program for the management of a hardware store. Write an ANSI C program that works as follows:

- It gets a text file whose format entails a given number of rows, each one containing: name of the product, product    identification code (4 characters), unitary cost, number of stored pieces (separated by space or tab).
For instance:
     - Nail_L_2.5  N924  1.20  300
     - Hammer H610  12.50  127
     - Screwdriver S234 4.55 432
     
- It loads data into a suitable tree data structure.
- It allows the user to choose and perform the following operations:
     - insertion of new input from keyboard;
     - deletion of existing data, selected from keyboard;
     - print on display of the sorted list of data, sorted according to one of the 4 keys, chosen by the user.
     
     
For what concerns theoretical analysis, the complexity of the following operations must be given: insertion, deletion, sorted list print.
Apart from theoretical analysis, an experimental study has to be done regarding the complexity of algorithms. In particular, N rows must be randomly generated to be provided as input to the program. The experimental analysis must therefore evaluate the complexity of algorithms for insertion, deletion, sorted list print when N grows.

### Steps to do to run program (on linux/unix systems):

```bash
make

./data_generator (only if you want to decide how many articles the program must process)

./hardware_store_management
```
