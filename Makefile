src := src/stage_2.c \
       src/stage_3.c \
       src/stage_4.c \
       src/stage_5.c \
       src/stage_hidden.c \
       src/crc32.c \
       src/rsa32.c

headers := src/main.h \
           src/crc32.h \
           src/rsa32.h

LIBS := -lpsapi

all: $(src) $(headers)
	gcc -shared -Wl,--kill-at -Wall -Wextra $(src) $(LIBS) -o bin/FMJ8FR.