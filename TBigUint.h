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
		const char* text, int size,
		IntType* ret, int& retSize
	) {

		//检查输入参数
		if (NULL == text || NULL == ret || size < 1 || retSize < 1) {
			retSize = 0;
			return;
		}
		memset((void*)ret, 0, sizeof(IntType) * retSize);
		int pos = 1;
		unsigned& data = ret[0];

		//处理每个十进制数字
		for (int index = 0; index < size; ++index) {
			unsigned char ch = text[index];
			//碰到错误的字符，则后面字符不处理了
			if (ch < '0' || ch > '9') {
				break;
			}
			//从高到低，每个整数片段都乘以10
			for (int i = pos - 1; i >= 0; --i) {
				if (ret[i] > m_OneTenthOfMaxPerData) {
					ret[i + 1] += ret[i] / (m_OneTenthOfMaxPerData);
					ret[i] = ret[i] % (m_OneTenthOfMaxPerData);
				}
				ret[i] *= 10;
			}
			//最低位加上新数字
			ret[0] += ch - '0';
			//判断进位
			/*
			for (int i = 0; i < pos; ++i) {
				if (ret[i] > m_MaxPerData)
				{
					ret[i + 1] = ret[i] / m_MaxPerData;
					ret[i] %= m_MaxPerData;
				}
			}
			*/
			//如果最高位进位了
			if (ret[pos] != 0) {
				if (pos > retSize - 1) {
					retSize = 0;
					return;
				}
				pos++;
			}
		}
		//全部处理完毕，则返回
		retSize = pos;
		return;
	}
public:
	static void Uint2Text(
		IntType* data, int dataSize,
		char* text, int& textSize
	) {
		if (dataSize * 9 >= textSize) {
			textSize = 0;
			return;
		}
		//将返回内存全部清空
		memset(text, 0, textSize);
		int pos = textSize - 1;
		//遍历大整数的每块数据
		for (int i = 0; i < dataSize; ++i) {
			int v = data[i];
			//每块数据都转为10进制数输出
			for (int j = 0; j < m_NumberCountPerData; ++j) {
				text[--pos] = '0' + v % 10;
				v = v / 10;
			}
		}
		//去掉数字开始的0
		int pos2 = pos;
		for (; pos2 < textSize; ++pos2) {
			if (text[pos2] != '0') {
				break;
			}
		}
		//将数字开宝到内存得到开头
		for (int i = pos2; i < textSize; ++i) {
			text[i-pos2] = text[i];
		}
		//返回有效字符个数
		textSize = textSize-pos2-1;
	}
};

