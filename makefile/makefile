TARGET = ../lib/libdialcrypt.a
OBJECTS = dialcrypt.o des_keys.o d3des.o fake_md5.o base64.o
CC = cc
CFLAGS = -c
AR = ar
ARFLAGS = -r
SOURCE_DIR = ../src/

all: $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	
%.o: $(SOURCE_DIR)%.c
	$(CC) $(CFLAGS) -o $@ $<
	
clean:
	rm $(OBJECTS)
	rm $(TARGET)
	
.PHONY: clean
