#ifndef FT_CONTAINERS_VECTOR_ITERATOR_HPP
# define FT_CONTAINERS_VECTOR_ITERATOR_HPP
#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft {

    template<class Iter>
    class vector_iterator: public ft::iterator<std::random_access_iterator_tag, typename iterator_traits<Iter>::value_type >  {
    public:

        typedef typename iterator_traits<Iter>::value_type value_type;
        typedef typename iterator_traits<Iter>::difference_type difference_type;
        typedef typename iterator_traits<Iter>::pointer pointer;
        typedef typename iterator_traits<Iter>::reference reference;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;
        typedef Iter iterator_type;

        vector_iterator(): _iter(iterator_type()) {}
        explicit vector_iterator( iterator_type it ): _iter(it) {}

        template <class It>
        vector_iterator( const vector_iterator<It> & other ): _iter(other.base()) {}


        ~vector_iterator() {}
        vector_iterator & operator=(const vector_iterator & other) {
            if (this != &other)
                _iter = other._iter;
            return *this;
        }
        vector_iterator & operator=( const iterator_type it ) {
            _iter = it;
            return *this;
        }

        operator bool() const {
            if (_iter)
                return true;
            else
                return false;
        }

        bool operator==(const vector_iterator & other) const { return _iter == other._iter; }
        bool operator!=(const vector_iterator & other) const { return !(*this == other); }
        bool operator<(const vector_iterator & other) const { return _iter < other._iter; }
        bool operator<=(const vector_iterator & other) const { return !(other < *this) ; }
        bool operator>=(const vector_iterator & other) const { return !(*this < other); }
        bool operator>(const vector_iterator & other) const { return other < *this; }
        reference operator*() { return *_iter; }
        const reference operator*() const { return *_iter; }

        vector_iterator & operator+=(difference_type n) {
            _iter += n;
            return *this;
        }

        vector_iterator operator+(difference_type n) {
            vector_iterator tmp = *this;
            tmp += n;
            return tmp;
        }

        vector_iterator & operator-=(difference_type n) { return *this += -n; }

        vector_iterator operator-(difference_type n) const {
            vector_iterator tmp = *this;
            tmp -= n;
            return tmp;
        }
        difference_type operator-(const vector_iterator & other) const {
            return _iter - other._iter;
        }

        vector_iterator& operator++() {
            ++_iter;
            return *this;
        }
        vector_iterator operator++(int) {
            vector_iterator tmp = *this;
            ++_iter;
            return tmp;
        }
        vector_iterator& operator--() {
            --_iter;
            return *this;
        }
        vector_iterator operator--(int) {
            vector_iterator tmp = *this;
            --_iter;
            return tmp;
        }

        reference operator[](difference_type n) {
            return *(_iter + n);
        }
        iterator_type base() const { return _iter; }

    private:
        iterator_type _iter;
    };

//    template <class Iterator>
//    vector_iterator<Iterator> operator+(
//            typename vector_iterator<Iterator>::difference_type n,
//            const vector_iterator<Iterator> &rev_it) {
//        return vector_iterator<Iterator>(rev_it.base() + n);
//    }

    // Subtraction operator for vector_iterator
    template <class Iterator>
    typename vector_iterator<Iterator>::difference_type operator-(
            const vector_iterator<Iterator> &lhs,
            const vector_iterator<Iterator> &rhs) {
        return lhs.base() - rhs.base();
    }

    template<class Iter>
    class reverse_vector_iterator {
    public:
        typedef typename iterator_traits<Iter>::value_type value_type;
        typedef typename iterator_traits<Iter>::difference_type difference_type;
        typedef typename iterator_traits<Iter>::pointer pointer;
        typedef typename iterator_traits<Iter>::reference reference;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;
        typedef Iter iterator_type;

        reverse_vector_iterator(): _iter(iterator_type()) {}
        explicit reverse_vector_iterator( iterator_type it ): _iter(it) {}
        reverse_vector_iterator( const reverse_vector_iterator & other ): _iter(other._iter) {}
        ~reverse_vector_iterator() {}
        reverse_vector_iterator & operator=(const reverse_vector_iterator & other) {
            if (this != &other)
                _iter = other._iter;
            return *this;
        }
        reverse_vector_iterator & operator=( const iterator_type it ) {
            _iter = it;
            return *this;
        }

        operator bool() const {
            if (_iter)
                return true;
            else
                return false;
        }

        bool operator==(const reverse_vector_iterator & other) const { return _iter == other._iter; }
        bool operator!=(const reverse_vector_iterator & other) const { return !(*this == other); }
        bool operator<(const reverse_vector_iterator & other) const { return _iter < other._iter; }
        bool operator<=(const reverse_vector_iterator & other) const { return !(other < *this) ; }
        bool operator>=(const reverse_vector_iterator & other) const { return !(*this < other); }
        bool operator>(const reverse_vector_iterator & other) const { return other < *this; }
        reference operator*() { return *_iter; }
        const reference operator*() const { return *_iter; }
        reverse_vector_iterator & operator+=(difference_type n) {
            _iter -= n;
            return *this;
        }
        reverse_vector_iterator operator+(difference_type n) const {
            reverse_vector_iterator tmp = *this;
            return tmp -= n;
        }
        reverse_vector_iterator & operator-=(difference_type n) { return *this += n; }
        reverse_vector_iterator operator-(difference_type n) {
            reverse_vector_iterator tmp = *this;
            return tmp += n;
        }

        difference_type operator-(const reverse_vector_iterator & other) const {
            return other._iter - _iter;
        }


        reverse_vector_iterator& operator++() {
            --_iter;
            return *this;
        }
        reverse_vector_iterator operator++(int) {
            reverse_vector_iterator tmp = *this;
            --_iter;
            return tmp;
        }
        reverse_vector_iterator& operator--() {
            ++_iter;
            return *this;
        }
        reverse_vector_iterator operator--(int) {
            reverse_vector_iterator tmp = *this;
            ++_iter;
            return tmp;
        }

        reference operator[](difference_type n) {
            return *(_iter + n);
        }
        iterator_type base() { return _iter; }
    private:
        iterator_type _iter;
    };
}// namespace

#endif//FT_CONTAINERS_VECTOR_ITERATOR_HPP
