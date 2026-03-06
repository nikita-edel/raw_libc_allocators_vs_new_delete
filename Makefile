CC      = gcc
CXX     = g++
CFLAGS  = -O3 -std=c99   -D_POSIX_C_SOURCE=200809L
CXXFLAGS= -O3 -std=c++17 -fno-exceptions

ifeq ($(OS),Windows_NT)
	EXT = .exe
	RM  = del /Q
else
	EXT =
	RM  = rm -f
endif

TARGETS = bench_c$(EXT) bench_cpp$(EXT)

all: $(TARGETS)

bench_c$(EXT): main.c
	$(CC) $(CFLAGS) -o $@ $<

bench_cpp$(EXT): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

run: all
	./bench_c$(EXT)
	./bench_cpp$(EXT)

clean:
	$(RM) $(TARGETS)

.PHONY: all clean run
