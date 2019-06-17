#include <iostream>
#include "LockedShMemTest.h"

int main() {
    printf("Started\n");

    pthread_t write_thread_id;
    //pthread_t read_thread_id;

    pthread_create(&write_thread_id, NULL, &write_thread, NULL);
//    pthread_create(&read_thread_id, NULL, &read_thread, NULL);
//

    usleep(500000);

    void * unused;
    read_thread(unused);

    releaseShMem(1);

    //pthread_join(write_thread_id, NULL);

    return 0;
}