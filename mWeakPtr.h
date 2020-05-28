//
// Created by andy on 22.05.2020.
//

#ifndef FINALCHECKING_MWEAKPTR_H
#define FINALCHECKING_MWEAKPTR_H
#include <memory>



using namespace std;


template <typename T>
class m_sp_counted
{
public:
    explicit m_sp_counted(T *p): shared_count(1), weak_count(1), ptr(p) {}

    void add_ref()
    {
        ++shared_count;
    }

    void release()
    {
        if (!--shared_count)
        {
            delete ptr;
            if (!--weak_count)
            {
                destroy();
            }
        }
    }

    void add_ref_weak()
    {
        ++weak_count;
    }

    void release_weak()
    {
        if (!--weak_count)
        {
            destroy();
        }
    }

    size_t use_count() const
    {
        return shared_count;
    }



    void add_ref_lock()
    {
        if (shared_count == 0)
        {
            throw std::bad_weak_ptr();
        } else
        {
            add_ref();
        }

    }


private:
    void destroy()
    {
        delete this;
    }

private:
   size_t shared_count;
   size_t weak_count;
    T *ptr;
};

template <typename T>
class m_shared_ptr;

template <typename T>
class m_weak_ptr
{
    friend class m_shared_ptr<T>;
public:
    m_weak_ptr(): ptr(nullptr), counted(nullptr) {}

    m_weak_ptr(const m_weak_ptr &other): ptr(other.ptr), counted(other.counted)
    {
        add_ref_weak();
    }

    m_weak_ptr(const m_shared_ptr<T> &p): ptr(p.ptr), counted(p.counted)
    {
        add_ref_weak();
    }

    m_weak_ptr &operator=(const m_weak_ptr &other)
    {
        release_weak();
        this->ptr = other.ptr;
        this->counted = other.counted;
        add_ref_weak();

        return *this;
    }

    m_weak_ptr &operator=(const m_shared_ptr<T> &p)
    {
        release_weak();
        ptr = p.ptr;
        counted = p.counted;
        add_ref_weak();

        return *this;
    }


    m_shared_ptr<T> lock()
    {
        try
        {
            return m_shared_ptr<T>(*this);
        } catch (const std::bad_weak_ptr &)
        {
            return m_shared_ptr<T>();
        }
    }

    size_t use_count() const
    {
        return counted != nullptr ? counted->use_count() : 0;
    }

private:
    void add_ref_weak()
    {
        if (counted)
        {
            counted->add_ref_weak();
        }
    }

    void release_weak()
    {
        if (counted)
        {
            counted->release_weak();
        }
    }

private:
    T *ptr;
    m_sp_counted<T> *counted;
};

template <typename T>
class m_weak_ptr;

template <typename T>
class m_shared_ptr
{
    friend class weak_ptr<T>;
public:
    m_shared_ptr() : ptr(nullptr), counted(nullptr) {}


    explicit m_shared_ptr(T *p)
    {
        std::unique_ptr<T> holder(p);
        counted = new m_sp_counted<T>(holder.get());
        ptr = holder.release();
    }

    ~m_shared_ptr()
    {
        release();
    }

    m_shared_ptr(const m_shared_ptr<T> &other) : ptr(other.ptr), counted(other.counted)
    {
        add_ref();
    }

    m_shared_ptr(const m_weak_ptr<T> &p) : ptr(p.ptr), counted(p.counted)
    {
        if (counted)
        {
            counted->add_ref_lock();
        } else
        {
            throw std::bad_weak_ptr();
        }
    }

    m_shared_ptr &operator=(const m_shared_ptr<T> &other)
    {
        release();
        ptr = other.ptr;
        counted = other.counted;
        add_ref();

        return *this;
    }

    T *get() const
    {
        return ptr;
    }

    size_t use_count() const
    {
        return counted != nullptr ? counted->use_count() : 0;
    }
    T *ptr;
    m_sp_counted<T> *counted;

private:
    void add_ref()
    {
        if (counted)
        {
            counted->add_ref();
        }
    }

    void release()
    {
        if (counted)
        {
            counted->release();
        }
    }
};


#endif //FINALCHECKING_MWEAKPTR_H
