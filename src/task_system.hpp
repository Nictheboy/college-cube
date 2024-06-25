#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

template <typename TTask>
class IEnqueuer {
   public:
    virtual ~IEnqueuer() {}
    virtual void Enqueue(TTask task) = 0;
};

template <typename TTask>
class IHandler {
   public:
    virtual ~IHandler() {}
    virtual void Handle(TTask& task, IEnqueuer<TTask>& enqueuer) = 0;
};

template <typename TTask>
class IHandleQueueSystem {
   public:
    virtual ~IHandleQueueSystem() {}
    virtual void Solve(TTask& initial_task, IHandler<TTask>& handler) = 0;
};

template <typename TTask>
class HandleQueueSystemSingleThread : public IHandleQueueSystem<TTask> {
   private:
    class Enqueuer : public IEnqueuer<TTask> {
       private:
        std::queue<TTask>& queue;

       public:
        Enqueuer(std::queue<TTask>& queue)
            : queue(queue) {}
        void Enqueue(TTask task) override { queue.push(task); }
    };

   public:
    void Solve(TTask& initial_task, IHandler<TTask>& handler) override {
        std::queue<TTask> queue;
        Enqueuer enqueuer(queue);
        enqueuer.Enqueue(initial_task);
        while (!queue.empty()) {
            TTask task = queue.front();
            queue.pop();
            handler.Handle(task, enqueuer);
        }
    }
};

template <typename TTask>
class HandleQueueSystemMultiThread : public IHandleQueueSystem<TTask> {
    const int concurrency;

   private:
    class Enqueuer : public IEnqueuer<TTask> {
       private:
        std::queue<TTask>& queue;
        std::mutex& mutex;
        std::condition_variable& condition_variable;

       public:
        Enqueuer(std::queue<TTask>& queue, std::mutex& mutex, std::condition_variable& condition_variable)
            : queue(queue), mutex(mutex), condition_variable(condition_variable) {}
        void Enqueue(TTask task) override {
            std::lock_guard lock(mutex);
            queue.push(task);
            condition_variable.notify_all();
        }
    };

   public:
    HandleQueueSystemMultiThread(int concurrency = std::thread::hardware_concurrency())
        : concurrency(concurrency) {}

    void Solve(TTask& initial_task, IHandler<TTask>& handler) override {
        std::queue<TTask> queue;
        std::mutex mutex;
        std::condition_variable condition_variable;
        std::atomic_int run_counter = 0;
        Enqueuer enqueuer(queue, mutex, condition_variable);
        enqueuer.Enqueue(initial_task);
        std::unique_ptr<std::unique_ptr<std::thread>[]> thread_pool =
            std::make_unique<std::unique_ptr<std::thread>[]>(concurrency);
        for (int i = 0; i < concurrency; i++) {
            thread_pool[i] = std::make_unique<std::thread>([&] {
                while (true) {
                    TTask task;
                    {
                        std::unique_lock lock(mutex);
                        bool request_quit = false;
                        while (queue.empty()) {
                            if (run_counter == 0) {
                                request_quit = true;
                                break;
                            }
                            condition_variable.wait(lock);
                        }
                        if (request_quit)
                            break;
                        task = queue.front();
                        // printf("%d\n", queue.size());
                        queue.pop();
                        run_counter++;
                    }
                    try {
                        handler.Handle(task, enqueuer);
                    } catch (...) {
                        printf(
                            "HandleQueueSystemMultiThread<CubeTask*>::Solve(CubeTask*&, IHandler<CubeTask*>&): "
                            "Exception caught\n");
                    }
                    run_counter--;
                    condition_variable.notify_all();
                }
            });
        }
        for (int i = 0; i < concurrency; i++) {
            thread_pool[i]->join();
        }
    }
};
