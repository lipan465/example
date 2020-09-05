#ifndef SHARE_MEMERY_H_
#define SHARE_MEMERY_H_
#include <stdio.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>


// 使用ipcs -m可以查看当前系统所有的共享内存空间信息

//如果你的程序创建了一个共享内存段，但没有销毁，可以使用命令ipcrm -m shmid命令删除共享内存段，不然程序再运行有可能出错

class ShmManger {
public:
    ShmManger() {
        shmId_ = -1;
        shmPtr_ = nullptr;
        semPtr_ = nullptr;
    }

    ~ShmManger()
    {
        if(shmPtr_ != nullptr) {
            if(shmdt(shmPtr_) == -1) {

            }
        }

        if(semPtr_ != nullptr) {
            if(sem_close(semPtr_) == -1) {

            }
        }
    }

    bool create(const char* name, size_t size) 
    {
        key_t shmKey = ftok(name, 0);

        shmId_ = shmget(shmKey, size, IPC_CREAT|0666);
        if(shmId_ == -1) return false;

        semPtr_ = sem_open(name, O_CREAT, 0644, 0);
        if(SEM_FAILED == semPtr_) {
            printf("%d", errno);
            shmId_ = -1;
            return false;
        }

        return true;
    }

    void* getBuf() 
    {
        if(shmId_ == -1) {
            return nullptr;
        }

        if(shmPtr_ != nullptr) {
            return shmPtr_;
        }

        void *p = shmat(shmId_, NULL, 0);
        if(p == ((void*) -1)) {
            return nullptr;
        }

        shmPtr_ = p;
        return p;
    }

    void notify()
    {
        sem_post(semPtr_);
    }

    void waitfor()
    {
        sem_wait(semPtr_);
    }
private:
    int shmId_;
    void* shmPtr_;
    sem_t* semPtr_;
};

#endif