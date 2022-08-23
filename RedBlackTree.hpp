#ifndef FT_CONTAINERS_REDBLACKTREE_HPP
# define FT_CONTAINERS_REDBLACKTREE_HPP
# include <iostream>
# include <memory>
# include "iterator.hpp"
# include "utility.hpp"

namespace ft {
    template<class T>
    struct Node {
        Node() { is_red = false; is_nil = true; }
        explicit Node(const T &key) : is_red(false), is_nil(false), key(key)  {}
        bool is_red;
        bool is_nil;
        T key;
        Node *left;
        Node *right;
        Node *p;
        bool isRightChild() const {
            return p->right == this;
        }
        bool isLeftChild() const {
            return p->left == this;
        }
    };

    template<class T>
    class tree_iterator {
    public:
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        typedef Node<T>* node_ptr;
        typedef std::bidirectional_iterator_tag iterator_category;

        tree_iterator(): pos(NULL) {}
        explicit tree_iterator(node_ptr pos): pos(pos) {}
        tree_iterator(const tree_iterator & other): pos(other.pos) {}

        template<class Iter>
        tree_iterator(const Iter & other): pos(other.base()) {}

        tree_iterator& operator=(const tree_iterator& other) {
            if (this != & other) {
                this->pos = other.pos;
            }
            return *this;
        }
        value_type& operator*() const {
            return pos->key;
        }
        value_type* operator->() const {
            return &(pos->key);
        }
        tree_iterator& operator++() {
            if (pos->is_nil) {
                return *this;
            }
            if (!pos->right->is_nil) {
                pos = pos->right;
                while (!pos->left->is_nil)
                    pos = pos->left;
            }
            else {
                while (pos->isRightChild())
                    pos = pos->p;
                pos = pos->p;
            }
            return *this;
        }
        tree_iterator operator++(int) {
            tree_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        tree_iterator& operator--() {
            if (pos->is_nil) {
                return *this;
            }
            if (!pos->left->is_nil) {
                pos = pos->left;
                while (!pos->right->is_nil)
                    pos = pos->right;
            }
            else {
                if (pos->isRightChild())
                    pos = pos->p;
                pos = pos->p;
            }
            return *this;
        }
        tree_iterator operator--(int) {
            tree_iterator tmp(*this);
            --(*this);
            return tmp;
        }
        bool operator==(tree_iterator const & other) const {
            return pos == other.pos;
        }
        bool operator!=(tree_iterator const & other) const {
            return !(*this == other);
        }
        node_ptr base() const { return pos; }

    private:
        node_ptr pos;
    };

    template<class T>
    class const_tree_iterator: public tree_iterator<T> {
    public:
        typedef Node<T>* node_ptr;
        typedef T value_type;

        const_tree_iterator(): tree_iterator<T>() {}
        explicit const_tree_iterator(node_ptr pos): tree_iterator<T>(pos) {}
        ~const_tree_iterator() {}
        const_tree_iterator(const_tree_iterator const & other): tree_iterator<T>(other) {}
        const_tree_iterator(tree_iterator<value_type> const & other): tree_iterator<T>(other) {}
    };

    template<class Iter>
    class reverse_tree_iterator {
        typedef typename iterator_traits<Iter>::value_type value_type;
        typedef typename iterator_traits<Iter>::difference_type difference_type;
        typedef typename iterator_traits<Iter>::pointer pointer;
        typedef typename iterator_traits<Iter>::reference reference;
        typedef typename iterator_traits<Iter>::iterator_category iterator_category;
        typedef typename Iter::node_ptr node_ptr;

    public:
        reverse_tree_iterator(): pos(NULL) {}
        reverse_tree_iterator(node_ptr pos): pos(pos) {}

        template<class T>
        reverse_tree_iterator(const reverse_tree_iterator<T> & other): pos(other.base()) {}

        reverse_tree_iterator& operator=(const reverse_tree_iterator& other) {
            if (this != & other) {
                this->pos = other.pos;
            }
            return *this;
        }
        value_type& operator*() const {
            return pos->key;
        }
        value_type* operator->() const {
            return &(pos->key);
        }
        reverse_tree_iterator& operator++() {
            if (pos->is_nil) {
                return *this;
            }
            if (!pos->left->is_nil) {
                pos = pos->left;
                while (!pos->right->is_nil)
                    pos = pos->right;
            }
            else {
                if (pos->isLeftChild())
                    pos = pos->p;
                pos = pos->p;
            }
            return *this;
        }
        reverse_tree_iterator operator++(int) {
            reverse_tree_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        reverse_tree_iterator& operator--() {
            if (pos->is_nil) {
                return *this;
            }
            if (!pos->right->is_nil) {
                pos = pos->right;
                while (!pos->left->is_nil)
                    pos = pos->left;
            }
            else {
                while (pos->isLeftChild())
                    pos = pos->p;
                pos = pos->p;
            }
            return *this;
        }
        reverse_tree_iterator operator--(int) {
            reverse_tree_iterator tmp(*this);
            --(*this);
            return tmp;
        }
        bool operator==(reverse_tree_iterator const & other) const {
            return pos == other.pos;
        }
        bool operator!=(reverse_tree_iterator const & other) const {
            return !(*this == other);
        }
        node_ptr base() const { return pos; }

    private:
        node_ptr pos;
    };

    template<class T, class Compare >
    class RedBlackTree {

    public:
        typedef tree_iterator<T> iterator;
        typedef const_tree_iterator<T> const_iterator;
        typedef reverse_tree_iterator<iterator> reverse_iterator;
        typedef reverse_tree_iterator<const_iterator> const_reverse_iterator;
        typedef Node<T>* node_ptr;

        RedBlackTree(Compare const & c): alloc(std::allocator<Node<T> >()), cmp(c) {
            nil = alloc.allocate(1);
            alloc.construct(nil, Node<T>());
            root = nil;
            nil->p = nil;
            first = nil;
            last = nil;
            _size = 0;
        }

        void copyTree(Node<T>*& new_root, Node<T>* parent, Node<T>* other_root) {
            if (other_root->is_nil)
                new_root = nil;
            else {
                new_root = alloc.allocate(1);
                alloc.construct(new_root, Node<T>(other_root->key));
                new_root->is_red = other_root->is_red;
                new_root->p = parent;
                copyTree(new_root->left, new_root, other_root->left);
                copyTree(new_root->right, new_root, other_root->right);
            }
        }

        RedBlackTree(const RedBlackTree& other): alloc(other.alloc), cmp(other.cmp) {
            nil = alloc.allocate(1);
            alloc.construct(nil, Node<T>());
            nil->p = nil;
            root = nil;
            copyTree(root, nil, other.root);
            _size = other.size();
            first = treeMinimum();
            last = treeMaximum();
        }

        RedBlackTree<T, Compare> & operator=(RedBlackTree<T, Compare> const & other) {
            if (this != &other) {
                clearTree(root);
                copyTree(root, nil, other.root);
                first = treeMinimum();
                last = treeMaximum();
            }
            return *this;
        }

        ~RedBlackTree() {
            clearTree(root);
            alloc.destroy(nil);
            alloc.deallocate(nil, 1);
        }

        Node<T> * newNode(const T & key) {
            Node<T> * ret = alloc.allocate(1);
            alloc.construct(ret, Node<T>(key));
            ret->left = nil;
            ret->right = nil;
            ret->p = nil;
            ret->is_nil = false;
            ret->is_red = true;
            return ret;
        }

        void clearTree() {
            clearTree(root);
            root = nil;
        }

        void clearTree(Node<T> *x) {
            if (x != nil) {
                clearTree(x->left);
                Node<T> *right = x->right;
                alloc.destroy(x);
                alloc.deallocate(x, 1);
                clearTree(right);
                --_size;
            }
        }

        void treeWalk() {
            treeWalk(root);
        }

        void treeWalk(Node<T> *x) {
            if (x != nil) {
                treeWalk(x->left);
                std::cout << x->key << " ";
                treeWalk(x->right);
            }
        }

        Node<T> *search(const T& k) const {
            return search(root, k);
        }

        Node<T> *search(Node<T> *x, const T& k) const {
            if (x == nil || (!cmp(k, x->key) && !cmp(x->key, k)))
                return x;
            if (cmp(k, x->key))
                return search(x->left, k);
            else
                return search(x->right, k);
        }

        Node<T> *iterSearch(const T& k) const {
            Node<T> *x = root;
            while (x != nil && k != x->key) {
                if (cmp(k, x->key))
                    x = x->left;
                else
                    x = x->right;
            }
            return x;
        }

        Node<T> *treeMinimum() {
            return treeMinimum(root);
        }

        Node<T> *treeMaximum() {
            return treeMaximum(root);
        }

        Node<T> *treeMinimum(Node<T> *x) {
            while (x->left != nil)
                x = x->left;
            return x;
        }

        Node<T> *treeMaximum(Node<T> *x) {
            while (x->right != nil)
                x = x->right;
            return x;
        }


        iterator begin() { return iterator(first); }
        const_iterator begin() const { return const_iterator(first); }
        iterator end() { return ++iterator(last); }
        const_iterator end() const { return ++const_iterator(last); }
        reverse_iterator rbegin() { return reverse_iterator(last); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(last); }
        reverse_iterator rend() { return ++reverse_iterator(first); }
        const_reverse_iterator rend() const { return ++const_reverse_iterator(first); }


        iterator lower_bound(const T & key) {
            Node<T> *x = root;
            Node<T> *y = nil;
            while (!x->is_nil) {
                if (cmp(x->key, key))
                    x = x->right;
                else {
                    y = x;
                    x = x->left;
                }
            }
            return iterator(y);
        }

        const_iterator lower_bound(const T & key) const {
            Node<T> *x = root;
            Node<T> *y = nil;
            while (!x->is_nil) {
                if (cmp(x->key, key))
                    x = x->right;
                else {
                    y = x;
                    x = x->left;
                }
            }
            return iterator(y);
        }

        iterator upper_bound(const T & key) {
            Node<T> *x = root;
            Node<T> *y = nil;
            while (!x->is_nil) {
                if (cmp(key, x->key)) {
                    y = x;
                    x = x->left;
                }
                else
                    x = x->right;
            }
            return iterator(y);
        }

        const_iterator upper_bound(const T & key) const {
            Node<T> *x = root;
            Node<T> *y = nil;
            while (!x->is_nil) {
                if (cmp(key, x->key)) {
                    y = x;
                    x = x->left;
                }
                else
                    x = x->right;
            }
            return iterator(y);
        }

        void swap( RedBlackTree& other ) {
            std::swap(root, other.root);
            std::swap(nil, other.nil);
            std::swap(first, other.first);
            std::swap(last, other.last);
            std::swap(alloc, other.alloc);
            std::swap(cmp, other.cmp);
            std::swap(_size, other._size);
        }

        Node<T> *treeSuccessor(Node<T> *x) {
            if (x->right != nil)
                return treeMinimum(x->right);
            Node<T> *y = x->p;
            while (y != nil && x == y->right) {
                x = y;
                y = y->p;
            }
            return y;
        }

        Node<T> *treePredecessor(Node<T> *x) {
            if (x->left != nil)
                return treeMaximum(x->left);
            Node<T> *y = x->p;
            while (y != nil && x == y->left) {
                x = y;
                y = y->p;
            }
            return y;
        }

        void leftRotate(Node<T> *x) {
            Node<T> *y = x->right;
            x->right = y->left;
            if (y->left != nil)
                y->left->p = x;
            y->p = x->p;
            if (x->p == nil)
                root = y;
            else if (x->p->left == x)
                x->p->left = y;
            else
                x->p->right = y;
            y->left = x;
            x->p = y;
        }

        void rightRotate(Node<T> *x) {
            Node<T> *y = x->left;
            x->left = y->right;
            if (y->right != nil)
                y->right->p = x;
            y->p = x->p;
            if (x->p == nil)
                root = y;
            else if (x->p->left == x)
                x->p->left = y;
            else
                x->p->right = y;
            y->right = x;
            x->p = y;
        }

        ft::pair<iterator, bool> rbInsert(const T& value) {
            Node<T> *new_node = newNode(value);
            ft::pair<iterator, bool> result = rbInsert(new_node);
            if (!result.second) {
                alloc.destroy(new_node);
                alloc.deallocate(new_node, 1);
            }
            return result;
        }

        ft::pair<iterator, bool> rbInsert(Node<T> *z) {
            Node<T> *x = root;
            Node<T> *y = nil;
            while (x != nil) {
                y = x;
                if (cmp(z->key, x->key))
                    x = x->left;
                else if (!cmp(z->key, x->key) && !cmp(x->key, z->key))
                    return ft::make_pair(iterator(x), false);
                else
                    x = x->right;
            }
            z->p = y;
            if (y == nil)
                root = z;
            else if (cmp(z->key, y->key))
                y->left = z;
            else
                y->right = z;
            z->left = nil;
            z->right = nil;
            z->is_red = true;
            insertFixUp(z);
            ++_size;
            return ft::make_pair(iterator(z), true);
        }

        iterator rbInsert(iterator hint, const T& value) {
            Node<T> *node = hint.base();
            Node<T> *z = newNode(value);
            iterator res = iterator(z);
            if (node->left == nil && cmp(value, node->key)) {
                node->left = z;
                ++_size;
                insertFixUp(z);
            }
            else if (node->right == nil && cmp(node->key, value)) {
                node->right = z;
                ++_size;
                insertFixUp(z);
            }
            else
                res = rbInsert(z).first;
            return res;
        }

        void insertFixUp(Node<T> *z) {
            while (z->p->is_red) {
                if (z->p == z->p->p->left) {
                    Node<T> *y = z->p->p->right;
                    if (y->is_red) {
                        z->p->is_red = false;
                        y->is_red = false;
                        z->p->p->is_red = true;
                        z = z->p->p;
                    } else {
                        if (z == z->p->right) {
                            z = z->p;
                            leftRotate(z);
                        }
                        z->p->is_red = false;
                        z->p->p->is_red = true;
                        rightRotate(z->p->p);
                    }
                } else {
                    if (z->p == z->p->p->right) {
                        Node<T> *y = z->p->p->left;
                        if (y->is_red) {
                            z->p->is_red = false;
                            y->is_red = false;
                            z->p->p->is_red = true;
                            z = z->p->p;
                        } else {
                            if (z == z->p->left) {
                                z = z->p;
                                rightRotate(z);
                            }
                            z->p->is_red = false;
                            z->p->p->is_red = true;
                            leftRotate(z->p->p);
                        }
                    }
                }
            }
            root->is_red = false;
            first = treeMinimum();
            last = treeMaximum();
        }

        void transplant(Node<T> *u, Node<T> *v) {
            if (u->p == nil)
                root = v;
            else if (u == u->p->left)
                u->p->left = v;
            else
                u->p->right = v;
            v->p = u->p;
        }

        void rbDelete(Node<T> *z) {
            Node<T> *y = z;
            Node<T> *x;
            bool is_red = y->is_red;
            if (z->left == nil) {
                x = z->right;
                transplant(z, z->right);
            } else if (z->right == nil) {
                x = z->left;
                transplant(z, z->left);
            } else {
                y = treeMinimum(z->right);
                is_red = y->is_red;
                x = y->right;
                if (y->p == z)
                    x->p = y;
                else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->p = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->p = y;
                y->is_red = z->is_red;
            }
            --_size;
            if (!is_red)
                deleteFixUp(x);
            first = treeMinimum();
            last = treeMaximum();
            alloc.destroy(z);
            alloc.deallocate(z, 1);
        }

        void deleteFixUp(Node<T> *x) {
            while (x != root && !x->is_red) {
                if (x == x->p->left) {
                    Node<T> *w = x->p->right;
                    if (w->is_red) {
                        w->is_red = false;
                        x->p->is_red = true;
                        leftRotate(x->p);
                        w = x->p->right;
                    }
                    if (!w->left->is_red && !w->right->is_red) {
                        w->is_red = true;
                        x = x->p;
                    } else {
                        if (!w->right->is_red) {
                            w->left->is_red = false;
                            w->is_red = true;
                            rightRotate(w);
                            w = x->p->right;
                        }
                        w->is_red = x->p->is_red;
                        x->p->is_red = false;
                        w->right->is_red = false;
                        leftRotate(x->p);
                        x = root;
                    }
                } else {
                    Node<T> *w = x->p->left;
                    if (w->is_red) {
                        w->is_red = false;
                        x->p->is_red = true;
                        rightRotate(x->p);
                        w = x->p->left;
                    }
                    if (!w->right->is_red && !w->left->is_red) {
                        w->is_red = true;
                        x = x->p;
                    } else {
                        if (!w->left->is_red) {
                            w->right->is_red = false;
                            w->is_red = true;
                            leftRotate(w);
                            w = x->p->left;
                        }
                        w->is_red = x->p->is_red;
                        x->p->is_red = false;
                        w->left->is_red = false;
                        rightRotate(x->p);
                        x = root;
                    }
                }
            }
            x->is_red = false;
        }
        size_t size() const {
            return _size;
        }
        size_t max_size() const {
            return alloc.max_size();
        }


    private:
        Node<T> *root;
        Node<T> *nil;
        Node<T> *first;
        Node<T> *last;
        std::allocator<Node<T> > alloc;
        Compare cmp;
        size_t _size;
    };
}// namespace ft
#endif//FT_CONTAINERS_REDBLACKTREE_HPP
