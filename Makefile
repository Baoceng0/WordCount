#export CFLAGS= '-Wall -Wextra -std=c99'
# Author Name: Cenglin Bao 

# target all means all targets currently defined in this file
all: lab5

# this target is the .zip file that must be submitted to Carmen
lab5.zip: Makefile main.c word_count.c bounded_buffer.c hashmap.c bounded_buffer.h hashmap.h word_count.h
	zip lab5.zip Makefile main.c word_count.c bounded_buffer.c hashmap.c bounded_buffer.h hashmap.h word_count.h

lab5: main.c word_count.c bounded_buffer.c hashmap.c bounded_buffer.h hashmap.h word_count.h
	gcc -std=gnu99 -o lab5 -lpthread main.c word_count.c bounded_buffer.c hashmap.c


# clean up
clean:
	rm -rf *.o lab5 lab5.zip


