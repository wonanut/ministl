#ifndef _MINISTL_HASH_TABLE
#define _MINISTL_HASH_TABLE

namespace ministl 
{

template <typename T>
    struct __hashtable_node
    {
        __hashtable_node* next;
        T val;

        __hashtable_node() = default;
        __hashtable_node(const T& t) : next(nullptr), val(t) {}

        __hashtable_node(const __hashtable_node& node) : next(node.next), val(node.val) {}
        __hashtable_node(__hashtable_node&& node) : next(node.next), val(ministl::move(node.val))
        {
            node.next = nullptr;
        }
    };

} // namespace ministl

#endif
