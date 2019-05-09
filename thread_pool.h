#ifndef TREAD_POOL_THREAD_POOL_H
#define TREAD_POOL_THREAD_POOL_H


#include <atomic>
#include <thread>
#include <functional>
#include <vector>
#include "thread_safe_queue.h"

class thread_pool
    {
private:
    std::atomic_bool done;
    thread_safe_queue<std::function<void ()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner{};

    void worker_thread ();

public:
    thread_pool ();

    ~thread_pool ()
    {
        done = true;
    }

    template<typename FunctionType>
    void submit (FunctionType f)
    {
        work_queue.push(std::function<void ()>(f));
    }
    };

#endif //TREAD_POOL_THREAD_POOL_H
