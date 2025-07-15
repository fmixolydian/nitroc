all: nitroc

nitroc: src/*.cpp src/*.h
	g++ src/*.cpp  -o nitroc -std=c++17 -Wall -Wpedantic

clean:
	rm -f nitroc