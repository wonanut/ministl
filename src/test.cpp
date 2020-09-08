#include "smart_ptr.h"
#include "t_string.h"
#include <iostream>
using namespace ministl;

ministl::String test_func() {
    ministl::String ret = "!23";
    return ret;
}

ministl::unique_ptr<int> get_unique_ptr() {
    std::cout << "----" << std::endl;
    ministl::unique_ptr<int> ret(new int(100));
    std::cout << "----" << std::endl;
    return ret;
}

int main() {
    {
        ministl::unique_ptr<int> uptr(new int(11));
        std::cout << *uptr << std::endl;
    }
    ministl::unique_ptr<int> uptr2(new int(12));
    ministl::unique_ptr<int> uptr3 = (ministl::unique_ptr<int>&&)uptr2; 
    // std::cout << *uptr2 << std::endl;
    std::cout << *uptr3 << std::endl;

    /*********** string test **************/
    // ministl::String str1 = "Hello!";
    // ministl::String str2(str1);

    // std::cout << *str1 << ", " << *str2 << std::endl;
    // ministl::String str3 = test_func();
    return 0;
}
