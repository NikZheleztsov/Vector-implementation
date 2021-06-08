#ifndef VECTOR_H
#define VECTOR_H
#include <cstdlib>
#include <limits>
#include <memory>
#include <stdexcept>
#include "iterator.h"

// exceptions
template <class T, class Allocator = std::allocator<T>>
class Vector 
{
    T* _data = nullptr;
    size_t _size = 0;
    size_t _cap = 0; // real size; capacity() !!! return _cap - 1
    Allocator _alloc = Allocator();

    void reallocate (bool is_full_copy = false, size_t new_cap = 0);

public:

    typedef T                                               value_type; 
    typedef Allocator                                       allocator_type;
    typedef size_t                                          size_type;
    typedef std::ptrdiff_t                                  difference_type;
    typedef value_type&                                     reference;
    typedef const value_type&                               const_reference;
    typedef typename 
        std::allocator_traits<Allocator>::pointer           pointer;
    typedef typename 
        std::allocator_traits<Allocator>::const_pointer     const_pointer;
    typedef Random_access_iterator<T>                       iterator;
    typedef const Random_access_iterator<T>                 const_iterator;
    typedef typename std::allocator_traits<Allocator>       alloc_traits;

    ////////////////////////
    //  Member functions  //
    ////////////////////////

    // Constructor
    Vector();
    explicit Vector (size_type count);
    Vector (const Vector& other); 
    Vector (Vector&& other);
    Vector(std::initializer_list<T> init);
    template <class InputIt> Vector( InputIt first, InputIt last);

    // Operator =
    Vector& operator=( const Vector& other);
    Vector& operator=( Vector&& other);
    Vector& operator=( std::initializer_list<T> ilist);

    // Iterators
    iterator begin() { return iterator(this->_data); }
    iterator end() { return iterator(this->_data + _size); }

    // Element access
    reference at( size_type pos);
    reference operator[]( size_type pos);
    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }
    T* data() noexcept { return _data; };

    // Capacity
    bool empty() const { return _size == 0; }
    size_type size() const { return _size; }
    size_type capacity () const { return _cap - 1; }
    size_type max_size() const;
    void reserve( size_type new_cap);
    void shrink_to_fit();

    // Modifiers
    void clear();

    iterator insert( iterator pos, const T& value);
    iterator insert( const_iterator pos, size_type count, const T& value);
    template <class InputIt, typename = // a little bit of cheating
        std::_RequireInputIter<InputIt>> 
        iterator insert( const_iterator pos,InputIt first, InputIt last);
    iterator insert( const_iterator pos, std::initializer_list<T> ilist);

    iterator erase( iterator pos);
    iterator erase( iterator first, iterator last);

    void push_back(const T& value);
    void pop_back();
    void resize( size_type count, T value = T());
    void swap( Vector& other); 

    // Destructor
    ~Vector();
};

#endif
