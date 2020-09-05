#include <string.h>
#include <iostream>
#include <string>
#include "shm.h"


struct Data {
    char buf[1024];
};

int main()
{
    ShmManger shm;

    if(!shm.create("/sharememtest", sizeof(Data))) {
        return -1;
    }

    Data* pData = (Data*)shm.getBuf();

    while(1) {
        shm.waitfor();
        std::cout<<pData->buf<<std::endl;
    }

    return 0;
}