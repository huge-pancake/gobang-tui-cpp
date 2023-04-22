#!/bin/bash

if [[ $1 == 'build' ]]; then
	mkdir -p build
	g++ ./src/main.cpp -o ./build/gobang-tui
elif [[ $1 == 'brun' ]]; then
	mkdir -p build
	g++ ./src/main.cpp -o ./build/gobang-tui
	./build/gobang-tui
elif [[ $1 == 'run' ]]; then
	./build/gobang-tui
else
	echo "Usage: ./builder build|brun|run"
fi
