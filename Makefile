CC=g++ -std=c++0x -O2 -lm
WARN=-Wall -Wextra
SOURCES=geometry.cpp collision.cpp main.cpp

all: dep
	make main

dep:
	@echo -en > Makefile.dep
	@for s in $(SOURCES); do \
		g++ -M $$s >> Makefile.dep; \
		echo -e '\t'@echo CC $$s >> Makefile.dep; \
		echo -e '\t'@$(CC) $(WARN) -c $$s >> Makefile.dep; \
	done

-include Makefile.dep

main: geometry.o main.o collision.o
	@echo LINK $@
	@$(CC) -o $@ $^

clean:
	rm -fr *.o main Makefile.dep
