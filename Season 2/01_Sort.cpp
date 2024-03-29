#include "./02_BubbleSort.hpp"
#include "./03_SelectSort.hpp"
#include "./04_InsertSort.hpp"
#include "./05_HeapSort.hpp"
#include "./06_MergeSort.hpp"
#include "./07_QuickSort.hpp"
#include <initializer_list>

using namespace std;
using namespace app;

void test_sort(int* array, size_t size, initializer_list<ISort<int>*> list) {
	for (ISort<int>* const* beg = list.begin(); beg != list.end(); ++beg) {
		int* copy = new int[size];
		for (size_t i = 0; i < size; ++i)
			copy[i] = array[i];
		(*beg)->sort(copy, size);
		cout << typeid(**beg).name() << "\t";
		for (size_t i = 0; i < size; ++i)
			cout << copy[i] << " ";
		cout << "\n";
	}
}

int main(int argc, char const* argv[]) {
	int array[] = { 7, 3, 5, 8, 6, 11, 4, 5, 3, 10, 2, 12, 4, 1, 9 };
	test_sort(array, 15,
		{ new BubbleSort<int>(), new SelectSort<int>(),
		 new InsertSort<int>(), new HeapSort<int>(), new MergeSort<int>(),
		 new QuickSort<int>() });

	return 0;
}

/*
N3app10BubbleSortIiEE   1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
N3app10SelectSortIiEE   1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
N3app10InsertSortIiEE   1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
N3app8HeapSortIiEE      1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
N3app9MergeSortIiEE     1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
N3app9QuickSortIiEE     1 2 3 3 4 4 5 5 6 7 8 9 10 11 12
*/
