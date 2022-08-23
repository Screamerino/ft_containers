#ifndef FT_CONTAINERS_ITERATOR_HPP
# define FT_CONTAINERS_ITERATOR_HPP
# include <iterator>

namespace ft {
    template <class Iter>
    struct iterator_traits
    {
        typedef typename Iter::iterator_category iterator_category;
        typedef typename Iter::value_type value_type;
        typedef typename Iter::difference_type difference_type;
        typedef typename Iter::pointer pointer;
        typedef typename Iter::reference reference;
    };

    // define iterator traits for non-iterator types
    template <class T>
    struct iterator_traits<T *>
    {
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template <class T>
    struct iterator_traits<const T *>
    {
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template <class Category, class T, class Distance = ptrdiff_t,
             class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
    };
}

#endif//FT_CONTAINERS_ITERATOR_HPP
