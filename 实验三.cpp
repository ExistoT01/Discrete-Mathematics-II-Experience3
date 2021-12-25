#include <iostream>
#include <vector>
#include "booster.h"
#include "Matrix.h"
using namespace std;


class solution {
public:
	//变量
	int n;//矩阵阶数
	int m;//边数
	int r;//面数
	//对偶图
	int dn;
	int dm;
	int dr;
	vector<vector<int>> adjMatrix;//邻接矩阵
	vector<vector<int>> faceMatrix;//面矩阵
	vector<vector<int>> incMatrix;//关联矩阵
	vector<vector<int>> dualAdjMatrix;//对偶图邻接矩阵
	vector<vector<int>> dualIncMatrix;//对偶图关联矩阵


	//函数
	solution() {
		this->n = 0;
		this->m = 0;
		this->r = 0;
		this->dm = 0;
		this->dn = 0;
		this->dr = 0;
	}

	void setN();
	void setAdjMatrix();
	void setFaceMatrix();
	void setIncMatrix();
	void setDualIncMatrix();
	void setDualAdjMatrix();
	void showMatrix(vector<vector<int>> matrix, int row, int col, int type);

};




int main()
{
	solution method;

	method.setN();
	method.setAdjMatrix();
	method.showMatrix(method.adjMatrix, method.n, method.n, 0);

	//平面图判断
	cout << endl;
	cout << "#==========平面图判断" << endl;
	cout << endl;

	if (canMap(method.adjMatrix, method.n, method.m))
		cout << "是平面图！" << endl;
	else
		cout << "不是平面图！" << endl;

	//读入面矩阵
	method.setFaceMatrix();

	method.showMatrix(method.faceMatrix, method.r, method.m, 2);


	//输出此平面图的顶点数 n、边数 m 和面数 r
	cout << endl;
	cout << "#==========输出此平面图的顶点数n、边数m和面数r" << endl;
	cout << endl;

	cout << "顶点数：" << method.n << " "
		<< "边数：" << method.m << " "
		<< "面数：" << method.r << endl;

	//求对偶图
	method.setDualIncMatrix();
	method.setDualAdjMatrix();

	//输出此平面图的对偶图的顶点数n*、边数m*和面数r*

	cout << endl;
	cout << "#==========输出此平面图的对偶图的顶点数n*、边数m*和面数r*" << endl;
	cout << endl;

	cout << "顶点数：" << method.dn << " "
		<< "边数：" << method.dm << " "
		<< "面数：" << method.dr << endl;

	method.showMatrix(method.dualAdjMatrix, method.dn, method.dn, 0);
	method.showMatrix(method.dualIncMatrix, method.dn, method.dm, 1);

	return 0;
}

void solution::setN()
{
	cout << endl;
	cout << "#==========读入矩阵阶数n" << endl;
	cout << endl;

	int input;
	cin >> input;
	this->n = input;
}

void solution::setAdjMatrix()
{
	cout << endl;
	cout << "#==========读入邻接矩阵" << endl;
	cout << endl;

	vector<int> tem;
	int input;

	for (int i = 0; i < n; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < n; j++)
		{
			cin >> input;
			if (j > i && input)
				m++;
			tem.push_back(input);
		}
		adjMatrix.push_back(tem);
	}

	//根据欧拉公式计算出面r的数量
	r = 2 + m - n;
}

void solution::setFaceMatrix()
{
	cout << endl;
	cout << "#==========读入面矩阵" << endl;
	cout << endl;

	vector<int> tem;
	int input;

	for (int i = 0; i < r; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < m; j++)
		{
			cin >> input;
			tem.push_back(input);
		}
		faceMatrix.push_back(tem);
	}
}

void solution::setIncMatrix()
{
	int cnt = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (adjMatrix[i][j]) {				
				incMatrix[i][cnt] = 1;
				incMatrix[j][cnt] = 1;
				cnt++;
			}
		}
	}
}

void solution::setDualIncMatrix()
{
	vector<int> tem;
	for (int i = 0; i < r; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < m; j++)
		{
			tem.push_back(0);
		}
		dualIncMatrix.push_back(tem);
	}

	int cnt = 0;
	for (int i = 0; i < r; i++)
	{
		for (int j = i + 1; j < r; j++)
		{
			for (int k = 0; k < m; k++)
			{
				if (faceMatrix[i][k] & faceMatrix[j][k]) {
					dualIncMatrix[i][cnt] = 1;
					dualIncMatrix[j][cnt] = 1;
					cnt++;
				}

			}
		}
	}

	dn = r;
	dm = m;
	dr = 2 + dm - dn;

	//showMatrix(dualIncMatrix, dn, dm, 1);
}

void solution::setDualAdjMatrix()
{
	vector<int> tem;
	for (int i = 0; i < dn; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < dn; j++)
		{
			tem.push_back(0);
		}
		dualAdjMatrix.push_back(tem);
	}


	int v1, v2;
	for (int j = 0; j < dm; j++)
	{
		v1 = v2 = 0;
		for (int i = 0; i < dn; i++)
		{
			if (dualIncMatrix[i][j])
			{
				if (!v1)
					v1 = i;
				else
					v2 = i;
			}
		}
		dualAdjMatrix[v1][v2] = 1;
		dualAdjMatrix[v2][v1] = 1;
	}

	//showMatrix(dualAdjMatrix, dn, dn, 0);

}

void solution::showMatrix(vector<vector<int>> matrix, int row, int col, int type)
{
	cout << endl;
	cout << "#==========输出";

	char ch_row = 't';
	char ch_col = 't';

	if (type == 0) {
		ch_row = 'v';
		ch_col = 'v';
		cout << "邻接矩阵" << endl;
	}
	else if (type == 1) {
		ch_row = 'v';
		ch_col = 'e';
		cout << "关联矩阵" << endl;
	}
	else if (type == 2) {
		ch_row = 'r';
		ch_col = 'e';
		cout << "面矩阵" << endl;
	}
	cout << endl;

	printf("%-5s\t", "index");
	for (int i = 0; i < col; i++)
		printf("%4c%d", ch_col, i+1);
	cout << endl;

	for (int i = 0; i < row; i++)
	{
		printf("%4c%d\t", ch_row, i+1);
		for (int j = 0; j < col; j++)
			printf("%5d", matrix[i][j]);
		cout << endl;
	}
}
