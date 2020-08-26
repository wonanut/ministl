#include "smart_ptr.h"
#include <iostream>
using namespace ministl;

int main() {
    ministl::unique_ptr<int> uptr(new int(11));
    return 0;
}
