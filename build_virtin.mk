include env/common_head.mk

LD_BIN=virtin

CC_SRC = $(wildcard lib/*.c) $(wildcard src/*.c)
CC_INC = -I. -Iinclude -I$(USR_LOCAL)/include
LD_INC = -L$(USR_LOCAL)/lib
LD_LIB =

FLAG_CC=-ggdb -O0

include env/common_foot.mk
