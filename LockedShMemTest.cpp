/*
 * File:   LockedShMemTest.h
 * Author: Yanikeev-AS
 *
 * Created on August 6, 2016, 11:02 AM
 */

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
        printf("Error: cant execute shm_open for %s. Return code: %d\n", shmem_name, fd);
        return RC_SHMEM_OPEN_ERR;
    }

    if (ptr)
    {
        *ptr = (ShMem*)::mmap(NULL, sizeof(ShMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    } else
    {
        printf("Error: mmap NULL pointer for %s.\n", shmem_name);
        return RC_BAD_POINTER_ERR
    }

    return RC_OK;
}