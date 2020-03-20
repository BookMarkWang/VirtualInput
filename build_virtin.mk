include env/common_head.mk

LD_BIN=virtin

CC_SRC = $(wildcard src/input_record.c)
CC_INC = -I. -Iinclude -I$(USR_LOCAL)/include
LD_INC = -L$(USR_LOCAL)/lib -L.
LD_LIB = -lvirtin

FLAG_CC=-ggdb -O0

include env/common_foot.mk
