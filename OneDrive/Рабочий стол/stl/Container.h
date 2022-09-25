#pragma once
#include <iostream>
#include "Iterator.h"
#include "Exception.h"
template <typename DataType>
class Vector
{
private:
    DataType* vec_arr;
    DataType* vec_arr_end;
    DataType* vec_mem_end;

public:
    friend class VectorIterator<DataType>;
    using Iterator = VectorIterator<DataType>;

    Vector() : vec_arr(nullptr), vec_arr_end(nullptr), vec_mem_end(nullptr)
    {
    }

    explicit Vector(int size)
    {
        vec_arr = new DataType[size * 2];
        vec_arr_end = vec_arr + size;
        vec_mem_end = vec_arr + size * 2;
        for (int i = 0; i < size; ++i)
            vec_arr[i] = DataType();
    }

    Vector(int size, const DataType& value)
    {
        vec_arr = new DataType[size * 2];
        vec_arr_end = vec_arr + size;
        vec_mem_end = vec_arr + size * 2;
        for (int i = 0; i < size; ++i)
            vec_arr[i] = value;
    }

    Vector<DataType>(const std::initializer_list<DataType>& list)
    {
        vec_arr = new DataType[list.size() * 2];
        vec_arr_end = vec_arr + list.size();
        vec_mem_end = vec_arr + list.size() * 2;
        int i = 0;
        for (auto& elem : list)
        {
            vec_arr[i] = elem;
            ++i;
        }
    }

    template <typename IterType>
    Vector(IterType beg, IterType end)
    {
        vec_arr = new DataType[end.m_cur - beg.m_cur];
        vec_arr_end = vec_arr + (end.m_cur - beg.m_cur);
        vec_mem_end = vec_arr + (end.m_cur - beg.m_cur);
        int i = 0;
        while (beg != end)
        {
            vec_arr[i] = *beg;
            ++i;
            ++beg;
        }
    }

    Vector(const Vector& vec)
    {
        vec_arr = new DataType[vec.capacity()];
        vec_arr_end = vec_arr + vec.size();
        vec_mem_end = vec_arr + vec.capacity();
        for (int i = 0; i < size(); ++i)
            vec_arr[i] = vec.vec_arr[i];
    }

    Vector<DataType>(Vector<DataType>&& vec) :
        vec_arr(vec.arr), vec_arr_end(vec.vec_arr_end), vec_mem_end(vec.vec_mem_end)
    {
        vec.vec_arr = nullptr;
        vec.vec_arr_end = nullptr;
        vec.vec_mem_end = nullptr;
    }

    ~Vector()
    {
        delete[] vec_arr;
    }

    Vector<DataType>& operator=(const Vector<DataType>& vec)
    {
        if (this != &vec) {
            if (capacity() != vec.capacity())
            {
                delete[] vec_arr;
                vec_arr = new DataType[vec.capacity()];
            }
            vec_arr_end = vec_arr + vec.size();
            vec_mem_end = vec_arr + vec.capacity();
            for (int i = 0; i < capacity(); ++i)
                vec_arr[i] = vec.vec_arr[i];
        }
        return *this;
    }

    Vector& operator=(Vector&& vec)
    {
        if (this != &vec) {
            delete[] vec_arr;
            vec_arr = vec.vec_arr;
            vec.vec_arr = nullptr;
            vec_arr_end = vec.vec_arr_end;
            vec.vec_arr_end = nullptr;
            vec_mem_end = vec.vec_mem_end;
            vec.vec_mem_end = nullptr;
        }
        return *this;
    }

    DataType& operator[](int index)
    {
        return vec_arr[index];
    }

    const DataType& operator[](int index) const
    {
        return vec_arr[index];
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    Iterator end()
    {
        return Iterator(this, size());
    }

    bool empty() const
    {
        return vec_arr == vec_arr_end;
    }

    int size() const
    {
        return vec_arr_end - vec_arr;
    }

    int capacity() const
    {
        return vec_mem_end - vec_arr;
    }

    void clear()
    {
        for (int i = 0; i < size(); ++i)
            vec_arr[i] = DataType();
        vec_arr_end = vec_arr;
    }

    DataType& at(int index)
    {
        if ((index > size()) || (index < 0))
            throw OutOfRangeException("exception: at");
        return vec_arr[index];
    }

    const DataType& at(int index) const
    {
        if ((index > size()) || (index < 0))
            throw OutOfRangeException("exception: at(const)");
        return vec_arr[index];
    }

    void pushBack(const DataType& elem)
    {
        if (vec_arr == nullptr)
        {
            vec_arr = new DataType[2];
            vec_arr_end = vec_arr + 1;
            vec_mem_end = vec_arr + 2;
            vec_arr[0] = elem;
        }
        else
        {
            vec_arr[size()] = elem;
            ++vec_arr_end;
        }
        if (capacity() == size())
        {
            int s = size();
            int cap = capacity();
            DataType* tmp = new DataType[cap * 2];
            for (int i = 0; i < s; ++i)
                tmp[i] = vec_arr[i];
            delete[] vec_arr;
            vec_arr = tmp;
            vec_arr_end = vec_arr + s;
            vec_mem_end = vec_arr + cap * 2;
        }
    }

    void popBack()
    {
        if (size() != 0)
        {
            vec_arr[size() - 1] = DataType();
            --vec_arr_end;
        }
    }

    Iterator insert(Iterator pos, const DataType& elem)
    {
        if (this != pos.m_vec)
            throw IteratorException("exception: insert(pos, elem)");
        vec_arr_end = vec_arr + size() + 1;
        int current = pos.m_cur;
        for (int i = size() - 1; i > current; --i)
            vec_arr[i] = vec_arr[i - 1];
        vec_arr[current] = elem;
        return pos;
    }

    template <class IterType>
    Iterator insert(Iterator pos, IterType beg, IterType end)
    {
        if ((this != pos.m_vec) || (beg.m_vec != end.m_vec) || (beg.m_cur > end.m_cur))
            throw IteratorException("exception: insert(pos, beg, end)");
        int res = end.m_cur - beg.m_cur;
        DataType* new_arr = new DataType[res];
        for (int i = 0; i < res; ++i)
        {
            new_arr[i] = (DataType)beg;
            ++beg;
        }
        resize(size() + res);
        for (int i = size() - 1; i > pos.m_cur + res - 1; --i) {
            vec_arr[i] = vec_arr[i - res];
        }
        for (int i = pos.m_cur; i < pos.m_cur + res; ++i) {
            vec_arr[i] = new_arr[i - pos.m_cur];
        }
        return pos;
    }

    Iterator erase(Iterator pos)
    {
        for (int i = pos.m_cur; i < size() - 1; ++i)
            vec_arr[i] = vec_arr[i + 1];
        vec_arr[size() - 1] = DataType();
        --vec_arr_end;
        return pos;
    }

    void reserve(int num)
    {
        if (num <= 0)
            throw IteratorException("exception: reserve");
        int new_capacity = capacity() + num;
        int dimension = size();
        DataType* tmp = new DataType[new_capacity];
        for (int i = 0; i < size(); ++i)
            tmp[i] = vec_arr[i];
        delete[] vec_arr;
        vec_arr = tmp;
        vec_arr_end = vec_arr + dimension;
        vec_mem_end = vec_arr + new_capacity;
    }

    void resize(int num)
    {
        if (num < 0)
            throw IteratorException("exception: resize");
        if (vec_arr == nullptr)
        {
            vec_arr = new DataType[num * 2];
            vec_arr_end = vec_arr + num;
            vec_mem_end = vec_arr + num * 2;
            for (int i = 0; i < size(); ++i)
                vec_arr[i] = DataType();
        }
        else
        {
            int tmp_capacity = capacity();
            int tmp_size = size();
            if (num >= capacity())
                tmp_capacity *= 2;
            DataType* tmp = vec_arr;
            vec_arr = new DataType[tmp_capacity];
            vec_arr_end = vec_arr + num;
            vec_mem_end = vec_arr + tmp_capacity;
            for (int i = 0; i < tmp_size; ++i)
                vec_arr[i] = tmp[i];
            for (int i = tmp_size; i < num; ++i)
                vec_arr[i] = DataType();
            delete[] tmp;
        }
    }
};

