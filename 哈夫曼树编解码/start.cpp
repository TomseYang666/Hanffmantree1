#include<iostream>
#include<string>
#include<fstream>
#include <stdio.h>
using namespace std;
#include "HTNode.h"
#include "HuffmanCode.h"
int type(int a[]) 
{
	int sum = 0;
	FILE*fp;
	fopen_s(&fp, "A4Input.txt", "r");
	char ch = fgetc(fp);
	string cs;
	while (ch != EOF)
	{
		cs += ch;
		ch = fgetc(fp);
	}
	for (int i = 0; i < cs.size(); i++) 
	{
		a[cs[i] - '\0']++;
	}
	fclose(fp);
	for (int i = 0; i <128; i++)
	{
		if (a[i] != 0)
		{
			sum++;
		}
	}
	return sum;
}
void WriteBinaryFile(const char *pBinaryFileName, string code)
{
	ofstream myOut;// ifstream 类型
	myOut.open(pBinaryFileName, ios::out | ios::binary); //文本文件
	if (!myOut.is_open()) return;
	myOut.write((char*)&code, sizeof(code));
	myOut.close();
}
int main()
{
	CHTNode* h=new CHTNode[256];
	CHuffmanCode* hcode= new CHuffmanCode[128];
	int a[128] = {0};//记录字母出现次数
	int n = type(a);//记录出现过的字母个数
	const char *pBinaryFileName = "Reduced.dat";//利用此类型文件减少内存
	//哈夫曼编码
	int i, j;
	for (i = 0; i<256; i++) 
	{
		h[i].weight = 0;
		h[i].parent = -1;
		h[i].lchild = -1;
		h[i].rchild = -1;
		h[i].c = '\0';
		h[i].nb = 0;//用于记录是否被遍历到
	}
	for (i = 0; i<128; i++) //在对应的码格里记录相应的字母
	{
			h[i].c = i + ' ';
			h[i].weight = a[i];
	}
	int s1=0,s2=0;
	for (i=0; i<(128-1); i++) 
	{
		int max1=2000,max2=2000;
		int x1 = -1, x2 = -1;
		for (j = 0; j<128+i; j++) 
		{
			if ((h[j].weight<max1|| h[j].weight==max1)&&h[j].parent==-1)
			{
				max2=max1;
				x2=x1;
				max1=h[j].weight;
				x1=j;
			}
			else if ((h[j].weight<max2|| h[j].weight==max2)&& h[j].parent == -1)
			{
				max2=h[j].weight;
				x2=j;
			}
		}
		//根据每个节点信息, 将其信息存储到节点数组中
		h[x1].parent=128+i; h[x2].parent =128+i;
		h[128+i].weight = h[x1].weight + h[x2].weight;
		h[128+i].lchild = x1; h[128+i].rchild = x2;
	}
	//根据叶子节点的位置, 将01数字填充到编码数组中
	for (i = 0; i < 128; i++)
	{
		hcode[i].cb[0]='/0';
		hcode[i].cb+= h[i].c;
		int parent = h[i].parent;
		int nc=i;
		while (nc != -1)
		{
			hcode[i].cb+=(h[parent].lchild==nc?'0':'1');
			nc = parent;
			if (h[parent].parent == -1)break;//这个语句，我仿佛想了一年
			parent = h[parent].parent;
		}
	}
	//写入文档
	FILE*fp;
		fopen_s(&fp, "A4Input_HuffTree.txt", "r+");
		for (i = 0; i < 128; i++)
		{
			if (hcode[i].cb[0]!= '/0')
			{
				fprintf(fp, "%c:", h[i].c);
				for (j =1; j < hcode[i].cb.size(); j++)
				{
					fprintf(fp, "%c", hcode[i].cb[j]);
				}
				fprintf(fp, "\n");
			}
		}
		fclose(fp);
		//哈夫曼编码写入文件
		fopen_s(&fp, "A4Input.txt", "r");
		char ch = fgetc(fp);
		string cs,cs1;
		while (ch != EOF)
		{
			cs += ch;
			ch = fgetc(fp);
		}
		fclose(fp);
		fopen_s(&fp, "A4Input_Encoded.txt", "w"); cs1 = cs;
		for (int ni = 0; ni<cs.size(); ni++)
		{
			int nt;
			for (nt = 0; nt <128; nt++)
			{
				if (h[nt].c == cs[ni])
				{
					for (int na=hcode[nt].cb.size()-1; na>0; na--)
					{
						fprintf(fp,"%c", hcode[nt].cb[na]);
					}
				}
			}
		}
	fclose(fp);
	cs = { ' ' };
	fopen_s(&fp, "A4Input_Encoded.txt", "r");
	ch = fgetc(fp);
	while (ch != EOF)
	{
		if (ch!='/0')
		{
			cs+=ch;
		}
		ch = fgetc(fp);
	}
	fclose(fp);
	WriteBinaryFile(pBinaryFileName, cs);
	//哈夫曼译码
	fopen_s(&fp, "A4Input_Decoded.txt", "r+");
	i = 0, j = 0;
	int lchild = 2*128-2,rchild=2*128-2;
	for(i=0;i<cs.size();i++)
	{
		if(cs[i] == '0') 
		{
			lchild = h[lchild].lchild;
			j = lchild;
			rchild=lchild;
		}
		if(cs[i] == '1') 
		{
			rchild = h[rchild].rchild;
			j=rchild;
			lchild=rchild;
		}
		if(h[lchild].lchild==-1&&h[rchild].rchild==-1) 
		{
			fprintf(fp, "%c",h[j].c);
			lchild = rchild = 2 * 128- 2;
			j=0;
		}
	}
	fclose(fp);
	return 0;
}