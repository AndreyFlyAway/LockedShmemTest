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

// constants
#define BASE_SHMNAME            "SHMEM_%d"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// return codes
#define RC_CREATE_SHMEM_OK              0
#define RC_SHMEM_OPEN_ERR              -1
#define RC_BAD_POINTER_ERR             -2
#define RC_MPA_DAILD_ERR               -3
#define RC_FTRUNCATE_ERR               -4
#define RC_CLOSE_FD_ERR                -5
#define RC_SEM_INIT_ERR                -6
#define RC_SHMEM_MLCOK_ERR             -7

// structs
struct ShMem {
    sem_t               data_mutex;     // Posix semaphore, protects access to data
    uint64_t            ui_data;
    float               fl_data;
    char                ch_data[32];
};

// prototypes
int createShMem(int id, ShMem** ptr);

#endif //LOCKEDSHMEMTEST_LOCKEDSHMEMTEST_H
