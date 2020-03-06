#ifndef __VIRTUAL_INPUT_H__
#define __VIRTUAL_INPUT_H__

#include <stdint.h>
#include <linux/input.h>
#include "trace.h"

int input_simulate(int fd, uint16_t type, uint16_t code, int32_t value);

int key_simulate(int fd, uint16_t code, int32_t value);
int mouse_simulate(int fd, uint16_t code, int32_t value);

int input_capture(int fd, struct input_event *event);

#endif //__VIRTUAL_INPUT_H__