.PHONY: all clean

CXX=g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude -Iserd/serd

TESTS = bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parse_regex_test.o bin/automaton_test.o bin/regex_to_fsa_test.o
SOURCE = bin/parseRDF.o bin/parse_regex.o bin/deterministic_automaton.o bin/non_deterministic_automaton.o bin/regex_to_fsa.o bin/automaton_intersection.o

all: main

main: bin/main.o $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/main.o $(SOURCE) serd/build/src/*.3.o

tests: bin $(TESTS) $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude $(TESTS) $(SOURCE) serd/build/src/*.3.o

bin:
	mkdir -p bin

bin/%.o: src/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: test/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin main tests
