# Will want to eventually replace with autoconf soln.
# Read http://make.mad-scientist.net/papers/how-not-to-use-vpath/
# For reasons why multi-directory builds are hard to hand write Makefiles for.
CC		= gcc
CFLAGS  	= `pkg-config --cflags gtk+-3.0 cairo`
CFLAGS	   	+= -g
LIBS  		= `pkg-config --libs gtk+-3.0 cairo`

SRC		= ./src
INCLUDES 	= ./include
OBJDIR 		= ./lib
OBJECTS 	= CircularLinkedList.o \
			  findscale.o

TARGET 		= findscale


EXE = $(addprefix $(SRC)/, $(TARGET))

$(EXE) : $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(CC) $^ $(LIBS) -o $@

$(OBJDIR)/%.o : $(SRC)/%.c
	$(CC) -c $(CFLAGS) -I$(INCLUDES) $< -o $@


# clean:
# 	rm -rf $(OBJDIR)/*.o $(SRC)/$(TARGET)
