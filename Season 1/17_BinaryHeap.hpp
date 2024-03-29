#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include "./17_IHeap.hpp"

namespace app {
	/**
	 * @brief 二叉堆
	 * @date 2022-04-22
	 * @tparam T
	 */
	template <typename T> class BinaryHeap : public IHeap<T> {
		const size_t DEFAULT_CAPACITY = 8;
		size_t _capacity = 0;
		shared_ptr<T>* _array = nullptr;

		void ensure_capacity();

		/**
		 * @brief 上滤
		 * @param index
		 */
		void sift_up(int index);

		/**
		 * @brief 下滤
		 * @param index
		 */
		void sift_down(int index);

		/**
		 * @brief 建堆
		 * @remark 上滤复杂度O(nlogn), 下滤复杂度O(n)
		 */
		void heapify() {
			for (int i = (this->_size >> 1) - 1; i >= 0; --i)
				sift_down(i);
		}

		void heap_empty_check() const {
			if (this->_size == 0)
				throw out_of_range("heap is empty.");
		}

		void not_null_check(shared_ptr<T> data) const {
			if (data == nullptr)
				throw invalid_argument("data must be not null.");
		}

	public:
		BinaryHeap<T>& operator=(const BinaryHeap<T>& heap);

		BinaryHeap<T>& operator=(BinaryHeap<T>&& heap) noexcept;

		/**
		 * @brief 将数组调整为二叉堆
		 * @param comparator
		 * @param array
		 * @param size
		 */
		BinaryHeap(typename IHeap<T>::Comparator comparator = nullptr,
			shared_ptr<T>* array = nullptr, size_t size = 0);

		BinaryHeap(const BinaryHeap<T>& heap) { *this = heap; }

		BinaryHeap(BinaryHeap<T>&& heap) noexcept { *this = move(heap); }

		~BinaryHeap() { delete[] _array; }

		size_t capacity() const { return _capacity; }

		void add(shared_ptr<T> data) override;

		shared_ptr<T> remove() override;

		/**
		 * @brief 删除堆顶元素, 添加新元素到堆中
		 * @param data 新元素
		 * @return shared_ptr<T> 被删除的堆顶元素
		 * @remark TopK问题: 从N个元素中, 找出K个最值
		 *         将前K个元素添加到堆中, 从K+1个数开始, 使用replace替换堆顶元素
		 *         遍历完成后, 堆中剩下的就是K个最值
		 */
		shared_ptr<T> replace(shared_ptr<T> data) override;

		shared_ptr<T> get() const override {
			heap_empty_check();
			return _array[0];
		}

		void clear() override {
			for (size_t i = 0; i < _capacity; ++i)
				_array[i] = nullptr;
			this->_size = 0;
		}

		void traverse(typename IHeap<T>::TraverseFunc func = nullptr) const override;
	};

	template <typename T>
	inline BinaryHeap<T>& BinaryHeap<T>::operator=(const BinaryHeap<T>& heap) {
		delete[] _array;
		_array = new shared_ptr<T>[heap._capacity];
		_capacity = heap._capacity;
		this->_size = heap._size;
		this->_comparator = heap._comparator;
		for (size_t i = 0; i < heap._size; ++i)
			_array[i] = heap._array[i];
		return *this;
	}

	template <typename T>
	inline BinaryHeap<T>& BinaryHeap<T>::operator=(BinaryHeap<T>&& heap) noexcept {
		delete[] _array;
		_array = heap._array;
		_capacity = heap._capacity;
		this->_size = heap._size;
		this->_comparator = heap._comparator;
		heap._array = nullptr;
		heap._size = 0;
		return *this;
	}

	template <typename T>
	inline BinaryHeap<T>::BinaryHeap(typename IHeap<T>::Comparator comparator,
		shared_ptr<T>* array, size_t size)
		: IHeap<T>(comparator) {
		if (array == nullptr || size == 0) {
			_capacity = DEFAULT_CAPACITY;
			_array = new shared_ptr<T>[DEFAULT_CAPACITY] {};
		}
		else {
			this->_size = size;
			_capacity = max(size, DEFAULT_CAPACITY);
			_array = new shared_ptr<T>[_capacity];
			for (size_t i = 0; i < size; ++i)
				_array[i] = array[i];
			heapify();
		}
	}

	template <typename T> inline void BinaryHeap<T>::add(shared_ptr<T> data) {
		not_null_check(data);
		ensure_capacity();
		_array[this->_size++] = data;
		sift_up(this->_size - 1);
	}

	template <typename T> inline shared_ptr<T> BinaryHeap<T>::remove() {
		heap_empty_check();
		size_t last = --this->_size;
		shared_ptr<T> root = _array[0];
		_array[0] = _array[last];
		_array[last] = nullptr;
		sift_down(0);
		return root;
	}

	template <typename T>
	inline shared_ptr<T> BinaryHeap<T>::replace(shared_ptr<T> data) {
		not_null_check(data);
		shared_ptr<T> root = nullptr;
		if (this->_size == 0) {
			_array[0] = data;
			this->_size++;
		}
		else {
			root = _array[0];
			_array[0] = data;
			sift_down(0);
		}
		return root;
	}

	template <typename T>
	inline void
		BinaryHeap<T>::traverse(typename IHeap<T>::TraverseFunc func) const {
		if (this->_size > 0) {
			int index = 0, lv_cnt = 1;
			queue<shared_ptr<T>> q;
			q.push(_array[index]);
			while (!q.empty()) {
				size_t left = (index << 1) + 1, right = left + 1;
				lv_cnt--;
				index++;
				shared_ptr<T> data = q.front();
				q.pop();
				if (left < this->_size)
					q.push(_array[left]);
				if (right < this->_size)
					q.push(_array[right]);
				if (func != nullptr)
					func(data);
				else
					cout << *data << "\t";
				if (lv_cnt == 0) {
					lv_cnt = q.size();
					cout << "\n";
				}
			}
		}
	}

	template <typename T> inline void BinaryHeap<T>::ensure_capacity() {
		if (this->_size >= _capacity) {
			shared_ptr<T>* old = _array;
			_capacity <<= 1;
			_array = new shared_ptr<T>[_capacity];
			for (size_t i = 0; i < this->_size; ++i)
				_array[i] = old[i];
			delete[] old;
		}
	}

	template <typename T> inline void BinaryHeap<T>::sift_up(int index) {
		shared_ptr<T> node = _array[index]; // 备份新结点
		while (index > 0) {
			int parent_index = (index - 1) >> 1;
			shared_ptr<T> parent = _array[parent_index];
			if (this->compare(node, parent) <= 0) // node不大于父结点, 退出循环
				break;
			_array[index] = parent; // node大于父结点, 交换位置
			index = parent_index;
		}
		_array[index] = node;
	}

	template <typename T> inline void BinaryHeap<T>::sift_down(int index) {
		shared_ptr<T> parent = _array[index]; // 备份根结点
		int half = this->_size >> 1;          // 完全二叉树首个叶结点
		while (index < half) {
			int child_index = (index << 1) + 1, right_index = child_index + 1;
			shared_ptr<T> child = _array[child_index]; // 默认左子结点和父结点比
			if (right_index < this->_size &&
				this->compare(_array[child_index], _array[right_index]) < 0)
				child = _array[child_index = right_index]; // 右子结点大于左子结点
			if (this->compare(parent, child) >= 0) // 子结点小于父结点, 退出循环
				break;
			_array[index] = child; // 父结点大于最大子结点, 交换位置
			index = child_index;
		}
		_array[index] = parent;
	}
} // namespace app

#endif /* BINARY_HEAP_HPP */
