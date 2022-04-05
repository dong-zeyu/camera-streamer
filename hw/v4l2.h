#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/videodev2.h>

#ifndef CFG_XIOCTL_RETRIES
#	define CFG_XIOCTL_RETRIES 4
#endif
#define XIOCTL_RETRIES ((unsigned)(CFG_XIOCTL_RETRIES))

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

extern int log_debug;

// assumes that name is first item
#define dev_name(dev) (dev ? *(const char**)dev : "?")
#define E_LOG_ERROR(dev, _msg, ...)		do { fprintf(stderr, "%s: %s: " _msg "\n", __FILENAME__, dev_name(dev), ##__VA_ARGS__); goto error; } while(0)
#define E_LOG_PERROR(dev, _msg, ...)		do { fprintf(stderr, "%s: %s: " _msg "\n", __FILENAME__, dev_name(dev), ##__VA_ARGS__); exit(-1); } while(0)
#define E_LOG_INFO(dev, _msg, ...)		do { fprintf(stderr, "%s: %s: " _msg "\n", __FILENAME__, dev_name(dev), ##__VA_ARGS__); } while(0)
#define E_LOG_VERBOSE(dev, _msg, ...)	do { fprintf(stderr, "%s: %s: " _msg "\n", __FILENAME__, dev_name(dev), ##__VA_ARGS__); } while(0)
#define E_LOG_DEBUG(dev, _msg, ...)		do { if (log_debug) { fprintf(stderr, "%s: %s: " _msg "\n", __FILENAME__, dev_name(dev), ##__VA_ARGS__); } } while(0)

typedef struct {
	char buf[10];
} fourcc_string;

fourcc_string fourcc_to_string(unsigned format);
unsigned fourcc_to_stride(unsigned width, unsigned format);
int xioctl(const char *name, int fd, int request, void *arg);
uint64_t get_monotonic_time_us(struct timespec *ts, struct timeval *tv);
int shrink_to_block(int size, int block);

#define E_XIOCTL(dev, _fd, _request, _value, _msg, ...) do { \
		int ret; \
		if ((ret = xioctl(dev_name(dev), _fd, _request, _value)) < 0) { \
			E_LOG_ERROR(dev, "xioctl(ret=%d): " _msg, ret, ##__VA_ARGS__); \
		} \
	} while(0)