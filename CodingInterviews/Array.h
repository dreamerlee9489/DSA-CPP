#pragma once
#include <stdlib.h>
#include <exception>

// Random Partition
int RandomInRange(int min, int max)
{
	int random = rand() % (max - min + 1) + min;
	return random;
}

void Swap(int* num1, int* num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

int Partition(int data[], int length, int start, int end)
{
	if (data == nullptr || length <= 0 || start < 0 || end >= length)
		throw new std::exception();

	int index = RandomInRange(start, end);
	Swap(&data[index], &data[end]); //将基准元素移至数组末尾

	int small = start - 1;
	for (index = start; index < end; ++index)
	{
		if (data[index] < data[end])
		{
			++small;
			if (small != index)
				Swap(&data[index], &data[small]);
		}
	}

	++small;
	Swap(&data[small], &data[end]); //将基准元素交换到最终位置

	return small;   //基准元素的索引
}