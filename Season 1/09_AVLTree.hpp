#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP
#include "./08_BBST.hpp"

namespace app
{
    /**
     * @brief AVL树
     * @date 2022-04-11
     * @tparam T
     */
    template <typename T>
    class AVLTree : public BBST<T>
    {
        using NODE = typename IBinaryTree<T>::template Node<T>;
        template <typename U>
        struct AVLNode : public NODE
        {
            size_t _HPPeight = 1;
            AVLNode<U> &operator=(const AVLNode<U> &node);
            AVLNode<U> &operator=(AVLNode<U> &&node) noexcept;
            AVLNode(shared_ptr<U> data, NODE *parent = nullptr, NODE *left = nullptr, NODE *right = nullptr)
                : NODE(data, parent, left, right) {}
            AVLNode(const AVLNode<U> &node) { *this = node; }
            AVLNode(AVLNode<U> &&node) noexcept { *this = move(node); }
            ~AVLNode() = default;
            int balance_factor();
            void update_HPPeight();
            NODE *taller_child();
            string to_string() const override { return ((IString &)*this->_data).to_string() + " h=" + std::to_string(_HPPeight); }
        };
        AVLNode<T> *get_node(shared_ptr<T> data) const override { return (AVLNode<T> *)BBST<T>::get_node(data); }
        NODE *create_node(shared_ptr<T> data, NODE *parent) override { return new AVLNode<T>(data, parent); }
        void rotate(NODE *r, NODE *b, NODE *c, NODE *d, NODE *e, NODE *f) override;
        void after_rotate(NODE *grand, NODE *parent, NODE *child) override;
        void after_add(NODE *node) override;
        void after_remove(NODE *node) override;
        bool is_balanced(NODE *node) { return abs(((AVLNode<T> *)node)->balance_factor()) <= 1; }
        void update_HPPeight(NODE *node) { ((AVLNode<T> *)node)->update_HPPeight(); }
        void rebalance(NODE *grand);

    public:
        AVLTree<T> &operator=(const AVLTree<T> &tree);
        AVLTree<T> &operator=(AVLTree<T> &&tree) noexcept;
        AVLTree(typename IBinaryTree<T>::Comparator comparator = nullptr) : BBST<T>(comparator) {}
        AVLTree(const AVLTree<T> &tree) { *this = tree; }
        AVLTree(AVLTree &&tree) noexcept { *this = move(tree); }
        ~AVLTree() = default;
    };

    template <typename T>
    template <typename U>
    inline AVLTree<T>::AVLNode<U> &AVLTree<T>::AVLNode<U>::operator=(const AVLNode<U> &node)
    {
        this->_data = node._data;
        this->_parent = node._parent;
        this->_left = node._left;
        this->_right = node._right;
        _HPPeight = node._HPPeight;
        return *this;
    }

    template <typename T>
    template <typename U>
    inline AVLTree<T>::AVLNode<U> &AVLTree<T>::AVLNode<U>::operator=(AVLNode<U> &&node) noexcept
    {
        this->_data = nullptr;
        this = &node;
        return *this;
    }

    template <typename T>
    template <typename U>
    inline int AVLTree<T>::AVLNode<U>::balance_factor()
    {
        size_t leftH = (this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_HPPeight;
        size_t rightH = (this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_HPPeight;
        return (int)(leftH - rightH);
    }

    template <typename T>
    template <typename U>
    inline void AVLTree<T>::AVLNode<U>::update_HPPeight()
    {
        size_t leftH = (this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_HPPeight;
        size_t rightH = (this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_HPPeight;
        _HPPeight = 1 + max(leftH, rightH);
    }

    template <typename T>
    template <typename U>
    inline typename AVLTree<T>::NODE *AVLTree<T>::AVLNode<U>::taller_child()
    {
        size_t leftH = (this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_HPPeight;
        size_t rightH = (this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_HPPeight;
        if (leftH > rightH)
            return this->_left;
        if (leftH < rightH)
            return this->_right;
        return this->is_left() ? this->_left : this->_right;
    }

    template <typename T>
    inline AVLTree<T> &AVLTree<T>::operator=(const AVLTree<T> &tree)
    {
        this->clear();
        if (tree._size > 0)
        {
            this->_comparator = tree._comparator;
            queue<NODE *> q;
            q.push(tree._root);
            while (!q.empty())
            {
                NODE *elem = q.front();
                this->add(elem->_data);
                q.pop();
                if (elem->_left != nullptr)
                    q.push(elem->_left);
                if (elem->_right != nullptr)
                    q.push(elem->_right);
            }
        }
        return *this;
    }

    template <typename T>
    inline AVLTree<T> &AVLTree<T>::operator=(AVLTree<T> &&tree) noexcept
    {
        this->clear();
        this->_size = tree._size;
        this->_root = tree._root;
        this->_comparator = tree._comparator;
        tree._size = 0;
        tree._root = nullptr;
        tree._comparator = nullptr;
        return *this;
    }

    template <typename T>
    inline void AVLTree<T>::after_add(NODE *node)
    {
        while ((node = node->_parent) != nullptr)
        {
            if (is_balanced(node))
                update_HPPeight(node);
            else
            {
                rebalance(node);
                break;
            }
        }
    }

    template <typename T>
    inline void AVLTree<T>::after_remove(NODE *node)
    {
        while ((node = node->_parent) != nullptr)
        {
            if (is_balanced(node))
                update_HPPeight(node);
            else
                rebalance(node);
        }
    }

    template <typename T>
    inline void AVLTree<T>::rebalance(NODE *grand)
    {
        NODE *parent = ((AVLNode<T> *)grand)->taller_child();
        NODE *node = ((AVLNode<T> *)parent)->taller_child();
        if (parent->is_left())
        {
            if (node->is_left())
            {
                this->rotate_right(grand);
                // rotate(grand, node, (AVLNode<T> *)node->_right, parent, (AVLNode<T> *)parent->_right, grand);
            }
            else
            {
                this->rotate_left(parent);
                this->rotate_right(grand);
                // rotate(grand, parent, (AVLNode<T> *)node->_left, node, (AVLNode<T> *)node->_right, grand);
            }
        }
        else
        {
            if (node->is_left())
            {
                this->rotate_right(parent);
                this->rotate_left(grand);
                // rotate(grand, parent, (AVLNode<T> *)node->_left, node, (AVLNode<T> *)node->_right, parent);
            }
            else
            {
                this->rotate_left(grand);
                // rotate(grand, grand, (AVLNode<T> *)parent->_left, parent, (AVLNode<T> *)node->_left, node);
            }
        }
    }

    template <typename T>
    inline void AVLTree<T>::rotate(NODE *r, NODE *b, NODE *c, NODE *d, NODE *e, NODE *f)
    {
        BBST<T>::rotate(r, b, c, d, e, f);
        update_HPPeight(b);
        update_HPPeight(f);
        update_HPPeight(d);
    }

    template <typename T>
    inline void AVLTree<T>::after_rotate(NODE *grand, NODE *parent, NODE *child)
    {
        BBST<T>::after_rotate(grand, parent, child);
        update_HPPeight(grand);
        update_HPPeight(parent);
    }
} // namespace app

#endif /* AVL_TREE_HPP */