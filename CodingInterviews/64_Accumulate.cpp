#include <cstdio>

// ====================方法一====================
class Temp
{
public:
	Temp() { ++N; Sum += N; }

	static void Reset() { N = 0; Sum = 0; }
	static unsigned int GetSum() { return Sum; }

private:
	static unsigned int N;
	static unsigned int Sum;
};

unsigned int Temp::N = 0;
unsigned int Temp::Sum = 0;

unsigned int Sum_Solution1(unsigned int n)
{
	Temp::Reset();

	Temp* a = new Temp[n];
	delete[]a;
	a = NULL;

	return Temp::GetSum();
}

// ====================方法二====================
class A;
A* Array[2];

class A
{
public:
	virtual unsigned int Sum(unsigned int n)
	{
		return 0;
	}
};

class B : public A
{
public:
	virtual unsigned int Sum(unsigned int n)
	{
		return Array[!!n]->Sum(n - 1) + n;
	}
};

int Sum_Solution2(int n)
{
	A a;
	B b;
	Array[0] = &a;
	Array[1] = &b;

	int value = Array[1]->Sum(n);

	return value;
}

// ====================方法三====================
typedef unsigned int (*fun)(unsigned int);

unsigned int Solution3_Teminator(unsigned int n)
{
	return 0;
}

unsigned int Sum_Solution3(unsigned int n)
{
	static fun f[2] = { Solution3_Teminator, Sum_Solution3 };
	return n + f[!!n](n - 1);
}

// ====================方法四====================
template <unsigned int n> struct Sum_Solution4
{
	enum Value { N = Sum_Solution4<n - 1>::N + n };
};

template <> struct Sum_Solution4<1>
{
	enum Value { N = 1 };
};

template <> struct Sum_Solution4<0>
{
	enum Value { N = 0 };
};