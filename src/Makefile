CC = gcc
CCFLAGS = -Wall -I../header -I../images
LDFLAGS = -lSDL2 -lSDL2_image -lm 
SRC = $(wildcard *.c)
OBJ = $(SRC:.c = .o)
EXEC = main1

all: $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c 
	$(CC) $(CCFLAGS) -o $@ -c $< 

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY : clean all

