SOURCE_CODE=src/server/*.cpp
TEST_CODE=src/server/tests/*.cpp
MAIN=src/main.cpp

build:
	g++ --std=c++11 $(MAIN) $(SOURCE_CODE) -O2 -o server.out

debug:
	g++ -g --std=c++11 $(MAIN) $(SOURCE_CODE) -o serverd.out

test:
	g++ -O2 --std=c++11 $(TEST_CODE) $(SOURCE_CODE) -o tests.out -lboost_unit_test_framework
	./tests.out
