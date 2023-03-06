#pragma once
#include <stdio.h>
#include <cassert>
#include <corecrt_memory.h>
#include <xpolymorphic_allocator.h>
class TBigInt
{
public:
	typedef unsigned int IntType;
	static const int m_MaxPerData = 1000000000;// 10亿， 每片BigInt的最大值
	static const int m_OneTenthOfMaxPerData = m_MaxPerData/10;// 1亿，每片BigInt在乘以10，前的最大值；超出这个值就先进位再做乘以10
	static const int m_NumberCountPerData = 9; //每片BigInt表达的数字个数
public:
	static void Text2Uint(
		//输入的字符串，以及字符串的长度
		const char* text, int textSize,
		//返回的地址，以及该内存对象的长度
		IntType* ret, int& retSize
	) {
		//检查输入参数
		if (NULL == text || NULL == ret || textSize < 1 || retSize < 1) {
			retSize = 0;
			return;
		}
		int count = textSize / 9 + 1;
		if (count >= retSize) {
			retSize = 0;
			return;
		}
		ret[count] = 0;

		//处理每个十进制数字
		char* pos = (char*)text + textSize;
		for (int index = count-1; index > 0 ; --index) {
			pos -= 9;
			int value = 0;
			for (int i = 0; i < 9; ++i) {
				value = value * 10 + pos[i] - '0';
			}
			ret[index] = value;
		}
		{
			int len = (long long)pos - (long long)text;
			int value = 0;
			for (int i = 0; i < len; ++i)
			{
				value = value * 10 + text[i] - '0';
			}
			ret[0] = value;
		}
		retSize = count;
	}
public:
	static void Uint2Text(
		IntType* data, int dataSize,
		char* ret, int& retSize
	) {
		if (dataSize * 9 >= retSize) {
			retSize = 0;
			return;
		}
		//将返回内存全部清空
		memset(ret, 0, retSize);
		int pos = 0;
		//遍历大整数的最高位数据
		{
			//最高位前的0，没有意义，去掉
			int value = data[0];
			int count = 0;
			for (; value != 0; ++count) {
				value = value / 10;
			}
			value = data[0];
			for (pos = count-1; pos >= 0; --pos) {
				ret[pos] = value%10 + '0';
				value = value / 10;
			}
			pos = count;
		}

		for (int i = 1; i < dataSize; ++i) {
			int v = data[i];
			//每块数据都转为10进制数输出
			for (int j = m_NumberCountPerData-1; j >= 0; --j) {
				ret[pos+j] = '0' + v % 10;
				v = v / 10;
			}
			pos += m_NumberCountPerData;
		}
		retSize = pos;
	}
};

