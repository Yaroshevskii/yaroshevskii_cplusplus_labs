#include <cstdlib>
#include <new>
#include <gtest/gtest.h>
#include <limits>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <shared_mutex>
#include "schedulePresenter.h"
#include "mIter.h"
#include "mHM.h"


using namespace std;



template <class T>
struct Mallocator
{
    typedef T value_type;

    Mallocator () = default;
    template <class U> constexpr Mallocator (const Mallocator <U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();

        if (auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) {
            report(p, n);
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept {
        report(p, n, 0);
        std::free(p);
    }

private:
    void report(T* p, std::size_t n, bool alloc = true) const {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T)*n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }
};

template <class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
template <class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }


size_t mem_usage;
template <typename T>
class MyAllocator : public std::allocator<T>{
private:
    using Base = std::allocator<T>;
    using Pointer = typename std::allocator_traits<Base>::pointer;
    using Size = typename std::allocator_traits<Base>::size_type;
public:
    MyAllocator() = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>& alloc) : Base(alloc){}

    Pointer allocate(Size b)
    {
        mem_usage = sizeof(T) * b;
        std::cout << mem_usage << std::endl;
        return Base::allocate(b);
    }

    template <class _Other>
    struct rebind {
        using other = MyAllocator<_Other>;
    };

    void deallocate(Pointer p, Size s)
    {
        std::cout << "dealloc" << std::endl;
        Base::deallocate(p, s);
    }
};



TEST(check, test1)
{
    HashMap<std::string, int, std::allocator<pair<string, int> > > stringMap(HASH_SIZE_DEFAULT);
//    HashMap<std::string, int, Mallocator< pair<string, int> > > stringMap(HASH_SIZE_DEFAULT);
    unordered_map<string, int> stringMap2(HASH_SIZE_DEFAULT);


    stringMap.insert("test1", 200);
    stringMap.insert("test2", 300);


    stringMap2["test1"] = 200;
    stringMap2["test2"] = 300;


    size_t N = 1000;

    auto f2 = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(9));
            stringMap.insert("test6", 999);
            stringMap2["test6"] = 999;
        }
//            cout << " exit f2" << endl;
    };

    auto f3 = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(11));
            stringMap.erase("test6");
            stringMap2.erase("test6");
        }
//            cout << " exit f3" << endl;
    };

    auto f = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(10));
            if (i == N - 1) {
//                    cout << " Thread = " << index << " i = " << i << endl;

                for (auto iter = stringMap.begin(); iter != stringMap.end(); ++iter) {
//                        cout << "th= " << index << " ret = " << (*iter).first << " " << (*iter).second << endl;

                    EXPECT_EQ( (*iter).second == stringMap2[(*iter).first], true );
                }
            }
        }
    };

    auto ff = [&](int index) {
        for (size_t i = 0; i < N; i++) {
            this_thread::sleep_for(chrono::milliseconds(10));
            if (i == N - 1) {
//                    cout << " Thread = " << index << " i = " << i << endl;
                for (auto iter = stringMap.begin(); iter != stringMap.end(); ++iter) {
//                        cout << "th= " << index << " ret = " << (*iter).first << " " << (*iter).second << endl;

                    EXPECT_EQ( (*iter).second == stringMap2[(*iter).first], true );
                }
            }
        }
    };

    thread t1(ref(f), 1);
    thread t2(ref(ff), 2);

    thread t3(ref(f2), 3);
    thread t4(ref(f3), 4);



    t1.join();
    t2.join();


    t3.join();
    t4.join();
}

TEST(check, test2)
{
    HashMap<std::string, int, std::allocator<pair<string, int> > > stringMap(HASH_SIZE_DEFAULT);
//    HashMap<std::string, int, Mallocator< pair<string, int> > > stringMap(HASH_SIZE_DEFAULT);
    unordered_map<string, int> stringMap2(HASH_SIZE_DEFAULT);


    stringMap.insert("test1", 200);
    stringMap.insert("test2", 300);


    stringMap2["test1"] = 200;
    stringMap2["test2"] = 300;


    size_t N = 1000;

    auto f2 = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(11));
            stringMap.insert("test6", 999);
            stringMap2["test6"] = 999;
        }
//        cout << " exit f2" << endl;
    };

    auto f3 = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(9));
            stringMap.erase("test6");
            stringMap2.erase("test6");
        }
//        cout << " exit f3" << endl;
    };

    auto f = [&](int index) {
        for (size_t i = 0; i < N; i++) {

            this_thread::sleep_for(chrono::milliseconds(10));
            if (i == N - 1) {
//                cout << " Thread = " << index << " i = " << i << endl;

                for (auto iter = stringMap.begin(); iter != stringMap.end(); ++iter) {
//                    cout << "th= " << index << " ret = " << (*iter).first << " " << (*iter).second << endl;

                    EXPECT_EQ( (*iter).second == stringMap2[(*iter).first], true );
                }
            }
        }
    };

    auto ff = [&](int index) {
        for (size_t i = 0; i < N; i++) {
            this_thread::sleep_for(chrono::milliseconds(10));
            if (i == N - 1) {
//                cout << " Thread = " << index << " i = " << i << endl;
                for (auto iter = stringMap.begin(); iter != stringMap.end(); ++iter) {
//                    cout << "th= " << index << " ret = " << (*iter).first << " " << (*iter).second << endl;

                    EXPECT_EQ( (*iter).second == stringMap2[(*iter).first], true );
                }
            }
        }
    };

    thread t1(ref(f), 1);
    thread t2(ref(ff), 2);

    thread t3(ref(f2), 3);
    thread t4(ref(f3), 4);



    t1.join();
    t2.join();


    t3.join();
    t4.join();
}

TEST(check, test3)
{
    HashMap<std::string, int, std::allocator<pair<string, int> > > stringMap(HASH_SIZE_DEFAULT);
    HashMap<std::string, int, std::allocator<pair<string, int> > > stringMap2(HASH_SIZE_DEFAULT);

    stringMap.insert("test1", 1);

    EXPECT_EQ( stringMap["test1"] == 1, true );

    stringMap["test1"] = 10;

    EXPECT_EQ( stringMap["test1"] == 10, true );

    stringMap.insert("test2", 2);

    stringMap2.insert("test10", 20);
    stringMap2.insert("test20", 30);

    stringMap.swap(stringMap2);

    EXPECT_EQ( stringMap["test10"] == 20, true );
    EXPECT_EQ( stringMap["test20"] == 30, true );

    EXPECT_EQ( stringMap2["test1"] == 10, true );
    EXPECT_EQ( stringMap2["test2"] == 2, true );

}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
