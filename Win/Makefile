UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
TARGET = main
else
TARGET = main.exe
endif


all:
	gcc -Wall -o bin/$(TARGET) src/navigation.c src/exploration.c src/main.c 

clean:
	rm bin/*