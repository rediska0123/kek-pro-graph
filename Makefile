.PHONY: all clean

CXX=g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude -Iserd/serd -IRedisGraph/src/graph -IRedisGraph/deps/xxHash -IRedisGraph/deps/rax

all: main

main: redis_all bin/main.o bin/parseRDF.o
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/main.o bin/parseRDF.o serd/build/src/*.3.o RedisGraph/src/redisgraph.so

rdf_test: redis_all bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o bin
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o serd/build/src/*.3.o RedisGraph/src/redisgraph.so

redis_all:
	@$(MAKE) -C ./RedisGraph all

bin:
	mkdir -p bin

bin/%.o: src/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: test/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin main rdf_test
	@$(MAKE) -C ./RedisGraph $@
