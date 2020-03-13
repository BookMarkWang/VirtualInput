#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "virtual_input.h"

int input_simulate(int fd, uint16_t type, uint16_t code, int32_t value)
{
    struct input_event event;

    event.type = type;
    event.code = code;
    event.value = value;
    gettimeofday(&event.time, 0);

    if (write(fd, &event, sizeof(struct input_event)) < 0)
    {
        TRACE_ERROR("input simulate error, error is %s", strerror(errno));
        return -1;
    }
    return 0;
}

int key_simulate(int fd, uint16_t code, int32_t value)
{
    return input_simulate(fd, EV_KEY, code, value);
}

int mouse_simulate(int fd, uint16_t code, int32_t value)
{
    return input_simulate(fd, EV_REL, code, value);
}

int input_capture(int fd, struct input_event *event)
{
    int ret;
    if(event)
    {
        ret = read(fd, event, sizeof(struct input_event));
        if(ret != sizeof(struct input_event))
        {
            TRACE_ERROR("input capture error, read %d bytes, error is %s", ret, strerror(errno));
            return -1;
        }
        return 0;
    }
    return -1;
}