TARGET = cnary
LIBRARY = libcnary.a
OBJECTS = cnary.o libcnary.a
LIBRARY_OBJECTS = node.o list.o iterator.o node_list.o node_iterator.o hashmap.o
CFLAGS=-g -I./include
LDFLAGS=


%.o: %.c
	$(CC) -o $(@) -c $(^) $(CFLAGS)

$(LIBRARY): $(LIBRARY_OBJECTS)
	$(AR) rs $(@) $(^)
	
$(TARGET): $(OBJECTS)
	$(CC) -o $(@) $(^) $(CFLAGS) $(LDFLAGS)

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(LIBRARY) $(OBJECTS) $(LIBRARY_OBJECTS)