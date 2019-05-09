#include "thread_pool.h"

void thread_pool::worker_thread ()
{
    while (!done) {
        std::function<void ()> task;
        if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
}

thread_pool::thread_pool () :
        done(false), joiner(threads)
{
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
        for (unsigned i = 0; i < thread_count; ++i) {
            threads.emplace_back(&thread_pool::worker_thread, this);
        }
    }
    catch (...) {
        done = true;
        throw;
    }
}
