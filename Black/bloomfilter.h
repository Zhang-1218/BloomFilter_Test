#ifndef _BLOOMFILTER_
#define _BLOOMFILTER_

#include<iostream>
#include<string>
#include<math.h>
#include<stdio.h>
#include <vector>
#include "hash.h"
#include <fstream>//ifstream���ļ���ofstreamд�ļ���fstream��д�ļ�

using namespace std;

class Bloomfilter{
public:
	Bloomfilter(double err_rate, int num, char* path);	//�����������ĵ�·��������������������ʧ���ʣ�ע�����õ��Ĺ�ϣ��������k��Ҫ������hashtable��size
	~Bloomfilter();
	bool is_contain(const char* str);	//�鿴�ַ����Ƿ��������д���
	int hashnum();				//����k
	// double real_precision();	//������ʵ��ʧ����
	int sizeofpool();				//����len
	void filter_init();		//��ʼ����¡������
private:
	void listinit();				//��path·�����ĵ�������ÿһ���������ڴ�bitpool��
	int  hashtable_init();			//�Ѽ�����ϣ�������뵽vector<unsigned (*)(const char*)> hastable�����У��������k
	int len;
	char* mypath;			//�ļ���·����ͨ�����캯������·��
	Bloomfilter() = delete;
	double precision;
	int *bitpool;		//��Ҫ�ڴ�ĳ���,�ڹ��캯��������
	int bitpoollen;		//��Ҫ�Ķ�����λ��m
	int hashfuncnum;	//��Ҫ�Ĺ�ϣ�����ĸ���k�� k <=hashtable.size();
	int samplenum;		//�������������캯������
	vector<unsigned int(*)(const char*)> hashtable;	//��ż����ַ�����ϣֵ�Ĺ�ϣ����
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
		cout << "��ϵ���еĺ�������,�����" << endl;
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
		hashval = hashval % (len * 32); //len*32Ϊbitpool����λ��
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
	while (!feof(fp))//�ļ�û�н���
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
