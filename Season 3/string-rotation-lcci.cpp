/**
 * @file string-rotation-lcci.cpp
 * @author 
 * @brief 面试题 01.09. 字符串轮转
 * @version 0.1
 * @date 2022-07-20
 * @remark https://leetcode.cn/problems/string-rotation-lcci/
 * @copyright Copyright (c) 2022
 * 
 */
#include <string>
using namespace std;

class Solution
{
public:
    bool isFlipedString(string s1, string s2)
    {
        if(s1.length() != s2.length())
            return false;
        return (s1 + s1).find(s2) != string::npos;
    }
};