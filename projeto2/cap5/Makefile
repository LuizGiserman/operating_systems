# the compiler
CC = gcc

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall -pthread

# the build target executable:
TARGET = barber

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)