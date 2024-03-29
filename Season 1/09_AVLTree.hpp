#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "./08_BBST.hpp"

namespace app {
	/**
	 * @brief AVL树
	 * @date 2022-04-11
	 * @tparam T
	 */
	template <typename T> class AVLTree : public BBST<T> {
		using NODE = typename IBinaryTree<T>::template Node<T>;

		template <typename U> struct AVLNode : public NODE {
			size_t _height = 1;

			AVLNode<U>& operator=(const AVLNode<U>& node);

			AVLNode<U>& operator=(AVLNode<U>&& node) noexcept;

			AVLNode(shared_ptr<U> data, NODE* parent = nullptr, NODE* left = nullptr,
				NODE* right = nullptr)
				: NODE(data, parent, left, right) {}

			AVLNode(const AVLNode<U>& node) { *this = node; }

			AVLNode(AVLNode<U>&& node) noexcept { *this = move(node); }

			~AVLNode() = default;

			int balance_factor();

			void update_height();

			NODE* taller_child();

			string to_string() const override {
				return ((IString&)*this->_data).to_string() +
					" h=" + std::to_string(_height);
			}
		};

		AVLNode<T>* get_node(shared_ptr<T> data) const override {
			return (AVLNode<T> *)BBST<T>::get_node(data);
		}

		NODE* create_node(shared_ptr<T> data, NODE* parent) override {
			return new AVLNode<T>(data, parent);
		}

		void rotate(NODE* r, NODE* b, NODE* c, NODE* d, NODE* e, NODE* f) override;

		/**
		 * @brief 维护父指针, 更新高度
		 * @param grand 失衡结点
		 * @param parent 失衡结点的较高子树
		 * @param child 被交换的子树
		 */
		void after_rotate(NODE* grand, NODE* parent, NODE* child) override;

		void after_add(NODE* node) override;

		void after_remove(NODE* node) override;

		bool is_balanced(NODE* node) {
			return abs(((AVLNode<T> *)node)->balance_factor()) <= 1;
		}

		void update_height(NODE* node) { ((AVLNode<T> *)node)->update_height(); }

		/**
		 * @brief 恢复平衡
		 * @param grand 高度最低的不平衡结点
		 */
		void rebalance(NODE* grand);

	public:
		AVLTree<T>& operator=(const AVLTree<T>& tree);

		AVLTree<T>& operator=(AVLTree<T>&& tree) noexcept;

		AVLTree(typename IBinaryTree<T>::Comparator comparator = nullptr)
			: BBST<T>(comparator) {}

		AVLTree(const AVLTree<T>& tree) { *this = tree; }

		AVLTree(AVLTree&& tree) noexcept { *this = move(tree); }

		~AVLTree() = default;
	};

	template <typename T>
	template <typename U>
	inline typename AVLTree<T>::template AVLNode<U>&
		AVLTree<T>::AVLNode<U>::operator=(const AVLNode<U>& node) {
		this->_data = node._data;
		this->_parent = node._parent;
		this->_left = node._left;
		this->_right = node._right;
		this->_height = node._height;
		return *this;
	}

	template <typename T>
	template <typename U>
	inline typename AVLTree<T>::template AVLNode<U>&
		AVLTree<T>::AVLNode<U>::operator=(AVLNode<U>&& node) noexcept {
		this->_data = nullptr;
		this = &node;
		return *this;
	}

	template <typename T>
	template <typename U>
	inline int AVLTree<T>::AVLNode<U>::balance_factor() {
		size_t leftH =
			(this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_height;
		size_t rightH =
			(this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_height;
		return (int)(leftH - rightH);
	}

	template <typename T>
	template <typename U>
	inline void AVLTree<T>::AVLNode<U>::update_height() {
		size_t leftH =
			(this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_height;
		size_t rightH =
			(this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_height;
		_height = 1 + max(leftH, rightH);
	}

	template <typename T>
	template <typename U>
	inline typename AVLTree<T>::NODE* AVLTree<T>::AVLNode<U>::taller_child() {
		size_t leftH =
			(this->_left == nullptr) ? 0 : ((AVLNode<U> *)this->_left)->_height;
		size_t rightH =
			(this->_right == nullptr) ? 0 : ((AVLNode<U> *)this->_right)->_height;
		if (leftH > rightH)
			return this->_left;
		if (leftH < rightH)
			return this->_right;
		// 删除结点引起不平衡时, 如果左右子树高度相同,
		// 破坏平滑结点与自身在父结点的位置相同
		return this->is_left() ? this->_left : this->_right;
	}

	template <typename T>
	inline AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& tree) {
		this->clear();
		if (tree._size > 0) {
			this->_comparator = tree._comparator;
			queue<NODE*> q;
			q.push(tree._root);
			while (!q.empty()) {
				NODE* elem = q.front();
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
	inline AVLTree<T>& AVLTree<T>::operator=(AVLTree<T>&& tree) noexcept {
		this->clear();
		this->_size = tree._size;
		this->_root = tree._root;
		this->_comparator = tree._comparator;
		tree._size = 0;
		tree._root = nullptr;
		tree._comparator = nullptr;
		return *this;
	}

	template <typename T> inline void AVLTree<T>::after_add(NODE* node) {
		// 添加结点可能会导致所有祖先结点失衡
		// 只需要调整高度最低的失衡结点
		while ((node = node->_parent) != nullptr) {
			if (is_balanced(node))
				update_height(node);
			else {
				rebalance(node);
				break;
			}
		}
	}

	template <typename T> inline void AVLTree<T>::after_remove(NODE* node) {
		// 删除结点可能会导致父结点或祖先结点失衡
		// 恢复平衡后, 更高层的祖先结点可能也需要调整
		while ((node = node->_parent) != nullptr) {
			if (is_balanced(node))
				update_height(node);
			else
				rebalance(node);
		}
	}

	template <typename T> inline void AVLTree<T>::rebalance(NODE* grand) {
		// 破坏平衡结点在其祖先结点和父结点的较高子树中
		NODE* parent = ((AVLNode<T> *)grand)->taller_child();
		NODE* node = ((AVLNode<T> *)parent)->taller_child();
		if (parent->is_left()) {
			if (node->is_left()) {
				// LL, 右单旋祖父结点
				this->rotate_right(grand);
				// rotate(grand, node, (AVLNode<T> *)node->_right, parent, (AVLNode<T>
				// *)parent->_right, grand);
			}
			else {
				// LR, 先左旋父结点, 再右旋祖父节点
				this->rotate_left(parent);
				this->rotate_right(grand);
				// rotate(grand, parent, (AVLNode<T> *)node->_left, node, (AVLNode<T>
				// *)node->_right, grand);
			}
		}
		else {
			if (node->is_left()) {
				// RL, 先右旋父结点, 再左旋祖父节点
				this->rotate_right(parent);
				this->rotate_left(grand);
				// rotate(grand, parent, (AVLNode<T> *)node->_left, node, (AVLNode<T>
				// *)node->_right, parent);
			}
			else {
				// RR, 左单旋祖父结点
				this->rotate_left(grand);
				// rotate(grand, grand, (AVLNode<T> *)parent->_left, parent, (AVLNode<T>
				// *)node->_left, node);
			}
		}
	}

	template <typename T>
	inline void AVLTree<T>::rotate(NODE* r, NODE* b, NODE* c, NODE* d, NODE* e,
		NODE* f) {
		BBST<T>::rotate(r, b, c, d, e, f);
		update_height(b);
		update_height(f);
		update_height(d);
	}

	template <typename T>
	inline void AVLTree<T>::after_rotate(NODE* grand, NODE* parent, NODE* child) {
		BBST<T>::after_rotate(grand, parent, child);
		update_height(grand);
		update_height(parent);
	}
} // namespace app

#endif /* AVL_TREE_HPP */