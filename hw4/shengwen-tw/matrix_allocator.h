#include <cinttypes>
#include <limits>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <atomic>

using namespace std;

struct ByteCounterData {
    std::atomic_size_t n_allocated;
    std::atomic_size_t n_deallocated;
};

class ByteCounter {
public:
    ByteCounterData *data;

    ByteCounter(): data(0) {
        if(data == nullptr) {
            data = new ByteCounterData();
            data->n_allocated = 0;
            data->n_deallocated = 0;
        }
    }

    ~ByteCounter() {}
};

template <class T>
class MatrixDataAllocator
{
public:
    using value_type = T;

    template <class U>
    struct rebind
    {
        using other = MatrixDataAllocator<U>;
    };

    MatrixDataAllocator() = default;
    ~MatrixDataAllocator() = default;

    template<class U>
    bool operator!= (const MatrixDataAllocator<U> &rhs) {
        return false;
    }

    T* allocate(size_t n)
    {
        size_t n_byte = sizeof(T) * n;

        byte_counter.data->n_allocated += n_byte;

        //cout << "[debug] address of allocator = " << this << endl;
        cout << "[debug] allocated bytes = " << n_byte << endl;

        return static_cast<T*>(malloc(n_byte));
    }

    T* allocate(size_t n, const T * hint)
    {

        return allocate(n);
    }

    void deallocate(T* p, size_t n)
    {
        size_t n_byte = sizeof(T) * n;

        byte_counter.data->n_deallocated += n_byte;

        cout << "[debug] deallocated bytes = " << n_byte << endl;

        free(p);
    }

    size_t max_size() const
    {
        return std::numeric_limits<size_t>::max();
    }

    std::size_t bytes() {
        return byte_counter.data->n_allocated - byte_counter.data->n_deallocated;
    }

    std::size_t allocated() {
        return byte_counter.data->n_allocated;
    }

    std::size_t deallocated() {
        return byte_counter.data->n_deallocated;
    }


private:
    ByteCounter byte_counter;
};
