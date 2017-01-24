//
// Created by adi on 24/01/17.
//

#ifndef EX4_THREADPOOL_H
#define EX4_THREADPOOL_H

#include <vector>
#include <pthread.h>
#include <iostream>
#include <queue>

using namespace std;
class Task{
public:
    Task(void(*execute_fn)(void*), void* arg):execute(execute_fn), arg(arg){};
    ~Task(){};
    void operator()();
private:
    void (*execute)(void*);
    void* arg;
};

class ThreadPool {
public:
    ThreadPool(int pool_size);
    ~ThreadPool();
    void* execute_thread();
    int add_task(Task* task);
private:
    int pool_size;
    pthread_mutex_t lock;
    vector<pthread_t> threads;
    deque<Task*> tasks;
    static void* start_thread(void* arg);
    static void exit_thread(void* arg = NULL);
};


#endif //EX4_THREADPOOL_H
