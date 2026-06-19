CXX ?= g++
CPPFLAGS ?= -I. -Iinclude
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic

PROGRAMS := main.exe console_main.exe tests.exe
HEADERS := print.h util.h $(wildcard include/*.h) $(wildcard include/*.tpp)

.PHONY: all main console test run-tests clean

all: $(PROGRAMS)

main: main.exe

console: console_main.exe

test run-tests: tests.exe
	./tests.exe

main.exe: main.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

console_main.exe: console_main.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

tests.exe: tests.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean:
	$(RM) $(PROGRAMS)
