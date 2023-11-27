
CC := g++
FLAGS := -g -fsanitize=address

test: main
	./main

retest: clean test

main: main.cpp
	$(CC) -o main main.cpp -std=c++20 $(FLAGS)

draft:
	rm -f main_draft
	$(CC) -g draft.cpp -o main_draft -std=c++20 $(FLAGS)
	./main_draft

clean:
	rm -f main main_draft
