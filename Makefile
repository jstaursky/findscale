CFLAGS := `pkg-config --cflags gtk+-3.0 cairo`
LIBS := `pkg-config --libs gtk+-3.0 cairo`
SRC := ./src
INCLUDES := ./include/

findscale: src/findscale.c src/List.c			# default goal
	gcc $(CFLAGS) -I$(INCLUDES) \
		$(SRC)/findscale.c $(SRC)/List.c  \
		$(LIBS) -o findscale
