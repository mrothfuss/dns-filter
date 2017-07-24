CC = cc
CFLAGS = -Wall -g

SRC = main.c buffer.c map_stp.c
OBJ = ${SRC:.c=.o}

all: dns_filter

options:
	@echo build options:
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "CC	= ${CC}"


dns_filter: ${OBJ}
	@echo ${CC} -o $@ ${OBJ}
	@${CC} -o $@ ${OBJ}

clean:
	@echo cleaning
	@rm -f dns_filter ${OBJ}

.PHONY: all options clean
