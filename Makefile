CFLAGS := `pkg-config --cflags gtk+-3.0 cairo`
LIBS := `pkg-config --libs gtk+-3.0 cairo`

findscale: findscale.c List.c			# default goal
	gcc $(CFLAGS) findscale.c List.c  $(LIBS) -o findscale
