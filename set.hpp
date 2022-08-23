#ifndef FT_CONTAINERS_SET_HPP
# define FT_CONTAINERS_SET_HPP
# include "RedBlackTree.hpp"
# include "algorithm.hpp"

namespace ft {
    template<
            class Key,
            class Compare = std::less<Key>,
            class Allocator = std::allocator<Key>>
    class set {
    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        typedef Allocator allocator_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename Allocator::pointer pointer;
        typedef typename Allocator::const_pointer const_pointer;
        typedef typename RedBlackTree<value_type, Compare>::iterator iterator;
        typedef typename RedBlackTree<value_type, Compare>::const_iterator const_iterator;
        typedef typename RedBlackTree<value_type, Compare>::reverse_iterator reverse_iterator;
        typedef typename RedBlackTree<value_type, Compare>::const_reverse_iterator const_reverse_iterator;

        set() : _comp(value_compare(key_compare())), _tree(_comp) {}

        explicit set(const Compare &comp,
                     const Allocator &alloc = Allocator()) : _comp(value_compare(comp)), _alloc(alloc), _tree(_comp) {}

        template<class InputIt>
        set(InputIt first, InputIt last,
            const Compare &comp = Compare(),
            const Allocator &alloc = Allocator()) : _comp(value_compare(comp)), _alloc(alloc), _tree(_comp) {
            for (; first != last; ++first) {
                _tree.rbInsert(_tree.newNode(ft::make_pair(*first)));
            }
        }

        set(const set &other) : _tree(other._tree), _comp(value_compare(key_compare())), _alloc(other._alloc) {}

        set &operator=(const set &other) {
            if (this != &other)
                _tree = other._tree;
            return *this;
        }

        ~set() {}

        allocator_type get_allocator() const { return _alloc; }

        iterator begin() { return _tree.begin(); }
        const_iterator begin() const { return _tree.begin(); }
        iterator end() { return _tree.end(); }
        const_iterator end() const { return _tree.end(); }
        reverse_iterator rbegin() { return _tree.rbegin(); }
        const_reverse_iterator rbegin() const { return _tree.rbegin(); }
        reverse_iterator rend() { return _tree.rend(); }
        const_reverse_iterator rend() const { return _tree.rend(); }

        ft::pair<iterator, bool> insert(const value_type &value) {
            return _tree.rbInsert(value);
        }

        iterator insert(iterator hint, const value_type &value) {
            return _tree.rbInsert(hint, value);
        }

        template<class InputIt>
        void insert(InputIt first, InputIt last) {
            for (; first != last; ++first)
                _tree.rbInsert(*first);
        }

        size_type size() const { return _tree.size(); }
        size_type max_size() const { return _tree.max_size(); }

        size_type count(const Key &key) const {
            node_ptr elem = _tree.search(key);
            if (elem->is_nil)
                return 0;
            else
                return 1;
        }
        iterator find(const Key &key) {
            return iterator(_tree.search(key));
        }
        const_iterator find( const Key& key ) const {
            return const_iterator(_tree.search(key));
        }

        void clear() {
            _tree.clearTree();
        }

        bool empty() const { return size() == 0; }
        key_compare key_comp() const { return _key_comp; }
        set::value_compare value_comp() const { return _comp; }

        void erase(iterator pos) {
            _tree.rbDelete(pos.base());
        }

        void erase(iterator first, iterator last) {
            if (size() != 0)
                while (first != last)
                    erase(first++);
        }

        size_type erase(const Key &key) {
            iterator elem = find(key);
            if (elem == end())
                return 0;
            erase(elem);
            return 1;
        }

        iterator lower_bound(const Key &key) {
            return _tree.lower_bound(key);
        }

        const_iterator lower_bound(const Key &key) const {
            return _tree.lower_bound(key);
        }

        iterator upper_bound(const Key &key) {
            return _tree.upper_bound(key);
        }

        const_iterator upper_bound(const Key &key) const {
            return _tree.upper_bound(key);
        }

        ft::pair<iterator, iterator> equal_range(const Key &key) {
            return ft::make_pair(lower_bound(key), upper_bound(key));
        }

        ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const {
            return ft::make_pair(lower_bound(key), upper_bound(key));
        }

        void swap(set &other) {
            _tree.swap(other._tree);
            std::swap(_key_comp, other._key_comp);
            std::swap(_comp, other._comp);
            std::swap(_alloc, other._alloc);
        }

    private:
        typedef Node<value_type> *node_ptr;
        RedBlackTree<value_type, value_compare> _tree;
        key_compare _key_comp;
        value_compare _comp;
        allocator_type _alloc;

        ft::RedBlackTree<value_type, value_compare> &tree() { return _tree; }
    };

    template<class Key, class Compare, class Alloc>
    void swap(ft::set<Key, Compare, Alloc> &lhs,
              ft::set<Key, Compare, Alloc> &rhs) {
        lhs.swap(rhs);
    }

    template<class Key, class Compare, class Alloc>
    bool operator==(const ft::set<Key, Compare, Alloc> &lhs,
                    const ft::set<Key, Compare, Alloc> &rhs) {
        return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class Key, class Compare, class Alloc>
    bool operator!=(const ft::set<Key, Compare, Alloc> &lhs,
                    const ft::set<Key, Compare, Alloc> &rhs) { return !(lhs == rhs); }

    template<class Key, class Compare, class Alloc>
    bool operator<(const ft::set<Key, Compare, Alloc> &lhs,
                   const ft::set<Key, Compare, Alloc> &rhs) {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template<class Key, class Compare, class Alloc>
    bool operator>(const ft::set<Key, Compare, Alloc> &lhs,
                   const ft::set<Key, Compare, Alloc> &rhs) { return rhs < lhs; }

    template<class Key, class Compare, class Alloc>
    bool operator>=(const ft::set<Key, Compare, Alloc> &lhs,
                    const ft::set<Key, Compare, Alloc> &rhs) { return !(rhs < lhs); }

    template<class Key, class Compare, class Alloc>
    bool operator<=(const ft::set<Key, Compare, Alloc> &lhs,
                    const ft::set<Key, Compare, Alloc> &rhs) { return !(lhs < rhs); }

}
}


#endif//FT_CONTAINERS_SET_HPP
