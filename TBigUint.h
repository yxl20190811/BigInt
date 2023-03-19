#pragma once
#include <stdio.h>
#include <cassert>
#include <corecrt_memory.h>
#include <xpolymorphic_allocator.h>

//大整数，可以表达一个任意大小的的无符号整数。
class TBigUint
{
public:
	TBigUint();
	~TBigUint();
public: 
	void BinText2This(const char* text);//从二进制字符串（忽略无效字符）转为大整数
	void OctText2This(const char* text);//从八进制字符串（忽略无效字符）转为大整数
	void DecText2This(const char* text);//从十进制字符串（忽略无效字符）转为大整数
	void HexText2This(const char* text);//从十六进制字符（忽略无效字符）串转为大整数
public:
	int toBinText(char* buf, int size);//将当前大整数转为2进制字符串
	int toOctText(char* buf, int size);//将当前大整数转为2进制字符串
	int toDecText(char* buf, int size);//将当前大整数转为2进制字符串
	int toHexText(char* buf, int size);//将当前大整数转为2进制字符串
private:
	static char toHexChar(char c); //将整数转为10进制的字符
	static char toHex(char c); //将十六进制字符转为整数
private:
	char* m_buf;
	int m_BufSize;
};

