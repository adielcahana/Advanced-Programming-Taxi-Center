//
// Created by adi on 24/01/17.
//


#include "ThreadPool.h"
/******************************************************************************
* The function Operation: execute the function that
* was given in the ctor with its args
******************************************************************************/
void Task::run() {
    execute(arg);
}
/******************************************************************************
* The function Operation: ThreadPool ctor, create pool_size threads
******************************************************************************/
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
/******************************************************************************
* The function Operation: ThreadPool dtor
******************************************************************************/
ThreadPool::~ThreadPool(){
    vector<Task*> killTasks;
    pthread_mutex_lock(&lock);
    //give a 'kill' task to all the active threads
    for (int i = 0; i < pool_size; i++) {
        Task* task = new Task(exit_thread, NULL);
        killTasks.push_back(task);
        tasks.push_front(task);
    }
    pthread_mutex_unlock(&lock);
    //wait for all the threads to exit
    for (int i = 0; i < pool_size; i++) {
        pthread_join(threads[i], NULL);
    }
    for (unsigned long i = 0; i < killTasks.size(); i++ ){
        delete killTasks[i];

    }
    //delete all remaining tasks
    unsigned long tasks_size = tasks.size();
    for (unsigned long i = 0; i < tasks_size; i++ ){
        delete tasks.front();
        tasks.pop_front();
    }
    pthread_mutex_destroy(&lock);
};
/******************************************************************************
* The function Operation: a generic function for each thread. the thread will
* alwayas try to get a new task, and execute it.
******************************************************************************/
void* ThreadPool::execute_thread(){
    Task* task = NULL;
    while(true) {
        //wait untill there is new task
        while (tasks.empty()){
            sleep(SLEEP);
        }
        //lock the accses to the task queue
        pthread_mutex_lock(&lock);
        //check if the thread managed to lock before the queue was emptied again
        if (tasks.empty()){
            pthread_mutex_unlock(&lock);
            continue;
        }
        //wait until there is new task
        task = tasks.front();
        tasks.pop_front();
        pthread_mutex_unlock(&lock);
        task->run();
        delete task;
    }
    return NULL;
};
/******************************************************************************
* The function Operation: add a task to the task queue
******************************************************************************/
int ThreadPool::add_task(Task* task){
    this->tasks.push_back(task);
};
/******************************************************************************
* The function Operation: a static function for thread creation. the function
* calls execute_thread, in order to provide the thread the task queue within
 * the threadpool
******************************************************************************/
void* ThreadPool::start_thread(void* arg) {
    ThreadPool* tp = (ThreadPool*) arg;
    tp->execute_thread();
    return NULL;
}
/******************************************************************************
* The function Operation: a 'kill' task for the thraedpool dtor
******************************************************************************/
void ThreadPool::exit_thread(void* arg){
    pthread_exit(arg);
}
