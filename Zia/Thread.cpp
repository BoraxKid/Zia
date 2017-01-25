#include "Thread.hpp"

namespace Zia
{
    Thread::Thread()
        : _toDestroy(false)
    {
        this->_worker = std::thread(&Thread::queueLoop, this);
    }

    Thread::~Thread()
    {
        if (this->_worker.joinable())
        {
            this->wait();
            this->_queueMutex.lock();
            this->_toDestroy = true;
            this->_condition.notify_one();
            this->_queueMutex.unlock();
            this->_worker.join();
        }
    }

    void Thread::addJob(const std::function<void()> &function)
    {
        std::lock_guard<std::mutex> lock(this->_queueMutex);
        this->_jobQueue.push(function);
        this->_condition.notify_one();
    }

    void Thread::wait()
    {
        std::unique_lock<std::mutex> lock(this->_queueMutex);
        this->_condition.wait(lock, [this]() { return (this->isJobQueueEmpty()); });
    }

    bool Thread::isJobQueueEmpty() const
    {
        return (this->_jobQueue.empty());
    }

    bool Thread::shouldDestroy() const
    {
        return (this->_toDestroy);
    }

    void Thread::queueLoop()
    {
        std::function<void()> job;

        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(this->_queueMutex);
                this->_condition.wait(lock, [this] { return (!this->isJobQueueEmpty() || this->shouldDestroy()); });
                if (this->_toDestroy)
                    break;
                job = this->_jobQueue.front();
            }
            job();
            {
                std::lock_guard<std::mutex> lock(this->_queueMutex);
                this->_jobQueue.pop();
                this->_condition.notify_one();
            }
        }
    }
}
