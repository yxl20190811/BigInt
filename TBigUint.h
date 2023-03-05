#pragma once
#include <stdio.h>
#include <cassert>
#include <corecrt_memory.h>
#include <xpolymorphic_allocator.h>
class TBigInt
{
public:
	typedef unsigned int IntType;
	static const int m_MaxPerData = 1000000000;// 10�ڣ� ÿƬBigInt�����ֵ
	static const int m_OneTenthOfMaxPerData = m_MaxPerData/10;// 1�ڣ�ÿƬBigInt�ڳ���10��ǰ�����ֵ���������ֵ���Ƚ�λ��������10
	static const int m_NumberCountPerData = 9; //ÿƬBigInt�������ָ���
public:
	static void Text2Uint(
		const char* text, int size,
		IntType* ret, int& retSize
	) {

		//����������
		if (NULL == text || NULL == ret || size < 1 || retSize < 1) {
			retSize = 0;
			return;
		}
		memset((void*)ret, 0, sizeof(IntType) * retSize);
		int pos = 1;
		unsigned& data = ret[0];

		//����ÿ��ʮ��������
		for (int index = 0; index < size; ++index) {
			unsigned char ch = text[index];
			//����������ַ���������ַ���������
			if (ch < '0' || ch > '9') {
				break;
			}
			//�Ӹߵ��ͣ�ÿ������Ƭ�ζ�����10
			for (int i = pos - 1; i >= 0; --i) {
				if (ret[i] > m_OneTenthOfMaxPerData) {
					ret[i + 1] += ret[i] / (m_OneTenthOfMaxPerData);
					ret[i] = ret[i] % (m_OneTenthOfMaxPerData);
				}
				ret[i] *= 10;
			}
			//���λ����������
			ret[0] += ch - '0';
			//�жϽ�λ
			/*
			for (int i = 0; i < pos; ++i) {
				if (ret[i] > m_MaxPerData)
				{
					ret[i + 1] = ret[i] / m_MaxPerData;
					ret[i] %= m_MaxPerData;
				}
			}
			*/
			//������λ��λ��
			if (ret[pos] != 0) {
				if (pos > retSize - 1) {
					retSize = 0;
					return;
				}
				pos++;
			}
		}
		//ȫ��������ϣ��򷵻�
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
		//�������ڴ�ȫ�����
		memset(text, 0, textSize);
		int pos = textSize - 1;
		//������������ÿ������
		for (int i = 0; i < dataSize; ++i) {
			int v = data[i];
			//ÿ�����ݶ�תΪ10���������
			for (int j = 0; j < m_NumberCountPerData; ++j) {
				text[--pos] = '0' + v % 10;
				v = v / 10;
			}
		}
		//ȥ�����ֿ�ʼ��0
		int pos2 = pos;
		for (; pos2 < textSize; ++pos2) {
			if (text[pos2] != '0') {
				break;
			}
		}
		//�����ֿ������ڴ�õ���ͷ
		for (int i = pos2; i < textSize; ++i) {
			text[i-pos2] = text[i];
		}
		//������Ч�ַ�����
		textSize = textSize-pos2-1;
	}
};

