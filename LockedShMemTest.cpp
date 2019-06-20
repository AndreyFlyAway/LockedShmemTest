/*
 * File:   LockedShMemTest.h
 * Author: Yanikeev-AS
 *
 * Created on August 6, 2016, 11:02 AM
 */

#include "LockedShMemTest.h"

int createShMem(int id, ShMem** ptr, int locked, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);

    // creating shared memory
    ::shm_unlink(shmem_name);
    int fd = ::shm_open(shmem_name, O_RDWR | O_CREAT | O_EXCL | O_NONBLOCK, FILE_MODE);

    if (fd < 0) {
        printf("Error: shm_open eeprog /dev/i2c-4 0x57 -16 -f -r 0x0 for %s. Return code: %d\n", shmem_name, fd);
        return RC_SHMEM_OPEN_ERR;
    }

    if (ptr) {
        *ptr = (ShMem *) ::mmap(NULL, sizeof(ShMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    } else {
        printf("Error: mmap NULL pointer for %s.\n", shmem_name);
        return RC_BAD_POINTER_ERR;
    }

    if (*ptr == MAP_FAILED) {
        printf("ERROR: mmap faild %s", shmem_name);
        return RC_MAP_FAILD_ERR;
    }

    if (::ftruncate(fd, sizeof(ShMem)) == -1) {
        printf("ERROR: mmap failed %s", shmem_name);
        return RC_FTRUNCATE_ERR;
    }

    if (::close(fd) == -1) {
        printf("ERROR: close by file descriptor of shared memory failed %s, fd: %d", shmem_name, fd);
        return RC_CLOSE_FD_ERR;
    }
    // semaphore initialization
    if (::sem_init(&(*ptr)->data_mutex, 1, 1) == -1)
    {
        printf("ERROR: failed %s", shmem_name);
        return RC_SEM_INIT_ERR;
    }

    // block shared memory
    if (locked == 1)
        if (mlock(ptr, sizeof(ShMem)) == -1)
        {
            printf("! ! mlock error for %s: %d (%s)\n", shmem_name, errno, strerror(errno));
            return RC_SHMEM_MLCOK_ERR;
        }

    // data initialization

    (*ptr)->ui_data = 0;
    (*ptr)->fl_data = 0.0;
    ::sprintf((*ptr)->ch_data, "Init 1");

    return RC_SHMEM_OK;
}

int getShMem(int id, ShMem** ptr, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);

    int fd = ::shm_open(shmem_name, O_RDWR, FILE_MODE);
    if (fd < 0)
    {
        printf("Error: shm_open failed for %s. Return code: %d\n", shmem_name, fd);
        return RC_SHMEM_OPEN_ERR;
    }

    if (ptr) {
        *ptr = (ShMem *) ::mmap(NULL, sizeof(ShMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    } else {
        printf("Error: mmap NULL pointer for %s.\n", shmem_name);
        return RC_BAD_POINTER_ERR;
    }

    if (*ptr == MAP_FAILED) {
        printf("ERROR: mmap faild %s", shmem_name);
        return RC_MAP_FAILD_ERR;
    }

    if (::close(fd) == -1) {
        printf("ERROR: close by file descriptor of shared memory failed %s, fd: %d", shmem_name, fd);
        return RC_CLOSE_FD_ERR;
    }

    return RC_SHMEM_OK;
}

int freeShMem(ShMem* ptr)
{
    if(ptr)
    {
        if(::munmap(ptr, sizeof(ShMem))<0)
        {
            printf("ERROR: freeShMem faild: %d (%s)\n", errno, strerror(errno));
            return RC_MAP_FAILD_ERR;
        }
    }

    return RC_SHMEM_OK;
}

int releaseShMem(int id, char* shmem_name_fmt)
{
    char shmem_name[32];
    ::sprintf(shmem_name, shmem_name_fmt, id);

    return ::shm_unlink(shmem_name);
}

void* write_thread(void * params)
{
    int get_mem_res {};
    tread_args * args = (tread_args *)params;

    for (int i = 0; i < args->shamem_num; i++)
    {
        ShMem* ptr = nullptr;
        get_mem_res = getShMem(i, &(ptr), args->chmem_fmt);
        if (get_mem_res != RC_SHMEM_OK)
        {
            return (void*)(get_mem_res);
        } else
        {
            sprintf(ptr->ch_data, "test_%d", i);
            ptr->ui_data = (uint)i;
            ptr->fl_data = (float)i;
        }
    }

    return NULL;
}

void* read_thread(void * params)
{
    uint sleep_time {520000};

    ShMem* ptr = nullptr;
    getShMem(1, &(ptr));

    for (int i = 0; i < EXPEREMETN_COUNTER; i++)
    {
        printf("Read thread %s\n", ptr->ch_data);
        usleep(sleep_time);
    }

    return NULL;
}

/* testing consumed time for work with locked shared memory and non locked*/
void shared_mem_test()
{
    /* create 2 sets of shared memory*/
    /* not locked */
    for(auto i=1; i<=SHARED_MEM_OBJ_NUM; i++)
    {
        ShMem* shmem_ptr = nullptr;
        createShMem(i, &shmem_ptr, 0, (char *)(BASE_SHMNAME_FMT)); // give optional arguments for clarity
    }
    /* locked */
    for(auto i=1; i<=SHARED_MEM_OBJ_NUM; i++)
    {
        ShMem* shmem_ptr = nullptr;
        createShMem(i, &shmem_ptr, 1, (char *)(LOCKED_SHMNAME_FMT));
    }

}