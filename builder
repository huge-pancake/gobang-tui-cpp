#!/bin/bash

if [[ $1 == 'build' ]]; then
	mkdir -p build
	clang++ ./src/main.cpp -o ./build/gobang-tui -O3 -std=c++20
elif [[ $1 == 'brun' ]]; then
	mkdir -p build
	clang++ ./src/main.cpp -o ./build/gobang-tui -O3 -std=c++20
	./build/gobang-tui
elif [[ $1 == 'run' ]]; then
	./build/gobang-tui
else
	echo "Usage: ./builder build|brun|run"
fi
