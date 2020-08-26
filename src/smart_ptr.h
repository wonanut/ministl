<<<<<<< HEAD
#ifndef _T_MEMORY
#define _T_MEMORY

namespace ministl {

template <typename T>
class unique_ptr {
public:
    typedef T* pointer;
	unique_ptr() : m_obj(nullptr) {}

    unique_ptr(T* p) : m_obj(p) {}

	// delete the default copy constructor
	unique_ptr(const unique_ptr<T>&) = delete;
	
	unique_ptr(unique_ptr<T>&& rhs) : m_obj(rhs.m_obj) {
	    rhs.m_obj = nullptr;
	}
    
	// delete the default assignment constructor
	unique_ptr<T>& operator = (const unique_ptr<T>& rhs) = delete;

	~unique_ptr() {
	    delete m_obj;
	}

	void reset() {
	    delete m_obj;
		m_obj = nullptr;
	}

	void reset(pointer _obj) {
	    delete m_obj;
		m_obj = _obj;
	}

	pointer release() noexcept {
	    pointer ret = m_obj;
		m_obj = nullptr;
		return ret;
	}

	pointer get() const noexcept {
	    return m_obj;
	}

	T& operator * () const noexcept {
	    return *m_obj;
	}

    T& operator -> () const noexcept {
	    return &*this;
	}
private:
    T* m_obj;
};

=======
#ifndef _SMART_PTR
#define _SMART_PTR

namespace ministl {

>>>>>>> c2c3538ebff4520eb7f8a1ce2bd821e10224bae6
}

#endif
