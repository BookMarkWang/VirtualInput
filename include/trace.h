#ifndef __TRACE_H__
#define __TRACE_H__
/**
 * @file    trace.h
 * @author  wangyang
 *
 * @brief   tmp for trace log macro
 */

#include <string.h>
#include <stdio.h>

#define VERSION "1.0"

#define TRACE_LOG(information, ...)  do{fprintf(stderr, "func:%s; "information"\n", __FUNCTION__, ##__VA_ARGS__);fflush(stdout);}while(0)
#define TRACE_MSG(information, ...)    do{fprintf(stderr, "func:%s; "information"\n",  __FUNCTION__, ##__VA_ARGS__);fflush(stdout);}while(0)

#define TRACE_ERROR(information, ...) do{fprintf(stderr, "func:%s; "information"\n", __FUNCTION__, ##__VA_ARGS__);fflush(stderr);}while(0)
#define TRACE_WARNING(information, ...)  do{fprintf(stderr, "func:%s; "information"\n", __FUNCTION__, ##__VA_ARGS__);fflush(stderr);}while(0)

#define TRACE_FUNC_HEAD TRACE_ERROR("start");
#define TRACE_FUNC_TAIL TRACE_ERROR("end");

#endif //__TRACE_H__

