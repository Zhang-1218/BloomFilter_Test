
#include<iostream>
#include"bloomfilter.h"

using namespace std;

int main()
{
	Bloomfilter mybloom(0.001, 10, "./redlist.txt");
	mybloom.filter_init();
	cout << "��Ҫ�Ĺ�ϣ�����ĸ���: " << mybloom.hashnum() << endl;
	cout << "��Ҫ������ٸ�int : " << mybloom.sizeofpool() << endl;
	cout << "www.dubai.com�ڼ�������: " << (mybloom.is_contain("www.dubai.com") ? "��" : "����") << endl;

	cout << "www.qq.com�ڼ�������: " << (mybloom.is_contain("www.qq.com") ? "��" : "����") << endl;
	system("pause");
	return 0;
}
