all: data_generator hardware_store_management

data_generator:
	gcc -ansi -Wall -O data_generator.c -o data_generator

hardware_store_management: 
	gcc -ansi -Wall -O hardware_store_management.c -o hardware_store_management

clean:
	rm data_generator
	rm hardware_store_management