TARGET = srf_test

CC = gcc

CUNIT_LDFLAGS = -lcunit
CUNIT_CFLAGS = 

CFLAGS = -Wall -g $(CUNIT_CFLAGS)

LDFLAGS = $(CUNIT_LDFLAGS)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	-rm -f *.o
	-rm -f *.gcno
	-rm -f $(TARGET)
