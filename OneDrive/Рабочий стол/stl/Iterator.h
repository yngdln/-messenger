#pragma once
#include "Container.h"
#include "Exception.h"


template<typename DataType>
class Vector;

template<typename DataType>
class VectorIterator
{
private:
    Vector<DataType>* m_vec;
    int m_cur;
public:

    friend class Vector<DataType>;
    using Vector = Vector<DataType>;

    VectorIterator(Vector* vec, int cur) : m_vec(vec), m_cur(cur)
    {}

    VectorIterator(const VectorIterator& it) : m_vec(it.m_vec), m_cur(it.m_cur)
    {}

    ~VectorIterator()
    {
        m_vec = nullptr;
    }

    VectorIterator& operator++()
    {
        if ((m_cur + 1) >= m_vec->capacity())
            throw OutOfRangeException("Iterator::operator++");
        ++m_cur;
        return *this;
    }

    VectorIterator operator++(int)
    {
        if ((m_cur + 1) >= m_vec->capacity())
            throw OutOfRangeException("Iterator::operator++(int)");
        VectorIterator It(*this);
        ++m_cur;
        return It;
    }

    VectorIterator& operator--()
    {
        if (m_cur < 1)
            throw OutOfRangeException("Iterator::operator--");
        --m_cur;
        return *this;
    }

    VectorIterator operator--(int)
    {
        if (m_cur < 1)
            throw OutOfRangeException("Iterator::operator--(int)");
        VectorIterator It(*this);
        --m_cur;
        return It;
    }

    VectorIterator& operator=(const VectorIterator& it)
    {
        if (this != &it) {
            m_vec = it.m_vec;
            m_cur = it.m_cur;
        }
        return *this;
    }

    VectorIterator operator+(const int n)
    {
        if ((m_cur + n) >= m_vec->capacity())
            throw OutOfRangeException("Iterator::operator+(int n)");
        VectorIterator It(*this);
        It.m_cur += n;
        return It;
    }

    VectorIterator operator-(const int n)
    {
        if (m_cur < n)
            throw OutOfRangeException("Iterator::operator-(int n)");
        VectorIterator It(*this);
        It.m_cur -= n;
        return It;
    }

    VectorIterator operator[](int index)
    {
        if ((index < 0) || ((m_cur + index) >= m_vec->capacity()))
            throw OutOfRangeException("Iterator::operator[]");
        VectorIterator It(*this);
        It.m_cur += index;
        return It;
    }

    bool operator==(const VectorIterator& it) const
    {
        return (m_vec == it.m_vec) && (m_cur == it.m_cur);
    }

    bool operator!=(const VectorIterator& it) const
    {
        return !(*this == it);
    }

    bool operator>(const VectorIterator& it) const
    {
        if (m_vec != it.m_vec)
            throw IteratorException("Iterator::operator>");
        return m_cur > it.m_cur;
    }

    bool operator<(const VectorIterator& it) const
    {
        if (m_vec != it.m_vec)
            throw IteratorException("Iterator::operator<");
        return m_cur < it.m_cur;
    }

    bool operator>=(const VectorIterator& it) const
    {
        return (m_cur > it.m_cur) || (m_cur = it.m_cur);
    }

    bool operator<=(const VectorIterator& it) const
    {
        return (m_cur < it.m_cur) || (m_cur = it.m_cur);
    }

    VectorIterator& operator+=(const int n)
    {
        if (m_vec->capacity() <= (m_cur + n))
            throw OutOfRangeException("Iterator::operator+=");
        m_cur += n;
        return *this;
    }

    VectorIterator& operator-=(const int n)
    {
        if ((m_vec->capacity()) <= (m_cur + n))
            throw OutOfRangeException("Iterator::operator-=");
        m_cur -= n;
        return *this;
    }

    DataType* operator->() const
    {
        return &(m_vec->m_arr[m_cur]);
    }

    DataType& operator*() const
    {
        return m_vec->at(m_cur);
    }

    explicit operator DataType()
    {
        return m_vec->at(m_cur);
    }
};

