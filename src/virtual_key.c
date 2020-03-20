#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <signal.h>
#include "virtual_input.h"

#define MAX_EPOLL_FILE_NUM 1024
#define MAX_FILE_NUM 1024

bool g_running = true;

char* help_msg = 
"\
SYNOPSIS:\n\
	virtkey [OPT] devices\n\
OPTIONS:\n\
    virtkey can be used capture/simulate key info from/to devices or files.\n\
    -f, --from\n\
        where to read key event, should be files or keyboard devices. one or more can follow.\n\
    -t, --to\n\
        where to write the key event, should be files or keyboard devices. one or more can follow.\n\
    -k, --key\n\
        one or more string key, used when simulate.\n\
    -l, --loop\n\
        loop the action with several times.\n\
    -i, interval\n\
        the interval miliseconds between each loop. \n\
    -V, --version\n\
        output version information and exit\n\
    -h, --help display this help and exit\n\
";

void write_to_files_with_buf(char* buf, uint32_t len, int* fd_array, int fd_num)
{
    if(!buf || !len){
        TRACE_WARNING("Wrong buf info, skip buf(%p),len(%d)", buf, len);
        return;
    }
    for(int i = 0; i < fd_num; ++i)
    {
        if(fd_array[i] > 0)
        {
            int left_len = len, write_len=0;
            while(left_len)
            {
                write_len = write(fd_array[i], buf, len);
                if(write_len <= 0)
                {
                    TRACE_LOG("Failed to write buf to fd_array[%d](%d), error is %s", i, fd_array[i], strerror(errno));
                    break;
                }
                left_len = len - write_len;
            }
            
        }
    }
}

void write_to_files_with_fd(int fd, int* fd_array, int fd_num)
{
    struct input_event event;
    while(!input_capture(fd, &event))
    {
        //TRACE_LOG("type(%d), code(%d), value(%d)", event.type, event.code, event.value);
        if(EV_KEY == event.type)
            TRACE_LOG("key(%s) %s", key_str[event.code], event.value==1?"Pressed":(event.value==2?"Long Pressed":"Released"));
        gettimeofday(&event.time, 0);
        write_to_files_with_buf((char*)&event, sizeof(struct input_event), fd_array, fd_num);
    }
}

void sig_handle(int sig,siginfo_t *info,void *t)
{
    if( SIGINT == sig )
    {
        TRACE_LOG("ctrl+c pressed!");
        g_running = false;
    }
    else if(SIGQUIT == sig )
    {
        TRACE_LOG("ctrl+/ pressed!");
        g_running = false;
    }
}

int main(int argc, char* argv[])
{
	int c;
	int long_index = 0;
	uint32_t loop = 1 , interval = 100;
	int from_fds[MAX_FILE_NUM]={-1}, to_fds[MAX_FILE_NUM]={-1};
    char* from_list[MAX_FILE_NUM] = {NULL};
    char* to_list[MAX_FILE_NUM] = {NULL};
    char* key_list[MAX_FILE_NUM] = {NULL};
    int from_num = 0, to_num=0, key_num=0;
	static struct option long_opts[] = {
		{ "from", required_argument, NULL, 'f' },
		{ "to", required_argument, NULL, 't' },
		{ "key", required_argument, NULL, 'f' },
		{ "loop", required_argument, NULL, 'l' },
		{ "interval", required_argument, NULL, 'i' },
		{ "version", no_argument, NULL, 'V' },
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0}
		};

    struct sigaction act;
    act.sa_sigaction = sig_handle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);

	while( (c = getopt_long(argc, argv, "f:t:k:l:i:Vh", long_opts, &long_index)) != -1)
	{
		switch(c)
		{
		case 0:
			break;
        case 'f':
            from_list[from_num++] = optarg;
            while(optind < argc )
            {
                if('-' == argv[optind][0])
                {
                    break;
                    
                }
                from_list[from_num++] = argv[optind++]; 
            }
            break;
        case 't':
            to_list[to_num++] = optarg;
            while(optind < argc)
            {
                if('-' == argv[optind][0])
                {
                    break;
                    
                }
                to_list[to_num++] = argv[optind++]; 
            }
            break;
        case 'k':
            key_list[key_num++] = optarg;
            while(optind < argc )
            {
                if('-' == argv[optind][0])
                {
                    break;
                    
                }
                key_list[key_num++] = argv[optind++]; 
            }
            break;
        case 'l':
            loop = atoi(optarg);
            TRACE_LOG("optarg=%s", optarg);
            if(loop < 0)
            {
                TRACE_ERROR("Wrong loop %d.", loop);
                return -1;
            }
            break;
        case 'i':
        TRACE_LOG("optarg=%s", optarg);
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
        case ':':
            TRACE_LOG("No argument for option %s", argv[optind]);
            break;
        case '?':
            TRACE_LOG("Unknown option %s", argv[optind]);
            break;
		default:
			TRACE_LOG("%s", help_msg);
			return 0;
		}
	}
    if(1 == optind)
    {
        TRACE_LOG("%s", help_msg);
    }

    TRACE_LOG("loop is %d", loop);
    for(int i = 0; i < from_num; ++i)
    {
        TRACE_LOG("From_list[%d]=%s", i, from_list[i]);
        from_fds[i] = open(from_list[i], O_RDWR);
        if(from_fds[i] <= 0)
        {
            TRACE_ERROR("error open file %s, error is %s!", from_list[i], strerror(errno));
        }
    }
    for(int i = 0; i < to_num; ++i)
    {
        TRACE_LOG("to_list[%d]=%s", i, to_list[i]);
        to_fds[i] = open(to_list[i], O_RDWR | O_CREAT);
        if(to_fds[i] <= 0)
        {
            TRACE_ERROR("error open file %s, error is %s!", to_list[i], strerror(errno));
        }
    }
    for(int i = 0; i < key_num; ++i)
    {
        TRACE_LOG("key_list[%d]=%s", i, key_list[i]);
    }

    struct stat stat_buf;
    int status;
    bool has_fd_for_epoll = false;
    int  epoll_fd;
    int wait_fds;
    struct  epoll_event ev;  
    struct  epoll_event evs[MAX_EPOLL_FILE_NUM];

    epoll_fd = epoll_create( MAX_EPOLL_FILE_NUM );

    for(int i = 0; i < from_num; ++i)
    {
        if(from_fds[i] > 0)
        {
            status = fstat(from_fds[i], &stat_buf);
            if(!status)
            {
                switch (stat_buf.st_mode & S_IFMT) {
                case S_IFBLK:
                case S_IFCHR:
                case S_IFDIR:
                case S_IFIFO:
                case S_IFSOCK:
                    ev.events = EPOLLIN | EPOLLET; 
                    ev.data.fd = from_fds[i];
                    if( epoll_ctl( epoll_fd, EPOLL_CTL_ADD, from_fds[i], &ev ) < 0 )  
                    {  
                        TRACE_WARNING("Epoll Error : %s\n", strerror(errno));  
                    }
                    else
                    {
                        has_fd_for_epoll = true;
                    }
                    break;
                case S_IFLNK:
                case S_IFREG:
                    for(int k = 0; k < loop; ++k)
                    {
                        write_to_files_with_fd(from_fds[i], to_fds, to_num);
                        lseek(from_fds[i], SEEK_SET, 0);
                    }
                    break;
                default: printf("unknown?\n"); break;
                }
            }
            else
            {
                TRACE_WARNING("Failed to fstat from file index:%d, error is %s", i, strerror(errno));
            }
        }
    }

    TRACE_LOG("to_num = %d", to_num);
    for(int i = 0; i < to_num; ++i)
    {
        TRACE_LOG("to_fds[%d]=%d, key_num=%d", i, to_fds[i], key_num);
        if(to_fds[i] > 0)
        {
            for(int k = 0; k < loop; ++k)//loop
            {
                for(int j = 0; j < key_num; ++j)
                {
                    key_str_simulate(to_fds[i], key_list[j]);
                }
            }
        }
    }


    while(g_running && has_fd_for_epoll)
    {
        if( ( wait_fds = epoll_wait( epoll_fd, evs, MAX_EPOLL_FILE_NUM, 500 ) ) == -1 )  
        {  
            TRACE_ERROR( "Epoll Wait Error : %s", strerror(errno) );
        }
        for(int i = 0; i < wait_fds; ++i)
        {
            if(evs[i].events & EPOLLIN)
            {
                write_to_files_with_fd(evs[i].data.fd, to_fds, to_num );
            }
        }
    }
    TRACE_LOG("Terminate!!");


    for(int i = 0; i < from_num; ++i)
    {
        if(from_fds[i] > 0)
        {
            close(from_fds[i]);
        }
    }
    for(int i = 0; i < to_num; ++i)
    {
        if(to_fds[i] > 0)
        {
            close(to_fds[i]);
        }
    }
    close(epoll_fd);
	return 0;
}
