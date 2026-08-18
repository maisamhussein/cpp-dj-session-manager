#ifndef POINTERWRAPPER_H
#define POINTERWRAPPER_H

#include <stdexcept> // ask ..


#include <utility>
#include <iostream>

/**
 * PointerWrapper - A template class that wraps a raw pointer
 * 
 * This is Phase 3 of the assignment. Students must analyze and implement
 * a complete pointer wrapper class that manages dynamic memory safely.
 * 
 * Refer to the assignment instructions (Phase 3) for detailed guiding questions
 * about resource management, ownership semantics, copy vs move, and interface design.
 */
template<typename T>
class PointerWrapper {
private:
    T* ptr;  // Raw pointer to the managed object

public:
    // ========== CONSTRUCTION AND DESTRUCTION ==========

    /**
     * Default constructor - creates empty wrapper
     */
    PointerWrapper() : ptr(nullptr) {}

    /**
     * Constructor from raw pointer - wraps the pointer
     */
    explicit PointerWrapper(T* p) : ptr(p) {}

    ~PointerWrapper()
     {
    delete ptr;
    ptr = nullptr;
    }
    // ========== COPY OPERATIONS (DELETED) ==========

    /**
     * Copy constructor is DELETED
     * Think about why this might be necessary for a pointer wrapper
     */
    PointerWrapper(const PointerWrapper& other) = delete;

    /**
     * Copy assignment is DELETED
     * Consider what problems could arise if copying was allowed
     */
    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    // ========== MOVE OPERATIONS (STUDENTS IMPLEMENT) ==========

 
    PointerWrapper(PointerWrapper&& other) noexcept 
    : ptr(other.ptr) 
    {
    //std::cout << "Move constructor called for: " << this << std::endl;
    other.ptr = nullptr;
    }
  
    PointerWrapper& operator=(PointerWrapper&& other) noexcept {
        if (this != &other)
         {
        delete ptr;        // משחררים מה שיש עכשיו
        ptr = other.ptr;   // גונבים את המצביע
        other.ptr = nullptr; // other נהיה ריק
         }
        return *this;
    }

    // ========== ACCESS OPERATIONS ==========

    T& operator*() const {
    if (!ptr) 
    {
        std::cerr << "[ERROR] Dereferencing null PointerWrapper" << std::endl;
    }
    return *ptr;    };

    T* operator->() const 
    {
     if (!ptr) {
        std::cerr << "[ERROR] Attempting to access member through null PointerWrapper" << std::endl;
    }
    return ptr;  
  }

    T* get() const 
    {
        if (!ptr) {
        std::cerr << "[ERROR] Calling get() on null PointerWrapper" << std::endl;
    }
    return ptr; 
   }

    // ========== OWNERSHIP MANAGEMENT ==========

    T* release() 
    {
    T* old_ptr = ptr;   // שמירת המצביע המקורי
    ptr = nullptr;      // ביטול הבעלות
    return old_ptr; 
    }    

    void reset(T* new_ptr = nullptr) {
        
    if (ptr == new_ptr) {
        return;
    }

    if (ptr) {
       // std::cout << "Reset called for pointer: " << ptr << std::endl;
        delete ptr;
    }

    ptr = new_ptr;
    }

    // ========== UTILITY FUNCTIONS ==========

    explicit operator bool() const {
       return ptr != nullptr;
    }

    /**
     * Swap two PointerWrapper objects
     * This is implemented for you as a reference
     */
    void swap(PointerWrapper& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// ========== NON-MEMBER FUNCTIONS ==========

/**
 * Helper function to create PointerWrapper
 * This is implemented for you as an example
 * Can you figure out when this would be useful in phase 4?
 */
template<typename T, typename... Args>
PointerWrapper<T> make_pointer_wrapper(Args&&... args) {
    return PointerWrapper<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
void swap(PointerWrapper<T>& lhs, PointerWrapper<T>& rhs) noexcept {
    lhs.swap(rhs);

}

#endif // POINTERWRAPPER_H