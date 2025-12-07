CC ?= cc
AR ?= ar
TARGET = cnary
LIBRARY = libcnary.a
OBJECTS = cnary.o
LIBRARY_OBJECTS = node.o list.o iterator.o node_list.o node_iterator.o hash_map.o
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g -I./include
LDFLAGS ?=


%.o: %.c
	$(CC) -o $(@) -c $(^) $(CFLAGS)

$(LIBRARY): $(LIBRARY_OBJECTS)
	$(AR) rcs $(@) $(^)
	
$(TARGET): $(OBJECTS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(LIBRARY) $(OBJECTS) $(LIBRARY_OBJECTS)