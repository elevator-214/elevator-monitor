#ifndef CHANNEL_H
#define CHANNEL_H
#include <condition_variable>
#include <mutex>
#include <deque>

// FIXME: double check
template <typename T> class channel
{
  public:
    channel(int buffer_size) :buffer_size(buffer_size) {}

    T get()
    {
        // std::lock_guard<std::mutex> _(mu);
        std::unique_lock<std::mutex> lk(mu);
        cv.wait(lk, [&]() { return buffer.size() > 0; });//返回true的时候，停止等待//lamuda表达式，参考https://zh.cppreference.com/w/cpp/language/lambda

        const T x = buffer.front();
        buffer.pop_front();

        // lk.unlock();
        cv.notify_all();

        return x;
    }

    void put(T x)
    {
        // std::lock_guard<std::mutex> _(mu);
        std::unique_lock<std::mutex> lk(mu);
//        cv.wait(lk, [&]() { return buffer.size() < buffer_size; });//为了读取视频
//        std::cout<<buffer_size<<std::endl;
        if(buffer.size()<buffer_size)
            buffer.push_back(x);
        else
        {

            buffer.clear();
            buffer.push_back(x);
        }

        // lk.unlock();
        cv.notify_all();
    }

  private:
    std::mutex mu;//互斥锁（英语：英语：Mutual exclusion，缩写 Mutex）是一种用于多线程编程中，防止两条线程同时对同一公共资源（比如全局变量）进行读写的机制。
    std::deque<T> buffer;
    const int buffer_size;
    std::condition_variable cv;//当 std::condition_variable 对象的某个 wait 函数被调用的时候，
    //它使用 std::unique_lock(通过 std::mutex) 来锁住当前线程。当前线程会一直被阻塞，直到另外一个线程在相同的 std::condition_variable 对象上调用了 notification 函数来唤醒当前线程。
};
#endif // CHANNEL_H

