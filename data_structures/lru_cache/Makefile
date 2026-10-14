CC := gcc

APP_NAME := demeo
APP_SRCS := $(wildcard *.c)
APP_OBJS := $(APP_SRCS:.c=.o)
APP_CFLAGS := -Ilib -g
APP_LDFLAGS := -L./ -Wl,-rpath,'$$ORIGIN'
APP_LDLIBS := -lcache

LIB_NAME = libcache.so
LIB_SRCS := $(wildcard lib/*.c)
LIB_OBJS := $(LIB_SRCS:.c=.o)
LIB_CFLAGS := -Wall -Wextra -fPIC -Ilib -g
LIB_LDFLAGS := -shared

all: $(LIB_NAME) $(APP_NAME)

$(LIB_NAME): $(LIB_OBJS)
	$(CC) $(LIB_LDFLAGS) -o $@ $^

lib/%.o: lib/%.c
	$(CC) $(LIB_CFLAGS) -c $< -o $@

$(APP_NAME): $(APP_OBJS) $(LIB_NAME)
	$(CC) $(APP_LDFLAGS) -o $@ $(APP_OBJS) $(APP_LDLIBS)

%.o: %.c
	$(CC) $(APP_CFLAGS) -c $< -o $@

install:

clean:
	rm -f $(APP_NAME) $(LIB_NAME) $(APP_OBJS) $(LIB_OBJS)


.PHONY: clean all install
