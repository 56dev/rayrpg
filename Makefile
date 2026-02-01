CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
DEPS = raylib.h
LIBS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
SRC = main.o

LIBSRC = RRPG_libsrc
LIB_NAME = librayrpg.a
LIB_OBJS = $(patsubst $(LIBSRC)/%.c, $(LIBSRC)/%.o, $(wildcard $(LIBSRC)/*.c))

all:main 

$(LIB_NAME): $(LIB_OBJS)
	ar rcs $@ $^ 

$(LIBSRC)/%.o: $(LIBSRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


main: $(SRC) $(LIB_NAME)
	$(CC) $(CFLAGS) $^ $(LIBS) -o main

clean:
	rm -rf *.o *.exe $(LIBSRC)/*.o $(LIB_NAME)

