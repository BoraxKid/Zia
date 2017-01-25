#include "ThreadPool.hpp"

namespace Zia
{
    ThreadPool::ThreadPool()
    {
    }

    ThreadPool::~ThreadPool()
    {
    }

    void ThreadPool::setThreadCount(int threadCount)
    {
        int i = 0;

        this->_threads.clear();
        while (i < threadCount)
        {
            this->_threads.push_back(std::make_unique<Thread>());
            ++i;
        }
    }

    void ThreadPool::addJob(int threadIndex, const std::function<void()> &function)
    {
        this->_threads.at(threadIndex)->addJob(function);
    }

    void ThreadPool::wait()
    {
        for (auto &thread : this->_threads)
            thread->wait();
    }
}
