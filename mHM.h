//
// Created by andy on 12/23/20.
//

#ifndef MHASHMAP_MHM_H
#define MHASHMAP_MHM_H


#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <shared_mutex>
#include <future>
#include <atomic>


using namespace std;



template <typename K, typename V, class TAllocator>
class HashNode
{
public:
    HashNode() : next(nullptr)
    {}

    HashNode(K key_, V value_) : next(nullptr)
    {
        item = std::allocator_traits<TAllocator>::allocate(alloc, 1);
        std::allocator_traits<TAllocator>::construct(alloc, item, make_pair(key_, value_));
    }

    ~HashNode()
    {
        std::allocator_traits<TAllocator>::destroy(alloc, item);
        std::allocator_traits<TAllocator>::deallocate(alloc, item, 1);

        next = nullptr;
    }

    const K& getKey() const {return item->first;}
    void setValue(V value_) {item->second = value_;}
    const V& getValue() const {return item->second;}

    std::pair<K, V>* GetItem()
    {
        return item;
    }

    V& GetVal()
    {
        return ref(item->second);
    }

    HashNode *next;
private:
    TAllocator alloc;
    std::pair<K, V> *item;
};


template <typename K, typename V, class TAllocator>
class HashBucket
{
public:
    HashBucket() : head(nullptr), itemCounter(0)
    {

    }

    ~HashBucket()
    {
        clear();
    }

    bool isExist()
    {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> * node = head;
        size_t counter = 0;


        while (node != nullptr && counter != itemCounter)
        {
            node = node->next;
            counter++;
        }

        if (node != nullptr && itemCounter != SizeOfBucket)
        {
            itemCounter++;
            return true;
        }

        return false;
    }

    V& GetItemByKey(K &key)
    {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> * node = head;
        V val;
        V& ret2 = ref(val);

        if (!find(key, val))
        {
            return ret2;
        }

        while (node != nullptr)
        {
            if (node->getKey() == key)
            {
                return node->GetVal();
            }
            node = node->next;
        }

        return ret2;
    }

    pair<K,V>* GetNextInBucket()
    {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);

        HashNode<K, V, TAllocator> * node = head;
        size_t counter = 0;
        pair<K,V> *ret = nullptr;

        while (node != nullptr && counter != (itemCounter-1))
        {
            node = node->next;
            counter++;
        }

        if (node != nullptr)
        {
            ret = node->GetItem();
        }

        return ret;
    }

    void InitIteration(size_t it)
    {
        itemCounter = it;
    }

    bool find(const K &key, V &value) const
    {

        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> * node = head;

        itemCounter = 0;
        while (node != nullptr)
        {

            if (node->getKey() == key)
            {
                value = node->getValue();
                return true;
            }
            node = node->next;
            itemCounter ++;
        }
        return false;
    }




    void insert(const K &key, const V &value)
    {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> * prev = nullptr;
        HashNode<K, V, TAllocator> * node = head;



        while (node != nullptr && node->getKey() != key)
        {
            prev = node;
            node = node->next;
        }

        if (nullptr == node)
        {
            if(nullptr == head)
            {
                head = new HashNode<K, V, TAllocator>(key, value);
            }
            else
            {
                prev->next = new HashNode<K, V, TAllocator>(key, value);
            }
            SizeOfBucket++;
        }
        else
        {
            node->setValue(value);
        }

    }


    void erase(const K &key)
    {

        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> *prev  = nullptr;
        HashNode<K, V, TAllocator> * node = head;

        while (node != nullptr && node->getKey() != key)
        {
            prev = node;
            node = node->next;
        }

        if (nullptr == node)
        {
            return;
        }
        else
        {
            if(head == node)
            {
                head = node->next;
            }
            else
            {
                prev->next = node->next;
            }
            delete node;
            SizeOfBucket--;
        }
    }


    void clear()
    {

        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        HashNode<K, V, TAllocator> * prev = nullptr;
        HashNode<K, V, TAllocator> * node = head;
        while(node != nullptr)
        {
            prev = node;
            node = node->next;
            delete prev;
        }
        head = nullptr;
        SizeOfBucket = 0;
    }

private:

    mutable size_t SizeOfBucket = 0;
    mutable size_t itemCounter = 0;



    HashNode<K, V, TAllocator> * head;
    mutable std::shared_timed_mutex mutex_;
};




//constexpr size_t HASH_SIZE_DEFAULT = 1031;
constexpr size_t HASH_SIZE_DEFAULT = 5;

template <typename K, typename V, class TAllocator >
class HashMap
{
public:


    class mIterator{
    private:
    public:
        mIterator( HashMap<K, V, TAllocator> *mapa = nullptr) : p(mapa){

    }

        bool operator!=(mIterator const& other) const
        {

        if (p->iterCounter >= p->hashSize )
        {
            p->IteratorBusy.store(false, memory_order_seq_cst);
        }
            return !(p->iterCounter >= p->hashSize );
        }
        pair<K,V> operator*() const
        {
            return item;
        }

        mIterator& operator++()
        {

            if (p)
            {

                p->GetNext();
                if (p->itemIter != nullptr)
                    item = make_pair(p->itemIter->first, p->itemIter->second);
            }
            return *this;
        }
    private:
        HashMap<K, V, TAllocator>* p;
        mutable pair<K, V> item;
    };

    HashMap(size_t hashSize_ = HASH_SIZE_DEFAULT) : hashSize(hashSize_)
    {
        hashTable = new HashBucket<K, V, TAllocator>[hashSize];
        IteratorBusy.store(false, memory_order_seq_cst);
    }

    ~HashMap()
    {
        delete [] hashTable;
    }

    HashMap(const HashMap&) = delete;
    HashMap(HashMap&&) = delete;
    HashMap& operator=(const HashMap&) = delete;
//    HashMap& operator=(HashMap&&) = delete;

    V& operator[](K && key)
    {
        size_t hashValue = hashFn(key) % hashSize ;
        return hashTable[hashValue].GetItemByKey(key);
    }

    HashMap& operator=(HashMap<K, V, TAllocator> & right)
    {
        this->clear();

        for(auto iter = right.begin(); iter != right.end(); ++iter)
        {
            this->insert( (*iter).first, (*iter).second ) ;
        }

        return *this;
    }

    void swap(HashMap<K, V, TAllocator> & other)
    {
        HashMap<K, V, TAllocator> buf;

        buf = *this;
        *this = other;
        other = buf;
    }


    bool find(const K &key, V &value) const
    {
        size_t hashValue = hashFn(key) % hashSize ;
        iterCounter.store(hashValue,memory_order_seq_cst);
        return hashTable[hashValue].find(key, value);
    }


    void insert(const K &key, const V &value)
    {
        size_t hashValue = hashFn(key) % hashSize ;
        hashTable[hashValue].insert(key, value);
        MapSize.store(MapSize.load(memory_order_seq_cst)+1, memory_order_seq_cst);
    }


    void erase(const K &key)
    {
        size_t hashValue = hashFn(key) % hashSize ;
        hashTable[hashValue].erase(key);
        MapSize.store(MapSize.load(memory_order_seq_cst)-1, memory_order_seq_cst);
    }


    void clear()
    {
        for(size_t i = 0; i < hashSize; i++)
        {
            (hashTable[i]).clear();
        }
        MapSize.store(0, memory_order_seq_cst);
    }

    void GetNext()
    {
        size_t i = 0;
        pair<K,V> *ret;
        itemIter = nullptr;

        for (i=iterCounter.load(memory_order_seq_cst); i<hashSize; ++i)
        {
            if ( (hashTable[i]).isExist() )
            {
                ret = (hashTable[i]).GetNextInBucket();
                itemIter = ret;
                return;
            }else
            {
                iterCounter.store(iterCounter.load(memory_order_seq_cst)+1, memory_order_seq_cst);

            }

            if (i >= (hashSize-1))
            {
                iterCounter.store(hashSize, memory_order_seq_cst);
                return;
            }
        }
    }

    mIterator find(const K &key)
    {
        mIterator iter;
        V value_;
        if(find(key, value_))
        {
            iter = mIterator(this);
            iter.operator++();
        }else
        {
            iter = mIterator();
        }

        return iter;
    }


    mIterator begin()
    {

        while (IteratorBusy.load(memory_order_seq_cst)){};

        IteratorBusy.store(true, memory_order_seq_cst);

        auto iter = mIterator(this);
//        iterCounter = 0;
        iterCounter.store(0, memory_order_seq_cst);
        for (size_t i=0; i<hashSize; ++i)
        {
            (hashTable[i]).InitIteration(0);
        }
        return iter.operator++();
    }

    mIterator end()
    {
        return mIterator();
    }

private:
    HashBucket<K, V, TAllocator> * hashTable;
    std::hash<K> hashFn;
    const size_t hashSize;
    mutable atomic<size_t> MapSize = 0;

    mutable atomic<size_t> iterCounter;
    mutable pair<K, V> *itemIter = nullptr;

    mutable atomic<bool> IteratorBusy;
};






#endif //MHASHMAP_MHM_H
