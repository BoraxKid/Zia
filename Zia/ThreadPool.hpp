#ifndef ZIA_THREADPOOL_HPP_
#define ZIA_THREADPOOL_HPP_
#pragma once

#include "Thread.hpp"

namespace Zia
{
    class ThreadPool
    {
    public:
        ThreadPool();
        virtual ~ThreadPool();
        void setThreadCount(int threadCount);
        void addJob(int threadIndex, const std::function<void()> &function);
        void wait();

    private:
        std::vector<std::unique_ptr<Thread>> _threads;
    };
}

#endif // ZIA_THREADPOOL_HPP_
