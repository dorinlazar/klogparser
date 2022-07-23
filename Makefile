.PHONY: executables makedirs
all: makedirs executables
build/kltime.o: src/kltime.cpp src/kltime.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/kltime.o src/kltime.cpp -Isrc -Isrc
build/logdb.o: src/logdb.cpp src/kltime.hpp src/logdb.hpp src/loginfo.hpp src/stringdb.hpp src/stringreader.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/logdb.o src/logdb.cpp -Isrc -Isrc
build/test-logparser.o: src/test-logparser.cpp src/kltime.hpp src/logdb.hpp src/loginfo.hpp src/stringreader.hpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/test-logparser.o src/test-logparser.cpp -Isrc -Isrc
build/loginfo.o: src/loginfo.cpp src/kltime.hpp src/loginfo.hpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/loginfo.o src/loginfo.cpp -Isrc -Isrc
build/stringreader.o: src/stringreader.cpp src/stringreader.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/stringreader.o src/stringreader.cpp -Isrc -Isrc
build/stringdb.o: src/stringdb.cpp src/stringdb.hpp
	g++ -Wall -Wextra -c -std=c++20 -g3 -O2 -flto -Werror -o build/stringdb.o src/stringdb.cpp -Isrc -Isrc
build/test-logparser.exe: build/kltime.o build/logdb.o build/loginfo.o build/stringdb.o build/stringreader.o build/test-logparser.o
	g++ -flto -Werror -ltcmalloc -o build/test-logparser.exe build/kltime.o build/logdb.o build/loginfo.o build/stringdb.o build/stringreader.o build/test-logparser.o
executables: build/test-logparser.exe
makedirs:
	mkdir -p build
