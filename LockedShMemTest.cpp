//
// Created by user on 6/13/19.
//

#include "LockedShMemTest.h"

int createShMem(int id, ShMem** ptr)
{
    char shmem_name[32];
    ::sprintf(shmem_name, BASE_SHMNAME, id);

    // creating shared memory
    ::shm_unlink(shmem_name);
    int fd = ::shm_open(shmem_name, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, FILE_MODE);

    if (fd < 0)
    {
        printf("Error: cant execute shm_open for %s: %d (%s)\n", shmem_name, fd);
    }


}