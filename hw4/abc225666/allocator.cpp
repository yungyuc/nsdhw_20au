#include <atomic>
#include <limits>
#include <iostream>
#include <new>
#include <vector>

using namespace std;

struct ByteCounterImpl {
    std::atomic_size_t allocated = 0;
    std::atomic_size_t deallocated = 0;
};

class ByteCounter {
public:
    ByteCounter()
      : counter(new ByteCounterImpl) {
    }

    ByteCounter(const ByteCounter& other) 
      : counter(other.counter) {
    }

    void increase(size_t amount) {
        counter->allocated += amount;
    }
    void decrease(size_t amount) {
        counter->deallocated += amount;
    }

    size_t bytes() {
        return counter->allocated - counter->deallocated;
    }

    size_t allocated() {
        return counter->allocated;
    }

    size_t deallocated() {
        return counter->deallocated;
    }
private:
    ByteCounterImpl* counter;
};

template<class T>
class CustomAllocator {
public:
    using value_type = T;

    CustomAllocator() = default;

    template<class U> constexpr
    CustomAllocator(const CustomAllocator<U>& other) {
        counter = other.counter;
    }

    T* allocate(size_t n) {
        if(n > numeric_limits<size_t>::max() / sizeof(T)) {
            throw bad_alloc();
        }
        const size_t bytes = n*sizeof(T);
        T* p = static_cast<T*>(malloc(bytes));
        if(p) {
            counter.increase(bytes);
            return p;
        }
        else {
            throw bad_alloc();
        }
    }

    void deallocate(T* p, size_t n) {
        free(p);
        const size_t bytes = n*sizeof(T);
        counter.decrease(bytes);
    }

    ByteCounter counter;
};
