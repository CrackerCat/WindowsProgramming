﻿// 磁盘相关.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
#include"disk.h"

using namespace std;

int main()
{		
	GetDiskDriverVolumnName();

	GetDiskDriverSpaceByDriverName(L"C:\\");

	//格式化磁盘 谨慎操作
	//FormatDriverByDriverName(L"G:\\");
}
