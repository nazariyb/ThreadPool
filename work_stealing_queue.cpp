#include "work_stealing_queue.h"

void work_stealing_queue::push (work_stealing_queue::data_type data)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    the_queue.push_front(std::move(data));
}

bool work_stealing_queue::empty () const
{
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_queue.empty();
}

bool work_stealing_queue::try_pop (work_stealing_queue::data_type &res)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
        return false;
    }
    res = std::move(the_queue.front());
    the_queue.pop_front();
    return true;
}

bool work_stealing_queue::try_steal (work_stealing_queue::data_type &res)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    if (the_queue.empty()) {
        return false;
    }
    res = std::move(the_queue.back());
    the_queue.pop_back();
    return true;
}
