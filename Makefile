CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LIBS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = src/main.o

LIBSRC = RRPG_libsrc
LIB_NAME = librayrpg.a
LIB_OBJS = $(patsubst $(LIBSRC)/%.c, $(LIBSRC)/%.o, $(wildcard $(LIBSRC)/*.c))

all:main 

lib/$(LIB_NAME): $(LIB_OBJS)
	ar rcs $@ $^ 

$(LIBSRC)/%.o: $(LIBSRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)


main: $(SRC) lib/$(LIB_NAME)
	$(CC) $(CFLAGS) $^ $(LIBS) -o main

clean:
	rm -rf src/*.o *.exe $(LIBSRC)/*.o $(LIB_NAME)

