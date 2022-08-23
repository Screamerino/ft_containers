#ifndef FT_CONTAINERS_VECTOR_HPP
# define FT_CONTAINERS_VECTOR_HPP
# include <memory>
# include <cstddef>
# include <stdexcept>
# include <limits>
# include <algorithm>
# include "algorithm.hpp"
# include "vector_iterator.hpp"
# include "type_traits.hpp"

namespace ft {
template< class T, class Alloc = std::allocator<T> >
class vector {
public:
    typedef T value_type;
    typedef Alloc allocator_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef reverse_vector_iterator<iterator> reverse_iterator;
    typedef reverse_vector_iterator<const_iterator> const_reverse_iterator;


    vector(): _size(0), _capacity(0), _array(NULL), _allocator(allocator_type()) {}
    explicit vector( const allocator_type & alloc ): _size(0), _capacity(0), _array(0), _allocator(alloc) {}

    explicit vector( size_type count,
                    const T& value = T(),
                    const allocator_type& alloc = allocator_type()): _size(count), _capacity(count) {
        _allocator = alloc;
        _array = _allocator.allocate(count);
        for (size_type i = 0; i < count; ++i)
            _allocator.construct(_array + i, value);
    }

    template< class InputIt >
    vector( InputIt first, InputIt last, const Alloc& alloc = Alloc(), typename enable_if<!is_integral<InputIt>::value, bool >::type = true ) {
        _allocator = alloc;
        _size = std::distance(first, last);
        _capacity = _size;
        _array = _allocator.allocate(_capacity);
        for (size_type i = 0; i < _size; ++i, ++first)
            _allocator.construct(_array + i, *first);
    }

    vector( const vector& other ): _size(other.size()), _capacity(other.capacity()) {
        _allocator = other.get_allocator();
        _array = _allocator.allocate(_capacity);
        for (size_type i = 0; i < _size; ++i)
            _allocator.construct(_array + i, other[i]);
    }

    ~vector() {
        _destroy_elements();
        _allocator.deallocate(_array, _capacity);
    }

    vector& operator=( const vector& other ){
        if (this != &other) {
            _destroy_elements();
            _allocator.deallocate(_array, _capacity);
            _allocator = other.get_allocator();
            _size = other.size();
            _capacity = other.capacity();
            _array = _allocator.allocate(_capacity);
            for (size_type i = 0; i < _size; ++i)
                _allocator.construct(_array + i, other[i]);
        }
        return *this;
    }

    size_type size() const { return _size; }
    size_type capacity() const { return _capacity; }
    allocator_type get_allocator() const { return _allocator; }

    void assign( size_type count, const T& value ) {
        _destroy_elements();
        if (count > _capacity)
            reserve(count);
        _size = count;
        for (size_type i = 0; i < _size; ++i)
            _allocator.construct(_array + i, value);
    }

//    typename enable_if<!is_integral<InputIt>::value, void >::type insert( iterator pos, InputIt first, InputIt last ) {

    template< class InputIt >
    typename enable_if<!is_integral<InputIt>::value, void >::type assign( InputIt first, InputIt last ) {
        _destroy_elements();
        size_t new_size = std::distance(first, last);
        if (new_size > _capacity)
            reserve(new_size);
        _size = new_size;
        for (size_t i = 0; i < _size; ++i)
            _allocator.construct(_array + i, first[i]);
    }

    void reserve( size_type new_cap ) {
        if (new_cap <= _capacity)
            return ;
        pointer newarr = _allocator.allocate(new_cap);
        size_t i = 0;
        try {
            for ( ; i < _size; ++i)
                _allocator.construct(newarr + i, _array[i]);
        }
        catch (...) {
            for (size_t j = 0; j < i; ++j)
                _allocator.destroy(newarr + i);
            _allocator.deallocate(newarr, new_cap);
            throw;
        }
        _destroy_elements();
        _allocator.deallocate(_array, _capacity);
        _array = newarr;
        _capacity = new_cap;
    }

    void resize( size_type count, T value = T() ) {
        if (count > _capacity)
            reserve(count);
        for (size_t i = _size; i < count; ++i)
            _allocator.construct(_array + i, value);
        if (count < _size) {
            _destroy_elements(count);
            _size = count;
        }
    }

    void push_back( const T& value ) {
        if (_capacity != 0 && _size == _capacity)
            reserve(2 * _capacity);
        if (_capacity == 0)
            reserve(1);
        _allocator.construct(_array + _size, value);
        ++_size;
    }

    void pop_back() {
        --_size;
        _allocator.destroy(_array + _size);
    }

    T& operator[]( size_type pos ) { return _array[pos]; }
    const T& operator[]( size_type pos ) const { return _array[pos]; }

    T& at( size_type pos ) {
        if (pos >= _size)
            throw std::out_of_range("Index out of range");
        return _array[pos];
    }
    const T& at( size_type pos ) const {
        if (pos >= _size)
            throw std::out_of_range("Index out of range");
        return _array[pos];
    }

    T& front() { return _array[0]; }
    const T& front() const { return _array[0]; }
    T& back() { return _array[_size - 1]; }
    const T& back() const { return _array[_size - 1]; }
    T* data() { return _array; }
    const T* data() const { return _array; }
    size_type max_size() const { return std::numeric_limits<difference_type>::max() / sizeof(value_type); }
    bool empty() const { return _size == 0; }

    void clear() {
        if (_size == 0)
            return ;
        _destroy_elements();
        _size = 0;
    }

    iterator begin() { return iterator(_array); }
    const_iterator begin() const { return const_iterator(_array); }
    iterator end() { return iterator(_array + _size); }
    const_iterator end() const { return const_iterator(_array + _size); }
    reverse_iterator rbegin() { return reverse_iterator(end() - 1); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end() - 1); }
    reverse_iterator rend() { return reverse_iterator(begin() - 1); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin() - 1); }

    iterator insert( iterator pos, const T& value ) {
        return _insert(pos, 1, value);
    }

    void insert( iterator pos, size_type count, const T& value) {
        _insert(pos, count, value);
    }

    iterator _insert( iterator pos, size_type count, const T& value) {
        if (_capacity != 0 && _size + count > _capacity) {
            size_type ncap = _capacity;
            if (_size + count > 2 * _capacity)
                ncap = _size + count;
            else
                ncap = 2 * _capacity;
            T *newarr = _allocator.allocate(ncap);
            T *end_arr;
            try {
                end_arr = _copy(begin(), pos, newarr);
                end_arr = _fill(end_arr, count, value);
                _copy(pos, end(), end_arr);
            } catch (...) {
                _destroy_temp_arr(newarr, end_arr);
                _allocator.deallocate(newarr, ncap);
                throw;
            }
            _destroy_elements();
            _allocator.deallocate(_array, _capacity);
            _array = newarr;
            _capacity = ncap;
            _size += count;
            return iterator(end_arr);
        }
        else if (end() - pos < static_cast<difference_type >(count)) {
            _copy(pos, end(), pos + count);
            difference_type index = end() - pos;
            try {
                _fill(_array + _size, count - index, value);
            }
            catch (...) {
                _destroy_temp_arr(pos + count, _array + _size + count);
                throw;
            }
            _size += count;
            _fill(pos, index, value);
        }
        else {
            iterator _end = end();
            _size += count;
            _copy(pos, _end, pos + count);
            _fill(pos, count, value);
        }
        return pos;
    }

    template< class InputIt >
    typename enable_if<!is_integral<InputIt>::value, void >::type insert( iterator pos, InputIt first, InputIt last ) {
        difference_type count = last - first;
        if (count + _size > _capacity) {
            size_type ncap = _capacity;
            if (_size + count > 2 * _capacity)
                ncap = _size + count;
            else
                ncap = 2 * _capacity;
            T* newarr = _allocator.allocate(ncap);
            T* endarr;
            try {
                endarr = _copy(begin(), pos, newarr);
                endarr = _copy(first, last, endarr);
                _copy(pos, end(), endarr);
            }
            catch (...) {
                _destroy_temp_arr(newarr, endarr);
                _allocator.deallocate(newarr, _size + ncap);
                throw;
            }
            _destroy_elements();
            _allocator.deallocate(_array, _capacity);
            _array = newarr;
            _capacity = ncap;
            _size += count;
        }
        else if (end() - pos < count) {
            _copy(pos, end(), pos + count);
            difference_type index = end() - pos;
            try {
//                _fill(_array + _size, count - index, value);
                _copy(first + index, last, _array + _size);
            }
            catch (...) {
                _destroy_temp_arr(pos + count, _array + _size + count);
                throw;
            }
            _size += count;
            _copy(first, last - (count - index), pos);
        }
        else {
            iterator _end = end();
            _size += count;
            _copy(pos, _end, pos + count);
            _copy(first, last, pos);
        }
    }

    iterator erase( iterator pos ) {
        iterator tmp = pos;
        return erase(pos, ++tmp);
    }

    iterator erase( iterator first, iterator last ) {
        if (first >= last)
            return last;
        else if (first == end())
            return end();
        size_type count_del = 0;
        size_type start = first - begin();
        for ( iterator it = first ; it != last ; ++it, ++count_del )
            _allocator.destroy(it);
        for ( size_type i = 0 ; i != _size - count_del - start; ++i, ++first )
            _allocator.construct(_array + start + i, _array[start + i + count_del]);
        _size -= count_del;
        return iterator(_array + start);
    }

    void swap( vector& other ) {
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
        std::swap(_array, other._array);
        std::swap(_allocator, other._allocator);
    }


private:
    size_t _size;
    size_t _capacity;
    T* _array;
    allocator_type _allocator;

    void _destroy_elements(size_type count = 0) {
        for (size_t i = count; i < _size; ++i)
            _allocator.destroy(_array + i);
    }

    void _destroy_temp_arr(T* start, T* end) {
        for ( ; start != end ; ++start)
            _allocator.destroy(start);
    }

    template<class It>
    T* _copy(It begin, It end, T* p) {
        T* ps = p;
        try {
            for ( ; begin != end ; ++begin, ++p)
                _allocator.construct(p, *begin);
        }
        catch (...) {
            _destroy_temp_arr(ps, p);
            throw;
        }
        return p;
    }

    T* _fill(T* p, size_type n, const T& value) {
        T* ps;
        try {
            for (; n != 0; --n, ++p)
                _allocator.construct(p, value);
        }
        catch (...) {
            _destroy_temp_arr(ps, p);
            throw;
        }
        return p;
    }

};

template< class T, class Alloc >
bool operator==( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs ) {
    return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template< class T, class Alloc >
bool operator!=( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs ) { return !(lhs == rhs); }

template< class T, class Alloc >
bool operator<( const ft::vector<T,Alloc>& lhs,
               const ft::vector<T,Alloc>& rhs ) {
    return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template< class T, class Alloc >
bool operator>( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs ) { return rhs < lhs; }

template< class T, class Alloc >
bool operator<=( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs ) { return !(rhs < lhs); }

template< class T, class Alloc >
bool operator>=( const ft::vector<T,Alloc>& lhs,
                const ft::vector<T,Alloc>& rhs ) { return !(lhs < rhs); }

template< class T, class Alloc >
void swap( ft::vector<T,Alloc>& lhs,
          ft::vector<T,Alloc>& rhs ) { return lhs.swap(rhs); }
}


#endif//FT_CONTAINERS_VECTOR_HPP
