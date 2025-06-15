//
// Created by weichuanqi on 2025/6/10.
//

#ifndef CLUCID_SMARTPOINT_H
#define CLUCID_SMARTPOINT_H

template<typename T>
class TOwningPtr {
    T* ptr;
public:
    explicit TOwningPtr(T* p = nullptr) : ptr(p) {}
    ~TOwningPtr() { delete ptr; }

    TOwningPtr(const TOwningPtr& other)
        : ptr(other.ptr ? new T(*other.ptr) : nullptr) {}

    TOwningPtr& operator=(const TOwningPtr& other) {
        if (this != &other) {
            if (ptr)
                delete ptr;
            ptr = other.ptr ? new T(*other.ptr) : nullptr;
        }
        return *this;
    }

    TOwningPtr(TOwningPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    TOwningPtr& operator=(TOwningPtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr; }
    [[nodiscard]] bool isValid() const { return ptr != nullptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};

#endif //CLUCID_SMARTPOINT_H
