#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "virtual_input.h"


//common
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

//virtual key
int str_key_to_code(char* key, int len)
{
    int ret = -1;
    if(len)
    {
        for(uint32_t i = 0; i <= KEY_SCALE; ++i)
        {
            char* tmp = strndup(key, len);
            if(!strcasecmp(tmp, key_str[i]))
            {
                ret = i;
                break;
            }
            free(tmp);
        }
    }
    if( -1 == ret)
    {
        TRACE_WARNING("Failed to transfer key(%s with len=%d) string to code", key, len);
    }
    return ret;
}

int key_simulate(int fd, uint16_t code)
{
    if(!input_simulate(fd, EV_KEY, code, 1)
        && !input_simulate(fd, EV_SYN, 0, 0)
        && !input_simulate(fd, EV_KEY, code, 0)
        && !input_simulate(fd, EV_SYN, 0, 0))
    {
        return 0;
    }
    return -1;
}

int key_str_simulate(int fd, char* key)
{
    TRACE_LOG("simulate key %s", key);
    uint16_t cmd[5], cmd_num=0;
    int tmp;
    char* start = NULL, *parse = key;
    start = strchr(parse, '+');
    while(start = strchr(parse, '+'))
    {
        tmp = str_key_to_code(parse, start - parse);
        if(-1 == tmp)
            return -1;
        cmd[cmd_num++] = (uint16_t)tmp;
        parse = start + 1;
        if(4 <= cmd_num)
        {
            TRACE_WARNING("Now only support most 5 key pressed together!!");
            return -1;
        }
    }
    tmp = str_key_to_code(parse, strlen(parse));
    if(-1 == tmp)
        return -1;
    cmd[cmd_num++] = (uint16_t)tmp;

    //only one key
    if(1 == cmd_num)
    {
        return key_simulate(fd, cmd[0]);
    }

    //Composite key
    for(int i = 0; i < cmd_num; ++i)
    {
        if(input_simulate(fd, EV_KEY, cmd[i], 1)
            || input_simulate(fd, EV_SYN, 0, 0)
            || input_simulate(fd, EV_KEY, cmd[i], 2)
            || input_simulate(fd, EV_SYN, 0, 0))
        {
            return -1;
        }
    }

    //release key
    for(int i = 0; i < cmd_num; ++i)
    {
        input_simulate(fd, EV_KEY, cmd[i], 0);
        input_simulate(fd, EV_SYN, 0, 0);
    }
    return 0;
}

//virtual mouse
int mouse_simulate(int fd, uint16_t code, int32_t value)
{
    return input_simulate(fd, EV_REL, code, value);
}