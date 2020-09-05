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
    std::string tmpStr;
    while(std::cin>>tmpStr) {
        strcpy(pData->buf, tmpStr.c_str());
        shm.notify();
    }

    return 0;
}