#    Copyright 2017 Sam Gonshaw
#
#    This file is part of Cookie Clicker Auto Reindeer Clicker.
#    Cookie Clicker Auto Reindeer Clicker
#
#    Cookie Clicker Auto Reindeer Clicker is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Cookie Clicker Auto Reindeer Clicker is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Cookie Clicker Auto Reindeer Clicker.
#    If not, see <http://www.gnu.org/licenses/>.



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
