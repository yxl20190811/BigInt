#include "TBigUint.h"

#define STACK_TMP_SIZE  1024

TBigUint::TBigUint()
{
	m_buf = NULL;
	m_BufSize = 0;
}

TBigUint::~TBigUint()
{
	if (NULL != m_buf) {
		free(m_buf);
		m_buf = NULL;
	}

}


void TBigUint::BinText2This(const char* text)
{
	//分配一块内存，以便存放转换后的整数
	int len = strlen(text);
	if (len <= 0) {
		m_BufSize = 0;
		return;
	}
	char* buf = (char*)malloc(len / 8 + 1);
	if (NULL == buf) { abort(); }
	m_buf = buf;
	m_BufSize = 0;
	
	//遍历每个二进制字符转为二进制整数
	int count = 0;
	int pos = 0;
	unsigned char value = 0;
	for (int i = len - 1; i >= 0; --i) {
		unsigned char ch = text[i];
		if (ch < '0' || ch > '1') {
			continue;
		}
		ch -= '0';

		//将二级制字符放到整数的对应位上
		value |= (ch << count);
		++count;
		//长度超出一个字节8位后，将整个字节放到临时内存对应字节去
		if (count >= 8) {
			buf[pos++] = value;
			value = 0;
			count = 0;
		}

	}
	//最后的字符转为的整数，也需要放到临时内存的对应字节去
	if (0 != value) {
		buf[pos++] = value;
	}

	//记录转换后到真正的长度
	m_BufSize = pos;
}

int TBigUint::toBinText(char* buf, int size)
{
	memset(buf, 0, size);
	unsigned char mask[] = {1,2,4,8,16,32,64,128};
	int pos = 0;
	bool isFirstCh = true;
	for (int i = m_BufSize-1; i >= 0; --i) {
		unsigned char ch = m_buf[i];
		for (int j = 7; j >= 0; --j) {
			 char ch1 = ((ch & mask[j]) >> j) + '0';
			 //最高位的'0'省略
			 if (isFirstCh && ch1 == '0') {
				 continue;
			 }
			 buf[pos++] = ch1;
			 isFirstCh = false;
			 //如果传入的内存大小不够，则直接返回后面的字符不再输出
			 if (pos >= size-2) {
				 buf[pos] = 0;
				 return pos;
			 }
		}
		
	}
	buf[pos]  = 0;
	return pos;
}

int TBigUint::toOctText(char* buf, int size)
{
	memset(buf, 0, size);
	int pos = 0;
	int index = 0;
	bool isFirstZero = true;
	//最高位的字符，需要将无效的最高位置0. 所以专门处理
	if (m_BufSize % 3 != 0) {
		unsigned int value = *(unsigned int*)(buf + m_BufSize - m_BufSize%3);
		if (m_BufSize % 3 == 2) {
			value &= 0x0000ffff;
		}
		if (m_BufSize % 3 == 1) {
			value &= 0x000000ff;
		}
		for (int j = 7; j >= 0; --j) {
			char ch = (value >> (j * 3)) & 0x00000007;
			if (isFirstZero && 0 == ch) {
				continue;
			}
			isFirstZero = false;
			buf[pos] = ch + '0';
			++pos;
		}
		for (int j = 0; j < 8; ++j) {
			buf[pos] = (value & 0x00000007) + '0';
			++pos;
			value <<= 3;
		}
		index = m_BufSize - m_BufSize % 3 - 3;
	}
	else {
		index = m_BufSize - 3;
	}



	for (; index >=0; index -= 3) {
		unsigned int value = *(unsigned int*)(m_buf + index);
		for (int j = 7; j >= 0; --j) {
			char ch = (value >> (j * 3)) & 0x00000007;
			if (isFirstZero && 0 == ch) {
				continue;
			}
			isFirstZero = false;
			buf[pos] = ch + '0';
			++pos;
		}
	}
	return pos;
}


void TBigUint::OctText2This(const char* text)
{
	char  tmp[STACK_TMP_SIZE]{ 0 };
	char* buf = tmp;
	int len = strlen(text);
	if (len*3/8 + 1 >= STACK_TMP_SIZE-1) {
		buf = (char*)malloc(len / 3 + 2);
		if (NULL == buf) {
			abort();
		}
	}
	int count = 0;
	int pos = 0;
	unsigned int value = 0;
	for (int i = len - 1; i >= 0; --i) {
		unsigned int ch = text[i];
		if (ch < '0' || ch > '7') {
			continue;
		}
		ch -= '0';
		value |= (ch << (count*3));
		++count;
		if (count>= 8) {
			memcpy(buf + pos, &value, 3);
			pos += 3;
			count = 0;
			value = 0;
		}

	}
	if (0 != value) {
		memcpy(buf+pos, &value, 3);
		pos += 3;
	}
	if (buf != tmp) {
		m_buf = buf;
	}
	else
	{
		if (pos >= m_BufSize) {
			if (NULL != m_buf) {
				free(m_buf);
			}
			m_buf = (char*)malloc(pos + 1);
		}
		memcpy(m_buf, buf, pos);
	}
	m_BufSize = pos;
}

char TBigUint::toHexChar(char c) {
	c = c & 0x0f;
	if (c >= 0 && c <= 9) {
		return c + '0';
	}
	else if (c >= 10 && c <= 15) {
		return c + 'a' - 10;
	}
	abort();
}

int TBigUint::toHexText(char* buf, int size)
{
	memset(buf, 0, size);
	if (m_BufSize * 2 >= size) {
		abort();
	}
	int pos = 0;
	for (int i = m_BufSize - 1; i >= 0; --i) {
		unsigned char ch = m_buf[i];
		buf[pos++] = toHexChar(ch>>4);
		buf[pos++] = toHexChar(ch);
	}
	return pos;
}


void TBigUint::HexText2This(const char* text)
{
	char  tmp[STACK_TMP_SIZE]{ 0 };
	char* buf = tmp;
	int len = strlen(text);
	if (len/2 + 1 >= STACK_TMP_SIZE - 1) {
		buf = (char*)malloc(len/2 + 2);
		if (NULL == buf) {
			abort();
		}
	}

	int count = 0;
	int pos = 0;
	for (int i = len - 1; i >= 0; --i) {
		unsigned char ch = text[i];
		if (ch >= '0' && ch <= '9') {
			ch -= '0';
		}
		else if (ch >= 'a' && ch <= 'f') {
			ch = ch - 'a' + 10;
		}
		else if (ch >= 'A' && ch <= 'F') {
			ch = ch - 'A' + 10;
		}
		else {
			abort();
		}

		if (0 == count) {
			buf[pos] = ch;
			count = 1;
		}
		else {
			buf[pos++] |= ch<<4;
			count = 0;
		}
	}
}

void TBigUint::DecText2This(const char* text) {
	int len = strlen(text);
	unsigned long long buf[10000] = { 0 };
	int pos = 1;
	unsigned long long value = 0;
	int count = 0;
	for (int i = 0; i < len; ++i) {
		char ch = text[i];
		if (ch < '0' || ch > '9')  continue;
		value = value * 10 + ch - '0';
		++count;
		if (count >= 9) {
			count = 0;
			buf[0] = buf[0] * 1000000000 + value;
			value = 0;
			for (int j = 1; j < pos; ++j) {
				buf[j] *= 1000000000;
				buf[j] += buf[j - 1] >> 32;
				buf[j - 1] &= 0xffffffff;
			}
			buf[pos] += buf[pos - 1] >> 32;
			buf[pos - 1] &= 0xffffffff;
			if (buf[pos] > 0) ++pos;
		}
	}
	if (count > 0) {
		unsigned long long radis = 1;
		for (int j = 0; j < count; ++j) radis *= 10;
		buf[0] = buf[0] * radis + value;

		for (int j = 1; j < pos; ++j) {
			buf[j] *= radis;
			buf[j] += buf[j - 1] >> 32;
			buf[j - 1] &= 0xffffffff;
		}
		buf[pos] += buf[pos - 1] >> 32;
		buf[pos - 1] &= 0xffffffff;
		if (buf[pos] > 0) ++pos;
	}
	if (NULL != m_buf) { free(m_buf); }
	m_BufSize = pos * 4;
	m_buf = (char*)(malloc(m_BufSize));
	for (int i = 0; i < pos; ++i) {
		*(unsigned int*)(m_buf + i * 4) = (unsigned int)buf[i];
	}
	
}


int TBigUint::toDecText(char* buf, int size)
{
	memset(buf, 0, size);
	unsigned long long data[1000];
	memset(data, 0, sizeof(data));
	int len = m_BufSize;
	for (int i = len / 4 - 1; i >= 0; --i) {
		data[i] = *(unsigned int*)(m_buf + 4*i);
	}
	len = (len + 1) / 4;
	int pos = 0;
	while (true) {
		for (int i = len - 1; i >= 1; --i) {
			data[i - 1] += (data[i] % 1000000000)<<32;
			data[i] /= 1000000000;
		}
		if (data[len - 1] <= 0) { 
			--len;
			if (len <= 0) {
				break;
			}
		}
		while (true) 
		{
			buf[pos++] = (data[0] % 10) + '0';
			data[0] /= 10;
			if (data[0] < 10000000000) { break; }
		}
	}
	return pos;
}
