//
// Created by real0 on 03.06.2022.
//

#ifndef OOP5_MYALLOCATOR_H
#define OOP5_MYALLOCATOR_H
#define SIZE 1024
#define COUNT 1024


template <uint32_t size, typename T>
struct Block {
    Block* next;
    T arr[size];
};

template<typename T, uint32_t Size = SIZE, uint32_t Count = COUNT>
class MyAllocator {
public:
    static const size_t byteCount = Size * sizeof(T);
    static const size_t pool_size = byteCount * Count;
    using value_type = T;
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
private:
    Block<byteCount, T>* blocks;
    Block<byteCount, T>** head_ptr;
    size_t* alloc_cnt;
public:
    pointer allocate(size_type cnt) {
        try {
            if (*head_ptr == nullptr) {
                throw std::invalid_argument("invalid");
            }
        } catch (const char* whatArg) {
            std::cout << "Error: " << whatArg;
        }
        pointer res = reinterpret_cast<pointer>(*head_ptr);
        *head_ptr = (*head_ptr)->next;
        return res;
    }

    void deallocate(pointer ptr, size_type cnt) {
        if(ptr) {
            auto *freePtr = reinterpret_cast<Block<byteCount, T>*>(ptr);
            freePtr->next = *head_ptr;
            *head_ptr = freePtr;
        }
    }
    MyAllocator() {
        if(Count == 0) {
            throw std::logic_error("this count's not allowed");
        }
        blocks = new Block<byteCount, T>[Count];
        head_ptr = new Block<byteCount, T>*;
        alloc_cnt = new size_type;
        *head_ptr = blocks;
        *alloc_cnt = 1;
        for (size_type i = 0; i < Count - 1; ++i) {
            blocks[i].next = blocks + i + 1;
        }
        blocks[Count - 1].next = nullptr;
    }
    ~MyAllocator() {
        if (*alloc_cnt == 1 && blocks != nullptr) {
            delete[] blocks;
            delete[] head_ptr;
            delete[] alloc_cnt;
        } else {
            blocks = nullptr;
            head_ptr = nullptr;
            alloc_cnt = nullptr;
        }
    }
    Block<byteCount, T>* Blocks() const{
        return blocks;
    }
    template<typename U>
    struct rebind {
        using other = MyAllocator<U, Size, Count>;
    };
    MyAllocator(const MyAllocator &other)
            : blocks(other.blocks),
              head_ptr(other.head_ptr),
              alloc_cnt(other.alloc_cnt) {
        std::cout << "constructor has been called\n";
        *alloc_cnt += 1;
    }
    bool operator==(const MyAllocator<T>& other) {
        return blocks == other.blocks;
    }
    bool operator!=(const MyAllocator<T>& other) {
        return !(*this == other);
    }
    MyAllocator& operator=(const MyAllocator<T>& other) {
        this->currentFreePtr = other.currentFreePtr;
        this->ptrCount = other.ptrCount;
        this->blocks_ = other.blocks_;
        return (*this);
    }
};


#endif //OOP5_MYALLOCATOR_H
