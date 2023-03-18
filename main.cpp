// BigInt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "TBigUint.h"

int main()
{
    char buf[10240];
    TBigUint  i;
    i.BinText2This("111010100001110001110");
    i.toBinText(buf, sizeof(buf));

    i.toOctText(buf, sizeof(buf));
    i.OctText2This(buf);

    i.toHexText(buf, sizeof(buf));
    i.HexText2This(buf);

    const char* t = "99999999999999999999999999999999999999999999999999999999";
    i.DecText2This(t);
    i.toDecText(buf, sizeof(buf));
    if (0 != strcmp(t, buf)) {
        abort();
    }
}

