// doesn't care about data inside
#include <cstdlib>
#include <limits>
#include <memory>
#include <stdexcept>
#include "iterator.h"
#include "vector.h"

template <class T, class Allocator>
inline void Vector<T,Allocator>::reallocate (bool is_full_copy, size_t new_cap) // it's already cap + 1 // only for inside use
{
    Vector temp(*this);

    if (new_cap == 0)
    {
        if (_cap % 2 == 0)
            new_cap = 1.5 * _cap;
        else 
            new_cap = 1.5 * _cap + 1;
    }

    Vector::alloc_traits::deallocate(_alloc, _data, _cap);
    _data = std::allocator_traits<Allocator>::allocate(_alloc, new_cap);
    _cap = new_cap;

    if (_size > _cap - 1)
        _size = _cap - 1;

    if (is_full_copy)
        for (Vector::difference_type i = 0; i < temp._size && i < _size; i++)
            Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i]);
}

template <class T, class Allocator>
inline Vector<T, Allocator>::Vector() : _size(0), _cap(1)
{
    _data = std::allocator_traits<Allocator>::allocate(_alloc, 1);
}

template <class T, class Allocator>
inline Vector<T, Allocator>::Vector( size_type count) : _size(0), _cap(count + 1)
{
    _data = Vector::alloc_traits::allocate(_alloc, count + 1);
}

template <class T, class Allocator>
inline Vector<T, Allocator>::Vector (const Vector& other) : _size(other._size), _cap(other._cap)
{
    _data = Vector::alloc_traits::allocate(_alloc, other._cap);
    for (Vector::difference_type i = 0; i < other._size; i++)
        Vector::alloc_traits::construct(_alloc, _data + i, other._data[i]);
}

template <class T, class Allocator>
inline Vector<T, Allocator>::Vector (Vector&& other)
{
    _size = other._size; 
    _cap = other._cap;
    _data = other._data;
    other._data = nullptr;
}

template <class T, class Allocator>
inline Vector<T, Allocator>::Vector(std::initializer_list<T> init)
{
    _data = Vector::alloc_traits::allocate(_alloc, _size + 1);

    auto it = init.begin();
    for (Vector::difference_type i = 0; i < init.size(); i++, it++)
        Vector::alloc_traits::construct(_alloc, _data + i, *it);

    _size = init.size();
    _cap = init.size() + 1;
}

// if dist < 0 throw std::length err
template <class T, class Allocator> template <class InputIt> 
inline Vector<T, Allocator>::Vector( InputIt first, InputIt last)
{
    auto dist = last - first; // can't be < 0
    _data = Vector::alloc_traits::allocate(_alloc, _cap);

    if (dist < 0 || dist > max_size())
        throw std::length_error("cannot create std::vector larger than max_size()");

    for (Vector::difference_type i = 0; i < dist; ++i, ++first)
        Vector::alloc_traits::construct(_alloc, _data + i, *first);

    _size = std::abs(dist);
    _cap = std::abs(dist + 1);
}

template<class T, class Allocator>
inline Vector<T,Allocator>& Vector<T, Allocator>::operator=( const Vector& other)
{
    Vector::alloc_traits::deallocate(_alloc, _data, _cap);
    _data = Vector::alloc_traits::allocate(_alloc, other._cap);
    for (Vector::difference_type i = 0; i < _size; i++)
        Vector::alloc_traits::construct(_alloc, _data + i, other._data[i]);

    _size = other._size;
    _cap = other._cap;

    return *this;
}

template<class T, class Allocator>
inline Vector<T, Allocator>& Vector<T, Allocator>::operator=( Vector&& other)
{
    _size = other._size;
    _cap = other._cap;
    _data = other._data;
    other._data = nullptr;

    return *this;
}

template<class T, class Allocator>
inline Vector<T, Allocator>& Vector<T, Allocator>::operator=( std::initializer_list<T> ilist)
{
    Vector::alloc_traits::deallocate(_alloc, _data, _cap);
    _data = Vector::alloc_traits::allocate(_alloc, _size + 1);

    auto it = ilist.begin();
    for (Vector::difference_type i = 0; i < ilist.size(); i++, it++)
        Vector::alloc_traits::construct(_alloc, _data + i, *it);

    _size = ilist.size();
    _cap = ilist.size() + 1;

    return *this;
}

template<class T, class Allocator>
inline typename Vector<T, Allocator>::reference Vector<T, Allocator>::
        operator[]( Vector::size_type pos)
{
    return this->_data[pos];
}

template<class T, class Allocator>
inline typename Vector<T, Allocator>::reference Vector<T, Allocator>::at( Vector::size_type pos)
{
    if (pos >= _size)
        throw std::out_of_range("Pos value is more than container's size");

    return (*this)[pos];
}

template<class T, class Allocator>
inline typename Vector<T, Allocator>::size_type Vector<T, Allocator>::max_size() const
{
    const size_t dif_max = std::numeric_limits<std::ptrdiff_t>::max();
    const size_t aloc_max = Vector::alloc_traits::max_size(_alloc);
    return std::min(dif_max, aloc_max);
}

template<class T, class Allocator>
void Vector<T, Allocator>::reserve( Vector::size_type new_cap)
{
    if (new_cap > max_size())
        throw std::length_error("new_cap > max_size()");

    if (new_cap > _cap - 1)
        reallocate(true, new_cap + 1);

    _cap = new_cap + 1;
}

template <class T, class Allocator>
inline void Vector<T, Allocator>::clear()
{
    for (Vector::difference_type i = 0; i < _size; i++)
        Vector::alloc_traits::destroy(_alloc, _data + i);

    _size = 0;
    // doesn't change the capacity
}

template <class T, class Allocator>
void Vector<T, Allocator>::shrink_to_fit()
{
    if (_cap - 1 > _size)
        reallocate(true, _size + 1);
}

// inserts value before pos
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert( Vector::iterator pos, const T& value)
{
    Vector temp(*this);
    auto it = this->begin();
    if (_size + 1 > _cap - 1)
        reallocate(); // invalidating of iterators

    Vector::difference_type i = 0;
    for (; it < pos; ++i, ++it)
        Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i]);

    Vector::iterator ret(_data + i);
    Vector::alloc_traits::construct(_alloc, _data + i, value);

    ++i;
    for (; i < temp.size() + 1; i++)
        Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i - 1]);

    _size += 1;
    return ret;
}

// inserts count copies of the value before pos
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert( Vector::const_iterator pos,
        Vector::size_type count, const T& value)
{
    Vector temp(*this);
    auto it = this->begin();
    if (_size + count >= _cap - 1)
        reallocate(0, _size + count + 1); // invalidating

    Vector::difference_type i = 0;
    for (; it < pos; ++i, ++it)
        Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i]);

    Vector::size_type j = 0;
    Vector::iterator ret(_data + i + j);
    for (; j < count; j++)
        Vector::alloc_traits::construct(_alloc, _data + i + j, value);

    for (; i < temp.size() + 1; i++)
        Vector::alloc_traits::construct(_alloc, _data + i + j, temp._data[i]);

    _size += count;
    return ret;
}

// ambiguity with overload !!!
// inserts elements from range [first, last) before pos
// According to cppref the behavior is undefined if first and last are iterators into *this
// STL vector throw except if len < 0
template <class T, class Allocator> template <class InputIt, typename> 
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert( 
        Vector::const_iterator pos, InputIt first, InputIt last)
{
    Vector temp(*this);
    std::ptrdiff_t len = last - first;
    if (len < 0 || len > max_size())
        throw std::length_error("vector::_M_range_insert");

    auto it = this->begin();
    if (_size + len >= _cap - 1)
        reallocate(0, _size + len + 1);

    Vector::difference_type i = 0, j = 0;

    for (; it < pos; ++i, ++it)
        Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i]);

    Vector::iterator ret(_data + i + j);

    for (; j < len; ++j, ++first)
        Vector::alloc_traits::construct(_alloc, _data + i + j, *first);

    for (; i < temp.size() + 1; i++)
        Vector::alloc_traits::construct(_alloc, _data + i + j, temp._data[i - 1]);

    _size += len;
    return ret;
}

// inserts elements from initializer list before pos
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert( 
        Vector::const_iterator pos, std::initializer_list<T> ilist)
{
    Vector temp(*this);
    auto it = this->begin();
    if (_size + ilist.size() >= _cap - 1)
        reallocate(0, _size + ilist.size() + 1);

    Vector::difference_type i = 0, j = 0;
    for (; it < pos; ++i, ++it)
        Vector::alloc_traits::construct(_alloc, _data + i, temp._data[i]);

    Vector::iterator ret(_data + i + j);
    auto it_list = ilist.begin();
    for (; j < ilist.size(); j++, it_list++)
        Vector::alloc_traits::construct(_alloc, _data + i + j, *it_list);

    for (; i < temp.size() + 1; i++)
        Vector::alloc_traits::construct(_alloc, _data + i + j, temp._data[i]);

    _size += ilist.size();
    return ret;
}

/*
    iterator erase( iterator pos);
    iterator erase( iterator first, iterator last);
    */

template <class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase( 
        typename Vector<T, Allocator>::iterator pos)
{
    Vector::difference_type diff = pos - begin();
    for (int i = 0; pos < end(); pos++, i++)
        Vector::alloc_traits::construct(_alloc, _data + diff + i, *(_data + diff + i + 1));
    _size--;

    return end();
}

// [first, last)
template <class T, class Allocator>
typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase( 
        typename Vector<T, Allocator>::iterator first, typename Vector<T, Allocator>::iterator last)
{
    if (empty())
        return last;

    auto f_pos = first - begin();
    auto dif = last - first;

    for (int i = 0; first < last; ++first, i++)
        Vector::alloc_traits::construct(_alloc, &(*first), *(_data + f_pos + dif + i));

    _size -= dif;
    return end();
}

template <class T, class Allocator>
void Vector<T, Allocator>::push_back(const T& value)
{
    if (_size + 1 > _cap - 1)
        reallocate(true);

    Vector::alloc_traits::construct(_alloc, _data + _size, value);
    _size++;
}

template <class T, class Allocator>
inline void Vector<T, Allocator>::pop_back()
{
    Vector::alloc_traits::destroy(_alloc, _data + _size - 1);
    _size--;
}

template <class T, class Allocator>
void Vector<T, Allocator>::resize( Vector::size_type count, T value)
{
    if (count > _size)
    {
        // default append
        if (count > _cap - 1)
            reallocate(true ,count + 1);

        for (size_t j = 0; j < count; j++)
            Vector::alloc_traits::construct(_alloc, _data + _size + j, value);
        
    } else {
        // erase at the end
        for (Vector::difference_type i = 0; i < _size - count; i++)
            Vector::alloc_traits::destroy(_alloc, _data + _size - 1 - i);
    }

    _size = count;
}

template <class T, class Allocator>
inline void Vector<T, Allocator>::swap( Vector& other)
{
    Vector other_temp = other;
    Vector this_temp = *this;

    other = std::move(this_temp);
    *this = std::move(other_temp);
}

template <class T, class Allocator>
inline Vector<T, Allocator>::~Vector()
{
    Vector::alloc_traits::deallocate(_alloc, _data, _cap);
}

