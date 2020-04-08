# Guidence from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# better guide https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
EXE := DataLoggerServer

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude #prepro
CFLAGS   := -Wall -g #compiler flags
LDFLAGS  := -Llib -pthread#linker flags
LDLIBS   := -Bstatic -lconfig -Bdynamic -lm -lrt #3rd p libs

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ)


