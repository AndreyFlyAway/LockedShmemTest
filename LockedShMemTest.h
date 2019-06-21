/*
 * File:   LockedShMemTest.h
 * Author: Yanikeev-AS
 *
 * Created on August 6, 2016, 11:02 AM
 */

#ifndef LOCKEDSHMEMTEST_LOCKEDSHMEMTEST_H
#define LOCKEDSHMEMTEST_LOCKEDSHMEMTEST_H

// headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/mman.h>
#include <cstdio>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include <iostream>

// constants
#define	EXPEREMETN_COUNTER	           10
#define BASE_SHMNAME_FMT               "SHMEM_%d"
#define LOCKED_SHMNAME_FMT             "LOCKED_SHMEM_%d"
#define	FILE_MODE	                   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define NMESG                          64
#define SHARED_MEM_OBJ_NUM             128

// return codes
#define RC_SHMEM_OK                     0
#define RC_SHMEM_OPEN_ERR              -1
#define RC_BAD_POINTER_ERR             -2
#define RC_MAP_FAILD_ERR               -3
#define RC_FTRUNCATE_ERR               -4
#define RC_CLOSE_FD_ERR                -5
#define RC_SEM_INIT_ERR                -6
#define RC_SHMEM_MLCOK_ERR             -7

/* shared memory data struct */
struct ShMem {
    sem_t       data_mutex;     // Posix semaphore, protects access to data
    uint64_t    ui_data;
    float       fl_data;
    char        ch_data[32];
};

/* queue struct (nahuya mne eto?) */
struct QueueData {
    sem_t	    mutex;
    int32_t     nstored;            // number of busy position  in queue
    int32_t     nget;               // index of element for getting operation
    int32_t     nput;               // index of element for putting operation
    ShMem	    msgdata[NMESG];     // the data
};

/* thread's parameters struct */
struct tread_args{
    char * chmem_fmt = (char *)(BASE_SHMNAME_FMT);
    int shamem_num {SHARED_MEM_OBJ_NUM};
};

// prototypes
int createShMem(int id, ShMem** ptr, int locked=0, char* shmem_name_fmt=(char *)(BASE_SHMNAME_FMT));
int getShMem(int id, ShMem** ptr, char* shmem_name_fmt=(char *)(BASE_SHMNAME_FMT));
int freeShMem(ShMem* ptr);
int releaseShMem(int id, char* shmem_name_fmt=(char *)(BASE_SHMNAME_FMT));
void* write_thread(void * params);
void* read_thread(void * params);
void shared_mem_test();
uint64_t get_timestamp_ns();

#endif //LOCKEDSHMEMTEST_LOCKEDSHMEMTEST_H
