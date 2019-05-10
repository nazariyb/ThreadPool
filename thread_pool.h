#ifndef TREAD_POOL_THREAD_POOL_H
#define TREAD_POOL_THREAD_POOL_H


#include <atomic>
#include <thread>
#include <functional>
#include <vector>
#include <future>
#include <queue>
#include "thread_safe_queue.h"
#include "function_wrapper.h"
#include "work_stealing_queue.h"

class thread_pool
    {
private:
    using task_type = function_wrapper;

    std::atomic_bool done;
    thread_safe_queue<task_type> pool_work_queue;
    std::vector<std::unique_ptr<work_stealing_queue>> queues;
    std::vector<std::thread> threads;
    join_threads joiner{};

    static thread_local work_stealing_queue *local_work_queue;
    static thread_local unsigned my_index;

    void worker_thread (unsigned my_index_);

    bool pop_task_from_local_queue (task_type &task);

    bool pop_task_from_pool_queue (task_type &task);

    bool pop_task_from_other_thread_queue (task_type &task);

public:
    thread_pool ();

    ~thread_pool ()
    {
        done = true;
    }

    void run_pending_task ();

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType ()>::type>
    submit (FunctionType f)
    {
        using result_type = typename std::result_of<FunctionType ()>::type;

        std::packaged_task<result_type ()> task(f);
        std::future<result_type> res(task.get_future());
        if (local_work_queue) {
            local_work_queue->push(std::move(task));
        } else {
            pool_work_queue.push(std::move(task));
        }
        return res;
    }
    };

#endif //TREAD_POOL_THREAD_POOL_H
