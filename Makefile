INCLUDE=-I.
CC=g++ -g -ggdb -pg -std=c++0x -O2 -lm $(INCLUDE)
LIBS=-lsfml-system -lsfml-graphics -lsfml-window
WARN=-Wall -Wextra
SOURCES=geometry.cpp\
	utility.cpp\
	collision.cpp\
	main.cpp\
	detector.cpp\
	shape.cpp\
	shapemanager.cpp\
	actor.cpp\
	actormanager.cpp\
	messages.cpp\
	xonix.cpp\
	test/run_tests.cpp

OBJS=geometry.o utility.o collision.o detector.o shape.o shapemanager.o actor.o actormanager.o messages.o xonix.o

all: dep
	make main

.PHONY: test
test: all
	make test/run_tests
	./test/run_tests

dep: test/run_tests.cpp
	@echo -en > Makefile.dep
	@for s in $(SOURCES); do \
		$(CC) -M $$s >> Makefile.dep; \
		echo -e '\t'@echo CC $$s >> Makefile.dep; \
		echo -e '\t'@$(CC) $(WARN) -c $$s >> Makefile.dep; \
	done

-include Makefile.dep

main: $(OBJS) main.o
	@echo LINK $@
	@$(CC) $(LIBS) -o $@ $^

test/run_tests.cpp test/gen_run_tests.sh:
	(cd test && ./gen_run_tests.sh > run_tests.cpp)

test/run_tests: $(OBJS) run_tests.o
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -fr *.o main test/run_tests test/run_tests.cpp test/*.o Makefile.dep
