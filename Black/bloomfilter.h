#ifndef _BLOOMFILTER_
#define _BLOOMFILTER_

#include<iostream>
#include<string>
#include<math.h>
#include<stdio.h>
#include <vector>
#include "hash.h"
#include <fstream>//ifstream读文件，ofstream写文件，fstream读写文件

using namespace std;

class Bloomfilter{
public:
	Bloomfilter(double err_rate, int num, char* path);	//传入样本的文档路径，样本个数，期望的失误率，注意计算得到的哈希函数个数k需要不大于hashtable的size
	~Bloomfilter();
	bool is_contain(const char* str);	//查看字符串是否在样本中存在
	int hashnum();				//返回k
	// double real_precision();	//返回真实的失误率
	int sizeofpool();				//返回len
	void filter_init();		//初始化布隆过滤器
private:
	void listinit();				//打开path路径的文档，计算每一行样本到内存bitpool中
	int  hashtable_init();			//把几个哈希函数加入到vector<unsigned (*)(const char*)> hastable容器中，必须大于k
	int len;
	char* mypath;			//文件的路径，通过构造函数传入路径
	Bloomfilter() = delete;
	double precision;
	int *bitpool;		//需要内存的长度,在构造函数中申请
	int bitpoollen;		//需要的二进制位数m
	int hashfuncnum;	//需要的哈希函数的个数k， k <=hashtable.size();
	int samplenum;		//样本个数，构造函数传入
	vector<unsigned int(*)(const char*)> hashtable;	//存放计算字符串哈希值的哈希函数
};

double lg2(double n)
{
	return log(n) / log(2);
}


using namespace std;

int Bloomfilter::hashtable_init()
{
	hashtable.push_back(*JSHash);
	hashtable.push_back(*RSHash);
	hashtable.push_back(*SDBMHash);
	hashtable.push_back(*APHash);
	hashtable.push_back(*BKDRHash);
	hashtable.push_back(*Hash1);
	hashtable.push_back(*Hash2);
	hashtable.push_back(*Hash3);
	hashtable.push_back(*Hash4);
	hashtable.push_back(*Hash5);
	return hashtable.size();

}

Bloomfilter::Bloomfilter(double err_rate, int num, char* path)
{
	mypath = path;
	samplenum = num;
	bitpoollen = -((samplenum*log(err_rate)) / (log(2)*log(2)));
	hashfuncnum = 0.7*(bitpoollen / samplenum);
	len = bitpoollen / 32 + 1;
	bitpool = new int[len];
}
int Bloomfilter::hashnum()
{

	return hashfuncnum;
}
int Bloomfilter::sizeofpool()
{

	return len;
}

void Bloomfilter::filter_init()
{
	hashtable_init();
	if (hashfuncnum > hashtable.size())
	{
		cout << "哈系表中的函数不足,请添加" << endl;
		exit(0);
	}
	listinit();
}

bool Bloomfilter::is_contain(const char* str)
{
	printf("%s\n", str);
	int  hashval;
	for (int i = 0; i != hashfuncnum; i++)
	{
		hashval = hashtable[i](str);
		hashval = hashval % (len * 32); //len*32为bitpool的总位数
		cout << hashval << endl;

		if (bitpool[hashval / 32] & (0x1 << (hashval % 32)))
		{
			continue;
		}
		else
		{
			return false;
		}
			

	}
	return true;
}
void Bloomfilter::listinit()
{
	FILE* fp;
	size_t length = 0;
	fp = fopen(mypath, "r+");
	if (fp == nullptr)
	{
		perror("open file error");
		exit(1);
	}

	int hashval;
	char* p;


	char buf[256];//char buf[100] = { 0 };
	while (!feof(fp))//文件没有结束
	{
		char* p = fgets(buf, sizeof(buf), fp);
		if (p != NULL)
		{
			printf("buf = %s", buf);
			printf("%s", p);
		}
		else
		{
			break;
		}

		while (*p != '\n')
		{
			p++;
		}
		*p = '\0';

		for (int i = 0; i != hashfuncnum; i++)
		{
			hashval = hashtable[i](buf);
			hashval = hashval % (len * 32);
			cout << hashval << endl;
			bitpool[hashval / 32] |= (0x1 << (hashval % 32));
		}
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
}

Bloomfilter::~Bloomfilter()
{
	delete[]bitpool;
}


#endif
