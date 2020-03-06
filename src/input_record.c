#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include "virtual_input.h"

char* help_msg = 
"\
SYNOPSIS:\n\
	virtin [OPT] devices\n\
OPTIONS:\n\
    virtin can be used to simulate linux input event or record the input events during sometimes.\n\
    -m, --mode\n\
        0 means manual simulattion, user should set input type, code and value\n\
        1 means capture, only file is useful, program will run forever until user terminate it with ctrl+c or kill command.\n\
        2 means auto simulation. \n\
    -t, --type\n\
        input event type, used when mode 0.\n\
    -c, --code\n\
        input event code, used when mode 0.\n\
    -v, --value\n\
        input event value, used when mode 0.\n\
    -l, --loop\n\
        loop the action with several times.\n\
    -i, interval\n\
        the interval miliseconds between each loop. \n\
    -f, --file\n\
        when mode 1, capture the event to this file,\n\
        when mode 2, get simulation data from this file.\n\
    -V, --version\n\
        output version information and exit\n\
    -h, --help display this help and exit\n\
";

int main(int argc, char* argv[])
{
	int c;
	int long_index = 0;
    uint32_t mode=0, loop = 1 , interval = 100;
    uint16_t type, code;
    int32_t value;
    int dev_fd=-1, file_fd=-1;
    char* file = NULL, *device=NULL;
    bool is_type_set=false, is_code_set=false, is_value_set=false;
	static struct option long_opts[] = {
		{ "mode", required_argument, NULL, 'm' },
        { "type", required_argument, NULL, 't' },
        { "code", required_argument, NULL, 'c' },
        { "value", required_argument, NULL, 'v' },
        { "file", required_argument, NULL, 'f' },
        { "loop", required_argument, NULL, 'l' },
        { "interval", required_argument, NULL, 'i' },
		{ "version", no_argument, NULL, 'V' },
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0}
	};

	while( (c = getopt_long(argc, argv, "m:t:c:v:f:Vh", long_opts, &long_index)) != -1)
	{
		switch(c)
		{
		case 0:
			break;
        case 'm':
            mode = atoi(optarg);
            break;
        case 't':
            is_type_set = true;
            type = atoi(optarg);
            if(type < 0)
            {
                TRACE_ERROR("Wrong type %d.", type);
                return -1;
            }
            break;
        case 'c':
            is_code_set = true;
            code = atoi(optarg);
            if(code < 0)
            {
                TRACE_ERROR("Wrong code %d.", code);
                return -1;
            }
            break;
        case 'v':
            is_value_set = true;
            value = atoi(optarg);
            break;
		case 'f':
			file = strdup(optarg);
			break;
        case 'l':
            loop = atoi(optarg);
            if(loop < 0)
            {
                TRACE_ERROR("Wrong loop %d.", loop);
                return -1;
            }
            break;
        case 'i':
            interval = atoi(optarg);
            if(interval < 0)
            {
                TRACE_ERROR("Wrong interval %d.", interval);
                return -1;
            }
            break;
		case 'V':
			TRACE_LOG("version %s", VERSION);
			return 0;
		case 'h':
			TRACE_LOG("%s", help_msg);
			return 0;
		default:
			TRACE_LOG("%s", help_msg);
			return 0;
		}
	}
    if(optind == argc)
    {
        TRACE_WARNING("%s", help_msg);
        return -1;
    }

    device = argv[argc - 1];
    TRACE_LOG("device is %s\n", device);

	dev_fd = open(device, O_RDWR);

    if(dev_fd <= 0)
    {
        TRACE_ERROR("error open devices %s, error is %s!", device, strerror(errno));
        return -1;
    }

    if(file)
    {
        file_fd = open(file, O_RDWR);
        if(dev_fd <= 0)
        {
            TRACE_ERROR("error open files %s, error is %s!", file, strerror(errno));
            return -1;
        }
    }
    else
    {
        if(mode==2)
        {
            TRACE_WARNING("When in mode 2, file is necessary!");
            close(dev_fd);
            return -1;
        }
    }

    switch(mode)
    {
    case 0:
        if(is_type_set && is_code_set && is_value_set)
        {
            while(loop--)
            {
                input_simulate(dev_fd, type, code, value);
                usleep(interval*1000);
            }
        }
        else
        {
            TRACE_WARNING("When in mode 0, user should set input type, code and value!");
            return -1;
        }
        break;
    case 1:
        {
            struct input_event event;
            while(!input_capture(dev_fd, &event))
            {
                TRACE_LOG("type=%d, code=%d, value=%d\n", event.type, event.code, event.value);
                if (file_fd > 0 && write(file_fd, &event, sizeof(event)) < 0)
                {
                    TRACE_ERROR("record event to file failed, error is %s!", strerror(errno));
                    break;
                }
            }
        }
        break;
    case 2:
        {
            while(loop--)
            {
                struct input_event event;
                while(read(file_fd, &event, sizeof(event)==sizeof(event)))
                {
                    if(input_simulate(dev_fd, event.type, event.code, event.value))
                    {
                        break;
                    }
                }
                usleep(interval*1000);
                lseek(file_fd, 0, SEEK_SET);
            }
        }
        break;
    default:
        TRACE_ERROR("Unsupport mode %d!", mode);
        break;
    }
    close(dev_fd);
    if(file_fd > 0)close(file_fd);
	return 0;
}