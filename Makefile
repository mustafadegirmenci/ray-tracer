# TODO: -O3 option should be included to improve performance while compiling

CC = g++
CFLAGS = -std=c++11

SRC_DIR = src
INCLUDE_DIR = include

INCLUDE_HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
INCLUDE_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

SUBDIR_HEADERS = $(wildcard $(INCLUDE_DIR)/**/*.h)
SUBDIR_SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp)

HEADERS = $(INCLUDE_HEADERS) $(SUBDIR_HEADERS)
SOURCES = $(INCLUDE_SOURCES) $(SUBDIR_SOURCES)

raytracer: $(HEADERS) $(SOURCES)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f raytracer