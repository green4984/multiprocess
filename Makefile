
SRC_DIR := .
INC_DIR := .
CC = gcc
TARGET = a.out

OBJS := $(shell ls $(SRC_DIR)/*.c)

CFLAGS += $(foreach inc, $(INC_DIR), -I$(inc))
CFLAGS += -g -Wall -DDEBUG
OBJS_O := $(foreach obj, $(OBJS), $(obj:.c=.o))

$(SRC_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h $(INC_DIR)/main.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/main.h
	$(CC) $(CFLAGS) -c $< -o $@

all : $(TARGET)
$(TARGET) : $(OBJS_O)
	$(CC) $(CFLAGS) $(OBJS_O) -o $(TARGET)
	@cd $(SRC_DIR) && ctags *.c

.PHONY : clean all
clean :
	-rm -f $(OBJS_O) $(TARGET)
