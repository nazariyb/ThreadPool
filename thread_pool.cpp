#include <memory>

#include "thread_pool.h"

void thread_pool::worker_thread (unsigned my_index_)
{
    my_index = my_index_;
    local_work_queue = queues[my_index].get();
    while (!done) {
        run_pending_task();
    }
}

bool thread_pool::pop_task_from_local_queue (thread_pool::task_type &task)
{
    return local_work_queue && local_work_queue->try_pop(task);
}

bool thread_pool::pop_task_from_pool_queue (thread_pool::task_type &task)
{
    return pool_work_queue.try_pop(task);
}

bool thread_pool::pop_task_from_other_thread_queue (thread_pool::task_type &task)
{
    for (unsigned i = 0; i < queues.size(); ++i) {
        unsigned const index = (my_index + i + 1) % queues.size();
        if (queues[index]->try_steal(task)) {
            return true;
        }
    }
    return false;
}

thread_pool::thread_pool () :
        done(false), joiner(threads)
{
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
        for (unsigned i = 0; i < thread_count; ++i) {
            queues.push_back(std::make_unique<work_stealing_queue>());
            threads.emplace_back(&thread_pool::worker_thread, this, i);
        }
    }
    catch (...) {
        done = true;
        throw;
    }
}

void thread_pool::run_pending_task ()
{
    task_type task;
    if (pop_task_from_local_queue(task) ||
        pop_task_from_pool_queue(task) ||
        pop_task_from_other_thread_queue(task)) {
        task();
    } else {
        std::this_thread::yield();
    }
}