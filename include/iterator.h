#ifndef ITERATOR_H
#define ITERATOR_H
#include <iterator>

template <class T>
class Random_access_iterator
{
    T* _pos = nullptr;

public:

    typedef Random_access_iterator Iter;
    typedef Random_access_iterator& Iter_ref;

    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    Random_access_iterator (pointer pos) : _pos(pos) {}

    bool operator==( Iter const& other) const { return (_pos == other._pos); }
    bool operator!=( Iter const& other) const { return (_pos != other._pos); }
    reference operator*() const { return *_pos; }
    pointer operator->() const { return _pos; };
    Iter operator++() { ++_pos; return *this; };
    Iter operator++(int) { auto tmp = *this; ++_pos; return tmp; }
    Iter operator--() { --_pos; return *this; };
    Iter operator--(int) { auto tmp = *this; --_pos; return tmp; }
    reference operator[](size_t& index) const { return _pos[index]; }
    Iter operator+( difference_type const& diff) { return Iter(_pos + diff); }
    friend Iter operator+( difference_type const& diff,
            Random_access_iterator const& it);
    Iter operator-( difference_type const& diff) const { return Iter(_pos - diff); }
    difference_type operator-(Iter const& other) const { return std::distance(other._pos,
            _pos); }
    Iter_ref operator+=(difference_type const& diff) { _pos += diff; return *this; }
    Iter_ref operator-=(difference_type const& diff) { _pos -= diff; return *this; }
    bool operator>(Iter const& other) const { return _pos > other._pos; }
    bool operator>=(Iter const& other) const { return _pos >= other._pos; }
    bool operator<(Iter const& other) const { return _pos < other._pos; }
    bool operator<=(Iter const& other) const { return _pos <= other._pos; }

};

template <class T>
inline Random_access_iterator<T> operator+( std::ptrdiff_t const& diff, 
        Random_access_iterator<T> const& it)
{ return Random_access_iterator(it._pos + diff); }

#endif
