
#include<iostream>
#include"bloomfilter.h"

using namespace std;

int main()
{
	Bloomfilter mybloom(0.001, 10, "./redlist.txt");
	mybloom.filter_init();
	cout << "需要的哈希函数的个数: " << mybloom.hashnum() << endl;
	cout << "需要申请多少个int : " << mybloom.sizeofpool() << endl;
	cout << "www.dubai.com在集合中吗: " << (mybloom.is_contain("www.dubai.com") ? "在" : "不在") << endl;

	cout << "www.qq.com在集合中吗: " << (mybloom.is_contain("www.qq.com") ? "在" : "不在") << endl;
	system("pause");
	return 0;
}
