CC = gcc
CFLAGS = -Iinclude -Wall -Wextra $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib) -Llib -lrayrpg -lopengl32 -lgdi32 -lwinmm


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
	$(CC) $(CFLAGS) -c $< -o $@  


main: $(SRC) lib/$(LIB_NAME)
	$(CC) $(CFLAGS) $^ $(LIBS) -o main -Wl,--verbose

clean:
	rm -rf src/*.o *.exe $(LIBSRC)/*.o lib/$(LIB_NAME)

