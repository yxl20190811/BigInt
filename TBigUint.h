#pragma once
#include <stdio.h>
#include <cassert>
#include <corecrt_memory.h>
#include <xpolymorphic_allocator.h>
class TBigUint
{
private:
	char  *m_buf;
	int m_BufSize;
public:
	TBigUint();
	~TBigUint();
public: 
	void BinText2This(const char* text);//二进制转为大整数
	void OctText2This(const char* text);//八进制转为大整数
	void DecText2This(const char* text);//十进制转为大整数
	void HexText2This(const char* text);//十六进制转为大整数
public:
	int toBinText(char* buf, int size);
	int toOctText(char* buf, int size);
	int toDecText(char* buf, int size);
	int toHexText(char* buf, int size);
private:
	static char toHexChar(char c);
	static char toHex(char c);
};

