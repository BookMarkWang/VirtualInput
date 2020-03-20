include env/common_head.mk

LD_OBJ = SO
LD_BIN = libvirtin.so

CC_SRC = $(wildcard lib/*.c)
CC_INC = -I. -Iinclude -I$(USR_LOCAL)/include
LD_INC = -L$(USR_LOCAL)/lib
LD_LIB =

FLAG_CC=-ggdb -O0 -fPIC

include env/common_foot.mk
