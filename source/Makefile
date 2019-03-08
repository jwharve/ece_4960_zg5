CC = gcc
CFLAGS = -Wall
LIBS = -lwiringPi -lm

TARGET = zen
SRCS = util.c move.c
DEPS = def.h

OBJS = $(SRCS:.c=.o)
HDRS = $(SRCS:.c=.h)
EXE = $(SRCS:.c=)

all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJS) $(DEPS)
	$(CC) -o $(TARGET) $(TARGET).c $(OBJS) $(CFLAGS) $(LIBS)

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

clean:
	rm $(OBJS) main $()
