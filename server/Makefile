SOURCE_CODE=src/server/*.cpp src/server/commands/*.cpp
TEST_CODE=src/server/tests/*.cpp
MAIN=src/main.cpp
CPP_VERSION=c++14

build:
	g++ --std=$(CPP_VERSION) $(MAIN) $(SOURCE_CODE) -O2 -o server.out

debug:
	g++ -g --std=$(CPP_VERSION) $(MAIN) $(SOURCE_CODE) -o serverd.out

test:
	g++ -O2 --std=$(CPP_VERSION) $(TEST_CODE) $(SOURCE_CODE) -o tests.out -lboost_unit_test_framework
	./tests.out
