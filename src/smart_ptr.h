#ifndef _SMART_PTR
#define _SMART_PTR

#include <iostream>

namespace ministl {

template <typename T>
class unique_ptr {
public:
    // constructor
    unique_ptr() : m_obj(nullptr) {
        std::cout << "default constructor" << std::endl;
    }
    unique_ptr(T* ptr) : m_obj(ptr) {
        std::cout << "constructor: " << *ptr << std::endl;
    }
    
    // delete the default copy constructor function.
    unique_ptr(const unique_ptr<T>&) = delete;
    
    // deflete the default assignment constructor function.
    unique_ptr<T>& operator = (const unique_ptr<T>* rhs) = delete;

    // rhs means right-hand-side
    unique_ptr(unique_ptr<T>&& rhs) : m_obj(rhs.m_obj) {
        std::cout << "rhs called!" << std::endl;
        rhs.m_obj = nullptr;
    }

    // deconstructor
    ~unique_ptr() {
        if (m_obj != nullptr) {
            delete m_obj;
            m_obj = nullptr;
            std::cout << "deconstructor function called: object deleted!" << std::endl;
        }
    }

    void reset() {
        delete m_obj;
        m_obj = nullptr;
    }

    void reset(T* _data) {
        delete m_obj;
        m_obj = _data;
    }

    void release() noexcept {
        T* ret = m_obj;
        m_obj = nullptr;
        return ret;
    }

    T* operator -> () const {
        return &*this;
    }

    T& operator * () const {
        return *m_obj;
    }

    T* get() const noexcept {
        return m_obj;
    }


private:
    T* m_obj;
};

}

#endif
