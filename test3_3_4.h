#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <future>
#include <atomic>


using namespace std;


class mSpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};


class mSingleton{
public:
    static mSingleton* getInstance(){
        mSingleton* singl= instance.load(std::memory_order_acquire);
        if ( !singl ){
            std::lock_guard<std::mutex> myLock(mMutex);
            singl= instance.load(std::memory_order_relaxed);
            if( !singl ){
                singl = new mSingleton();
                instance.store(singl, std::memory_order_release);
            }
        }
        return singl;
    }
private:
    mSingleton()= default;
    ~mSingleton()= default;
    mSingleton(const mSingleton&)= delete;
    mSingleton& operator=(const mSingleton&)= delete;

    static std::atomic<mSingleton*> instance;
    static std::mutex mMutex;
};