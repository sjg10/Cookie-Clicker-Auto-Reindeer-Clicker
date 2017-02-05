TARGET = clicker
MAGICKWAND =  `pkg-config --cflags --libs MagickWand`
LIBS = ${MAGICKWAND}
CC = gcc
CFLAGS = -g -Wall ${MAGICKWAND}
SRC_FOLDER = ../src
CSRC = $(wildcard ${SRC_FOLDER}/*.c)
#CSRC = ../src/main.c ../src/scan.c
HEADERS = $(wildcard $(SRC_FOLDER)/*.h)

#VPATH will ensure C files are searched for in SRC_FOLDER not local
VPATH = ${SRC_FOLDER}

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(notdir ${CSRC}))

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
