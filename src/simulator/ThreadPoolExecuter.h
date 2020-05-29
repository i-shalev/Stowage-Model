//====================================================================
// a simple thread pool
// @ Amir Kirsh
//====================================================================

#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
#include<vector>
#include<optional>
#include<functional>
#include<chrono>

// helperer classes for named arguments
template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class NumTasks: public Named<int> {
    using Named<int>::Named;
};

class IterationsPerTask: public Named<int> {
    using Named<int>::Named;
};

class NumThreads: public Named<int> {
    using Named<int>::Named;
};

template<typename Producer>
class ThreadPoolExecuter {
    Producer producer;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running {false};
    std::atomic_bool stopped {false};
    static thread_local int num_tasks_finished;
    std::atomic_int total_num_tasks_finished { 0 };

    void worker_function() {
        while(!stopped) {
            auto task = producer.getTask();
            if(!task) break;
            (*task)();
            ++num_tasks_finished;
            ++total_num_tasks_finished;
        }
        if(stopped) {
            std::cout << std::this_thread::get_id() << " - stopped gracefully after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
        else {
            std::cout << std::this_thread::get_id() << " - finished after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
    }
public:
    ThreadPoolExecuter(Producer producer, NumThreads numThreads)
            : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }
    bool start() {
        bool running_status = false;
        // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
        if(!running.compare_exchange_strong(running_status, true)) {
            return false;
        }
        for(int i=0; i<numThreads; ++i) {
            workers.push_back(std::thread([this]{
                worker_function();
            }));
        }
        return true;
    }
    void stop_gracefully() {
        stopped = true;
        wait_till_finish();
    }
    void wait_till_finish() {
        for(auto& t : workers) {
            t.join();
        }
        std::cout << "thread pool finished/stopped after processing " << total_num_tasks_finished << " task(s)" << std::endl;
    }
};

// ThreadPoolExecuter.h - being a template - but outside of the class:
template<typename Producer>
thread_local int ThreadPoolExecuter<Producer>::num_tasks_finished { 0 };

// SimpleTasksProducer - you may want to separate it into .h and .cpp
class SimpleTasksProducer {
    const int numTasks = -1;
    const int iterationsPerTask = -1;
    std::atomic_int task_counter = 0;
    std::mutex m;

    std::optional<int> next_task_index() {
        for(int curr_counter = task_counter.load(); curr_counter < numTasks; ) {
            // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
            // note that in case compare_exchange_weak fails because the value of
            // task_counter != curr_counter than task_counter.load() is copied into curr_counter
            // in case of spurious failure (value not checked) curr_counter would not change
            if(task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
                return {curr_counter}; // zero based
            }
        }
        return {};
    }

    std::optional<int> next_task_index_simple() {
        // this is a more simple approach for getting the next task index
        // it will return each time an unused index in the range[0, numTasks)
        // the difference from the one above is that at certain point of time
        // the variable task_counter may exceed numTasks by any number between 1 and numThreads-1
        // however, we will not return such values
        if(task_counter < numTasks) {
            int next_counter = ++task_counter; // atomic operation
            // another thread could have increment task_counter after we passed the prev if
            // so we must check again - following check is on a local variable so no race
            if(next_counter <= numTasks) {
                return {next_counter - 1}; // zero based
            }
            else {
                // just so that at the end we will have: task_counter == numTasks
                // this is not critical in our scenario but might be in other scenarios
                --task_counter;
            }
        }
        return {};
    }

public:
    SimpleTasksProducer(NumTasks numTasks, IterationsPerTask iterations)
            : numTasks(numTasks), iterationsPerTask(iterations) {}
    SimpleTasksProducer(SimpleTasksProducer&& other)
            : numTasks(other.numTasks), iterationsPerTask(other.iterationsPerTask), task_counter(other.task_counter.load()) {}
    std::optional<std::function<void(void)>> getTask() {
        auto task_index = next_task_index(); // or: next_task_index_simple();
        if(task_index) {
            return [task_index, this]{
                for(int i=0; i<iterationsPerTask; ++i) {
                    std::lock_guard g{m};
                    std::cout << std::this_thread::get_id() << "-" << *task_index << ": " << i << std::endl;
                    std::this_thread::yield();
                }
            };
        }
        else return {};
    }
};

