#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

using namespace std;

class MyRAIIThread
{
    thread &mTh;
public:
    MyRAIIThread(thread & th) : mTh(th)
    {
    }

    ~MyRAIIThread()
    {
        cout << endl << "~MyRAIIThread" << endl;
        if (mTh.joinable())
        {
            mTh.join();
        }
    }

    MyRAIIThread(const MyRAIIThread &)= delete;
    MyRAIIThread& operator=(const MyRAIIThread &)= delete ;

};

void Painer()
{
    unique_lock<mutex> lk(m);
    while (1) {
        isDoneDrying = false;
        cout << " hello im painer" << endl;
        this_thread::sleep_for(chrono::seconds(1));

        cv.notify_one();
        cv.wait(lk, []{return isDoneDrying == true;});
    }

}

void Dryer()
{
    unique_lock<std::mutex> lk(m);
    while (1) {
        cout << " hello im dryer" << endl;
        this_thread::sleep_for(chrono::seconds(3));
        isDoneDrying = true;

        cv.notify_one();
        cv.wait(lk, []{return isDoneDrying == false;} );
    }
}

void RAII_Presenter()
{
    {

        auto fff = [](){
            cout << endl << "Hello im thread" << endl;
        };

        std::thread t(fff);

        MyRAIIThread mTh( t );

        cout << endl << "here" << endl;
    }
    cout << endl << "end main" << endl;
	
}


void Painer_Dryer_Presenter()
{
    thread t1(Painer), t2(Dryer);
    t1.join();
    t2.join();
}