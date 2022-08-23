#ifndef FT_CONTAINERS_STACK_HPP
# define FT_CONTAINERS_STACK_HPP
# include <vector>
# include <cstddef>

namespace ft {
template <class T, class Container = std::vector<T> >
class stack {
public:
    typedef T value_type;
    typedef Container container_type;
    typedef size_t size_type;

    explicit stack( const container_type& ctnr = container_type() ): c(ctnr) {}
    stack( const stack& other ): c(other.c) {}
    ~stack() {}
    stack& operator=( const stack& other ) {
        if (this != &other)
            c = other.c;
        return *this;
    }
    value_type& top() { return c.back(); } ;
    const value_type& top() const { return c.back(); };
    size_type size() const { return c.size(); }
    void push( const value_type& value ) { c.push_back(value); }
    void pop() { c.pop_back(); }
    bool empty() const { return c.empty(); }

    friend bool operator== (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c == rhs.c; }

    friend bool operator!= (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c != rhs.c; };

    friend bool operator<  (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c < rhs.c; }

    friend bool operator<= (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c <= rhs.c; }

    friend bool operator>  (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c > rhs.c; }

    friend bool operator>= (const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs) { return lhs.c >= rhs.c; }

protected:
    container_type c;
};
}

#endif // FT_CONTAINERS_STACK_HPP
