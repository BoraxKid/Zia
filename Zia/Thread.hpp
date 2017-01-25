#ifndef ZIA_THREAD_HPP_
#define ZIA_THREAD_HPP_
#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace Zia
{
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();
        void addJob(const std::function<void()> &function);
        void wait();
        bool isJobQueueEmpty() const;
        bool shouldDestroy() const;

    private:
        void queueLoop();

        bool _toDestroy;
        std::thread _worker;
        std::queue<std::function<void()>> _jobQueue;
        std::mutex _queueMutex;
        std::condition_variable _condition;
    };
}

#endif // ZIA_THREAD_HPP_
