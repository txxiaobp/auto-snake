#ifndef INDEX_HEAP_H
#define INDEX_HEAP_H

#include <iostream>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <cstring>
#include <ctime>

const int DEFAULT_LENGTH = 10;
const int INVALID_INDEX = -1;


template <typename T>
class IndexHeap
{
public:
    IndexHeap(int capacity = DEFAULT_LENGTH);
    ~IndexHeap();

    void operator=(const IndexHeap<T> &heap);
    int size();
    bool empty();

    void push(int index, T val);
    void change(int index, T val);
    T get(int index);
    bool in_heap(int index);
    T top();
    int top_index();
    void pop();

private:
    void shift_up(int index);
    void shift_down(int index);
    int parent(int index);
    int left_child(int index);
    int right_child(int index);
    void swap(int index1, int index2);

private:
    T *data;
    int *indexes;  // map from heap to data
    int *reverse;  // map from data to heap
    int sz;
    int capacity;
};


namespace IndexHeapTest
{
    bool sort(std::vector<int> &nums);
    void test();
}



template <typename T>
IndexHeap<T>::IndexHeap(int capacity)
    : data(new T[capacity])
    , indexes(new int[capacity])
    , reverse(new int[capacity])
    , sz(0)
    , capacity(capacity)
{
    assert(capacity > 0);
    for (int i = 0; i < capacity; i++)
    {
        indexes[i] = i;
        reverse[i] = INVALID_INDEX;
    }
}

template <typename T>
IndexHeap<T>::~IndexHeap()
{
    delete[] data;
    delete[] indexes;
    delete[] reverse;
}

template <typename T>
void IndexHeap<T>::operator=(const IndexHeap<T> &heap)
{
    delete[] data;
    delete[] indexes;
    delete[] reverse;

    sz = heap.sz;
    capacity = heap.capacity;

    data = new T[capacity];
    indexes = new T[capacity];
    reverse = new T[capacity];

    memcpy(data, heap.data, sizeof(T) * capacity);
    memcpy(indexes, heap.indexes, sizeof(T) * capacity);
    memcpy(reverse, heap.reverse, sizeof(T) * capacity);
}

template <typename T>
int IndexHeap<T>::size()
{
    return sz;
}

template <typename T>
bool IndexHeap<T>::empty()
{
    return sz == 0;
}

template <typename T>
void IndexHeap<T>::shift_up(int index)
{
    while (index > 0 && data[indexes[parent(index)]] > data[indexes[index]])
    {
        swap(parent(index), index);
        index = parent(index);
    }
}

template <typename T>
void IndexHeap<T>::shift_down(int index)
{
    while (left_child(index) < sz)
    {
        int tmp_index = left_child(index);
        int right_index = right_child(index);
        if (right_index < sz && data[indexes[tmp_index]] > data[indexes[right_index]])
        {
            tmp_index = right_index;
        }
        if (data[indexes[index]] > data[indexes[tmp_index]])
        {
            swap(index, tmp_index);
            index = tmp_index;
        }
        else
        {
            break;
        }
    }
}

template <typename T>
int IndexHeap<T>::parent(int index)
{
    return (index - 1) / 2;
}

template <typename T>
int IndexHeap<T>::left_child(int index)
{
    return 2 * index + 1;
}

template <typename T>
int IndexHeap<T>::right_child(int index)
{
    return 2 * index + 2;
}

template <typename T>
void IndexHeap<T>::swap(int index1, int index2)
{
    assert(index1 >= 0 && index1 < capacity);
    assert(index2 >= 0 && index2 < capacity);

    std::swap(indexes[index1], indexes[index2]);
    reverse[indexes[index1]] = index1;
    reverse[indexes[index2]] = index2;
}

template <typename T>
void IndexHeap<T>::push(int index, T val)
{
    if (in_heap(index))
    {
        return;
    }

    assert(index >= 0 && index < capacity);
    data[index] = val;
    reverse[index] = sz;
    indexes[sz] = index;
    shift_up(sz);
    sz++;
}

template <typename T>
void IndexHeap<T>::change(int index, T val)
{
    assert(in_heap(index));
    data[index] = val;
    int idx = reverse[index];
    shift_down(idx);
    shift_up(idx);
}

template <typename T>
T IndexHeap<T>::get(int index)
{
    return data[index];
}

template <typename T>
bool IndexHeap<T>::in_heap(int index)
{
    assert(index >= 0 && index < capacity);
    return reverse[index] != INVALID_INDEX;
}

template <typename T>
T IndexHeap<T>::top()
{
    assert(!empty());
    return data[indexes[0]];
}

template <typename T>
int IndexHeap<T>::top_index()
{
    assert(!empty());
    return indexes[0];
}

template <typename T>
void IndexHeap<T>::pop()
{
    assert(!empty());
    swap(0, --sz);
    reverse[indexes[sz]] = INVALID_INDEX;
    shift_down(0);
}

//void IndexHeapTest::test()
//{
//    int size = 1000;
//    int bound = 10 * size;
//    IndexHeap<int> heap(size);
//    int test_times = 1000;
//    int test_change_times = 10 * size;

//    srand(time(NULL));
//    for (int t = 0; t < test_times; t++)
//    {
//        for (int i = 0; i < size; i++)
//        {
//            heap.push(i, rand() % bound);
//        }
//        for (int i = 0; i < test_change_times; i++)
//        {
//            heap.change(rand() % size, rand() % bound);
//        }
//        std::vector<int> nums;
//        while (!heap.empty())
//        {
//            nums.push_back(heap.top());
//            heap.pop();
//        }
//        if (!sort(nums))
//        {
//            std::cout << "test error" << std::endl;
//            assert(false);
//        }
//        std::cout << "test " << t << " ok" << std::endl;
//    }
//    std::cout << "all test ok" << std::endl;
//}

//bool IndexHeapTest::sort(std::vector<int> &nums)
//{
//    int size = nums.size();
//    for (int i = 0; i < size - 1; i++)
//    {
//        if (nums[i] > nums[i + 1])
//        {
//            return false;
//        }
//    }
//    return true;
//}
#endif // INDEX_HEAP_H
