//
// Created by adi on 24/01/17.
//

#include <unistd.h>
#include "ThreadPool.h"
#include "Comunicator.h"
void Task::operator()() {
    execute(arg);
}

ThreadPool::ThreadPool(int pool_size){
    this->pool_size = pool_size;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        cout << "unable to init taxi center lock" << endl;
    }
    for(int i=0; i < pool_size; i++){
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, ThreadPool::start_thread, (void*) this);
        if (ret != 0) { /*todo: error handling */
        }
        threads.push_back(tid);
    }
}

ThreadPool::~ThreadPool(){
    for (int i = 0; i < pool_size; i++) {
        Task* task = new Task(exit_thread, task);
        tasks.push_front(task);
    }
    for (int i = 0; i < pool_size; i++) {
        pthread_join(threads[i], NULL);
    }
    unsigned long tasks_size = tasks.size();
    for (unsigned long i = 0; i < tasks_size; i++ ){
        delete tasks.front();
        tasks.pop_front();
    }
    pthread_mutex_destroy(&lock);
};

void* ThreadPool::execute_thread(){
    Task* task = NULL;
    while(true) {
        while (tasks.empty()){
            sleep(SLEEP);
        }
        pthread_mutex_lock(&lock);
        task = tasks.front();
        tasks.pop_front();
        pthread_mutex_unlock(&lock);
        (*task)();
        delete task;
    }
    return NULL;
};

int ThreadPool::add_task(Task* task){
    this->tasks.push_back(task);
};

void* ThreadPool::start_thread(void* arg) {
    ThreadPool* tp = (ThreadPool*) arg;
    tp->execute_thread();
    return NULL;
}

void ThreadPool::exit_thread(void* task){
    delete (Task*)task;
    pthread_exit(NULL);
}
