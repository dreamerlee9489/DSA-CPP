/**
 * @file count-primes.cpp
 * @author dreamerlee9489@outlook.com
 * @brief 204. 计数质数
 * @version 0.1
 * @date 2022-08-07
 * @link https://leetcode.cn/problems/count-primes/
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	int countPrimes(int n) {
		int res = 0;
		vector<bool> dp(n, true);
		for (size_t i = 2; i < n; i++) {
			if (dp[i]) {
				++res;
				// 如果i是质数，那么i的所有倍数都不是质数
				// 没必要从2i开始，2i必然是2的倍数
				if (i * i < n)
					for (size_t j = i * i; j < n; j += i)
						dp[j] = false;
			}
		}
		return res;
	}
};

int main(int argc, char const* argv[]) {
	cout << Solution().countPrimes(0) << "\n";
	cout << Solution().countPrimes(1) << "\n";
	cout << Solution().countPrimes(10) << "\n";
	cout << Solution().countPrimes(499979) << "\n";
	return 0;
}
