#include <string>
#include <future>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>

int factorial(int N)
{
    int res = 1;

    for (size_t i = N; i > 1; i--)
    {
        res *= i;
    }
    
    std::cout << "Result = " << res << std::endl;
    return res;
}


int main(int argc, const char* argv[])
{   
    // thread
    std::thread t1(factorial, 6); // creates a thread

    std::this_thread::sleep_for(std::chrono::milliseconds(3)); // main thread sleep for 3 milliseconds

    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::microseconds(4);
    std::this_thread::sleep_until(tp);

    // mutex = to synchronize data access
    std::mutex mu;
    std::lock_guard<std::mutex> locker(mu);
    std::unique_lock<std::mutex> uLocker(mu); // can lock/unlock mutex multiple times and also transfer ownership of mutex to another unique lock
    uLocker.try_lock(); // try lock mutex otherwise immediately return
   // uLocker.try_lock_for(std::chrono::nanoseconds(500)); // try and lock the mutex and if 500 ns passed and mutex still cant be locked then will return
   // uLocker.try_lock_until(tp); // try and lock until a certain time point


    // condition variable = synchronize execution order of threads
    std::condition_variable cond;
    cond.wait_for(uLocker, std::chrono::microseconds(2));
    cond.wait_until(uLocker, tp);
    // future and promise
    std::promise<int> p; // promise to send integer variable
    std::future<int> f = p.get_future();
    f.get();
    f.wait();
    f.wait_for(std::chrono::milliseconds(2));
    f.wait_until(tp);

    // async = another way to spawn a thread
    std::future<int> fu = std::async(factorial, 6); // spawn child thread or run in same thread (main thread)

    // packaged task
    std::packaged_task<int(int)> t(factorial);
    std::future<int> fu2 = t.get_future();

    t(6); // executes task

    return 0;
}











