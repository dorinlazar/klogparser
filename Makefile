.PHONY: executables makedirs
all: makedirs executables
build/test-logparser.o: src/test-logparser.cpp src/stringdb.hpp src/loginfo.hpp src/kltime.hpp src/logdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/test-logparser.o src/test-logparser.cpp -Isrc -Isrc
build/logdb.o: src/logdb.cpp src/logdb.hpp src/kltime.hpp src/loginfo.hpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/logdb.o src/logdb.cpp -Isrc -Isrc -Ideps/ctre/single-header/
build/kltime.o: src/kltime.cpp src/kltime.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/kltime.o src/kltime.cpp -Isrc -Isrc -Ideps/ctre/single-header/
build/loginfo.o: src/loginfo.cpp src/loginfo.hpp src/kltime.hpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/loginfo.o src/loginfo.cpp -Isrc -Isrc
build/stringdb.o: src/stringdb.cpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/stringdb.o src/stringdb.cpp -Isrc -Isrc
build/test-logparser.exe: build/logdb.o build/kltime.o build/loginfo.o build/stringdb.o build/test-logparser.o
	g++ -flto=6 -Werror -ltcmalloc -o build/test-logparser.exe build/logdb.o build/kltime.o build/loginfo.o build/stringdb.o build/test-logparser.o
executables: build/test-logparser.exe
makedirs:
	mkdir -p build
