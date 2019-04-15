#pragma once

#include <mbgl/actor/scheduler.hpp>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace mbgl {

struct ThreadLifecycle {
    virtual void onThreadCreated() const {};
    virtual void onThreadDestroyed() const {};
    virtual ~ThreadLifecycle() = default;
};

class ThreadPool : public Scheduler {
public:
    explicit ThreadPool(std::size_t count, std::unique_ptr<ThreadLifecycle> _lifecycle = std::make_unique<ThreadLifecycle>());
    ~ThreadPool() override;

    void schedule(std::weak_ptr<Mailbox>) override;

private:
    std::unique_ptr<ThreadLifecycle> lifecycle;
    std::vector<std::thread> threads;
    std::queue<std::weak_ptr<Mailbox>> queue;
    std::mutex mutex;
    std::condition_variable cv;
    bool terminate { false };
};

} // namespace mbgl
