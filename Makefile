.PHONY: executables makedirs
all: makedirs executables
build/logdb.o: src/logdb.cpp src/kltime.hpp src/logdb.hpp src/loginfo.hpp src/stringdb.hpp src/stringreader.hpp
	g++ -c src/logdb.cpp -o build/logdb.o -Isrc -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/loginfo.o: src/loginfo.cpp 
	g++ -c src/loginfo.cpp -o build/loginfo.o -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/kltime.o: src/kltime.cpp src/kltime.hpp
	g++ -c src/kltime.cpp -o build/kltime.o -Isrc -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/stringreader.o: src/stringreader.cpp src/stringreader.hpp
	g++ -c src/stringreader.cpp -o build/stringreader.o -Isrc -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/test-logparser.o: src/test-logparser.cpp src/logdb.hpp src/loginfo.hpp src/kltime.hpp src/stringreader.hpp src/stringdb.hpp
	g++ -c src/test-logparser.cpp -o build/test-logparser.o -Isrc -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/customparsers.o: src/customparsers.cpp 
	g++ -c src/customparsers.cpp -o build/customparsers.o -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/stringdb.o: src/stringdb.cpp src/stringdb.hpp
	g++ -c src/stringdb.cpp -o build/stringdb.o -Isrc -Isrc -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/tests/test-is-digit.o: src/tests/test-is-digit.cpp src/customparsers.hpp
	g++ -c src/tests/test-is-digit.cpp -o build/tests/test-is-digit.o -Isrc -Isrc -Isrc/tests -std=c++23 -g3 -O2 -flto -Werror -Wall -Wextra

build/test-logparser.exe: build/kltime.o build/logdb.o build/stringdb.o build/stringreader.o build/test-logparser.o
	g++ -o build/test-logparser.exe build/kltime.o build/logdb.o build/stringdb.o build/stringreader.o build/test-logparser.o -flto -Werror

build/tests/test-is-digit.exe: build/tests/test-is-digit.o
	g++ -o build/tests/test-is-digit.exe build/tests/test-is-digit.o -flto -Werror -lfmt

executables: build/test-logparser.exe build/tests/test-is-digit.exe
makedirs:
	mkdir -p build build/tests
