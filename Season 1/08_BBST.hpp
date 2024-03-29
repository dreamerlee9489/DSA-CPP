#ifndef BBST_HPP
#define BBST_HPP

#include "./08_BST.hpp"

namespace app {
	/**
	 * @brief 平衡二叉搜索树基类
	 * @date 2022-04-12
	 * @tparam T
	 */
	template <typename T> class BBST : public BST<T> {
		using NODE = typename IBinaryTree<T>::template Node<T>;

	protected:
		virtual void rotate(NODE* r, NODE* b, NODE* c, NODE* d, NODE* e, NODE* f);

		/**
		 * @brief 维护父指针
		 * @param grand 失衡结点
		 * @param parent 失衡结点的较高子树
		 * @param child 被交换的子树
		 */
		virtual void after_rotate(NODE* grand, NODE* parent, NODE* child);

		void rotate_left(NODE* grand);

		void rotate_right(NODE* grand);

	public:
		BBST(typename IBinaryTree<T>::Comparator comparator = nullptr)
			: BST<T>(comparator) {}

		virtual ~BBST() = default;
	};

	template <typename T> inline void BBST<T>::rotate_left(NODE* grand) {
		NODE* parent = grand->_right;
		NODE* child = parent->_left;
		grand->_right = child; // 祖父节点的右子树指向父结点的左子树
		parent->_left = grand; // 父结点的左子树指向祖父结点
		this->after_rotate(grand, parent, child);
	}

	template <typename T> inline void BBST<T>::rotate_right(NODE* grand) {
		NODE* parent = grand->_left;
		NODE* child = parent->_right;
		grand->_left = child;   // 祖父节点的左子树指向父结点的右子树
		parent->_right = grand; // 父结点的右子树指向祖父结点
		this->after_rotate(grand, parent, child);
	}

	template <typename T>
	inline void BBST<T>::rotate(NODE* r, NODE* b, NODE* c, NODE* d, NODE* e,
		NODE* f) {
		d->_parent = r->_parent;
		if (r->is_left())
			r->_parent->_left = d;
		else if (r->is_right())
			r->_parent->_right = d;
		else
			this->_root = d;
		b->_right = c;
		if (c != nullptr)
			c->_parent = b;
		f->_left = e;
		if (e != nullptr)
			e->_parent = f;
		d->_left = b;
		d->_right = f;
		b->_parent = d;
		f->_parent = d;
	}

	template <typename T>
	inline void BBST<T>::after_rotate(NODE* grand, NODE* parent, NODE* child) {
		parent->_parent = grand->_parent;
		if (grand->is_left())
			grand->_parent->_left = parent;
		else if (grand->is_right())
			grand->_parent->_right = parent;
		else
			this->_root = parent;
		if (child != nullptr)
			child->_parent = grand;
		grand->_parent = parent;
	}
} // namespace app

#endif /* BBST_HPP */