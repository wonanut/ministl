#ifndef _STRING
#define _STRING

namespace ministl {

#include <string.h>
#include <iostream>

class String {
public:
    String() : m_data(nullptr), m_len(0) {}
    String(const char* ptr) {
        std::cout << "construnctor 2" << std::endl;
        m_len = strlen(ptr);
        _copy_data(ptr);
    }

    String(const String& other) {
        if (other.m_data != nullptr) {
            std::cout << "copy construnctor" << std::endl;
            m_len = other.m_len;
            _delete_data();
            _copy_data(other.m_data);
        }
    }

    String& operator = (const String& other) {
        if (other.m_data != nullptr) {
            std::cout << "assignment construnctor" << std::endl;
            m_len = other.m_len;
            _delete_data();
            _copy_data(other.m_data);
        }
        return *this;
    }

    String(String&& other) {
        std::cout << "copy construnctor rhs" << std::endl;
        m_len = other.m_len;
        m_data = other.m_data;
        other.m_data = nullptr;
    }

    String& operator = (String&& other) {
        if (this != &other) {
            std::cout << "assignment construnctor rhs" << std::endl;
            m_len = other.m_len;
            m_data = other.m_data;
            other.m_data = nullptr;
        }
    }

    ~String() {
        _delete_data();
    }

    char* operator* () {
        return m_data;
    }

private:
    void _copy_data(const char* ptr) {
        if (ptr) {
            m_data = new char[m_len + 1];
            memcpy(m_data, ptr, m_len);
            m_data[m_len] = '\0';
        }
    }

    void _delete_data() {
        if (m_data != nullptr) {
            delete [] m_data;
            m_data = nullptr;
        }  
    }

private:
    char* m_data;
    size_t m_len;

};  // class String

} // namespace ministl

#endif
