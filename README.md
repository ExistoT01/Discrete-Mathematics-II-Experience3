# 离散数学 II 实验三(实验报告)

20020007095-叶鹏

盛艳秀-老师

## 实验题目

平面图判定及对偶图的求解

## 实验目的

1. 了解平面图的定义及其判定算法
2. 掌握平面图的对偶图的求解方法
3. 掌握平面图与其对偶图之间的点、面和边的关系

## 实验要求

给定一无向简单连通图的相邻矩阵

1. 判定此图是否是平面图
2. 若是平面图，输出此图的面矩阵
3. 输出此平面图的顶点数n、边数m和面数r
4. 输出此平面图的对偶图的顶点数n\*、边数m\*和面数r\*
5. 输出此平面图的对偶图的关联矩阵或邻接矩阵

## 实验内容和实验步骤

### 需求分析

1. 输入的形式和输入值的范围

   - 输入形式为：输入邻接矩阵的阶数 $n$ ，接着以矩阵的形式输入邻接矩阵
   - 输入值范围：因为使用可扩展数组，理论上 $n$ 没有限制

2. 输出的形式

   - 输出形式：

     - 相邻矩阵：默认第 $i$ 行对应顶点 $v_i$
     - 关联矩阵：默认第 $i$ 行对应顶点 $v_i$，第 $j$ 列对应边 $e_j$
     - 面矩阵：$M=(m_{ij})_{r×m}$ 

     $m_{ij}=\begin{cases}1 & \text{ej是ri的边界} \\  0 & \text{ej不是ri的边界}\end{cases}$

3. 程序实现的功能

   给定一无向简单连通图的相邻矩阵，判定此图是否是平面图，若是平面图，输出此图的面矩阵、顶点数n、边数m和面数r，输出此平面图的对偶图的顶点数n\*、边数m\*、面数r\*、关联矩阵和邻接矩阵

### 概要设计

- 定义一个`solution`类，所有的成员函数与成员变量都定义在类中，矩阵采用`vector`储存

```c++
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
	}

	void setN();
	void setAdjMatrix();
	void setFaceMatrix();
	void setIncMatrix();
	void setDualIncMatrix();
	void setDualAdjMatrix();
	void showMatrix(vector<vector<int>> matrix, int row, int col, int type);

};
```

- 接着我们就在`main`函数中实例化类，并调用类的相关函数即可
- 因为之后会用到求矩阵的秩，所以头文件内包含`Matrix.h`文件，使用其中的求秩函数
- 判断是否是平面图用到欧拉定理+`kuratowski`定理，单独列出一个头文件`booster.h`，封装判断函数`canMap`

### 详细设计

#### 判定此图是否是平面图

1. 欧拉定理+`kuratowski`定理判断是否是平面图

   `kuratowski`定理：

   *G* 是平面图当且仅当 *G* 中不含与 *K*~5~ 或 *K*~3,3~ 同胚的子图

   将判断函数`canMap`封装在头文件`booster.h`内，使用时传入参数矩阵即可判断

```c++
struct Graph {
    map<int, int> head;
    vector<int> next, last, val, att;
    int atp, atpPos;
    void clear() {
        head.clear(), next.clear(), last.clear(), val.clear(), att.clear(), atp = atpPos = 0;
        next.push_back(0), last.push_back(0), val.push_back(0);
        next.push_back(0), last.push_back(0), val.push_back(0);
    }
    Graph() { clear(); }
    void add(int x, int y) {
        next.push_back(head[x]), last.push_back(y), val.push_back(1), head[x] = next.size() - 1;
    }
    const bool operator < (const Graph& temp) const {
        return atp < temp.atp;
    }
}Tot;

void getAtp(Graph& G) {
    sort(G.att.begin(), G.att.end()), G.atp = 0;
    for (int i = 1; i <= PlaneNum; i++) {
        if (book[i].size() < G.att.size()) continue;
        int now = 0;
        for (int j = 0; j < G.att.size(); j++) {
            while (now < book[i].size() - 1 && book[i][now] < G.att[j]) now++;
            if (book[i][now] != G.att[j]) break;
            else if (j == G.att.size() - 1)
                G.atp++, G.atpPos = i;
        }
    }
}

void embed(int pos) {
    for (int i = 1; i <= sta[0]; i++) isEmbed[sta[i]] = true;
    int l = 0, r = Plane[pos].size() - 1;
    while (Plane[pos][l] != sta[1] && Plane[pos][l] != sta[sta[0]]) l++;
    while (Plane[pos][r] != sta[1] && Plane[pos][r] != sta[sta[0]]) r--;
    vector<int> temp1, temp2;
    for (int i = 0; i < l; i++) temp1.push_back(Plane[pos][i]);
    if (Plane[pos][l] == sta[1]) for (int i = 1; i <= sta[0]; i++) temp1.push_back(sta[i]);
    else for (int i = sta[0]; i >= 1; i--) temp1.push_back(sta[i]);
    for (int i = r + 1; i < Plane[pos].size(); i++) temp1.push_back(Plane[pos][i]);
    for (int i = r - 1; i > l; i--) temp2.push_back(Plane[pos][i]);
    if (Plane[pos][l] == sta[1]) for (int i = 1; i <= sta[0]; i++) temp2.push_back(sta[i]);
    else for (int i = sta[0]; i >= 1; i--) temp2.push_back(sta[i]);
    Plane[pos] = book[pos] = temp1, ++PlaneNum;
    Plane[PlaneNum] = book[PlaneNum] = temp2;
    sort(book[pos].begin(), book[pos].end()), sort(book[PlaneNum].begin(), book[PlaneNum].end());
}

bool match(int x, int goal, Graph& G) {
    vis[x] = true;
    for (int l = G.head[x]; l; l = G.next[l]) {
        int y = G.last[l];
        if (vis[y]) continue;
        if (y == goal || (!isEmbed[y] && match(y, goal, G))) {
            G.val[l] = G.val[l ^ 1] = 0;
            if (y == goal) sta[++sta[0]] = y;
            sta[++sta[0]] = x;
            return true;
        }
    }
    return false;
}

void findGraph(Graph& G, int l, Graph& ret) {
    int x = G.last[l], fa = G.last[l ^ 1];
    ret.add(x, fa), ret.add(fa, x), G.val[l] = G.val[l ^ 1] = 0;
    if (!isEmbed[x]) for (int lk = G.head[x]; lk; lk = G.next[lk]) {
        if (G.val[lk]) findGraph(G, lk, ret);
    }
    else if (!vis[x])
        ret.att.push_back(x), vis[x] = true;
}

bool Solve(list<Graph>& Lis) {
    if (!Lis.size()) return true;
    list<Graph>::iterator it = Lis.begin();
    int cnt = Lis.size() - 1;
    while (!Lis.empty()) {
        Graph& Now = *it;
        getAtp(Now), cnt++;
        if (!Now.atp) return false;
        if (cnt == Lis.size() || Now.atp == 1) {
            memset(vis, 0, sizeof(vis));
            sta[0] = 0, match(Now.att[0], Now.att[1], Now);
            embed(Now.atpPos), memset(vis, 0, sizeof(vis));
            for (int j = 2; j < sta[0]; j++) for (int l = Now.head[sta[j]]; l; l = Now.next[l]) if (Now.val[l]) {
                Graph temp;
                findGraph(Now, l, temp);
                if (!vis[sta[j]]) temp.att.push_back(sta[j]);
                for (int k = 0; k < temp.att.size(); k++) vis[temp.att[k]] = 0;
                Lis.push_back(temp);
            }
            list<Graph>::iterator temp = it++;
            Lis.erase(temp), cnt = 0, it--;
        }
        it++;
        if (it == Lis.end()) it = Lis.begin();
    }
    return true;
}

void Tarjan(int x, int fa, vector<Graph>& ret) {
    dfn[x] = low[x] = ++dfn[0];
    for (int l = Tot.head[x]; l; l = Tot.next[l]) {
        int y = Tot.last[l];
        if (y == fa) continue;
        if (!dfn[y]) Tarjan(y, x, ret), low[x] = min(low[x], low[y]);
        else low[x] = min(low[x], dfn[y]);
    }
    if (dfn[x] <= low[x]) {
        Graph temp;
        for (int l = Tot.head[x]; l; l = Tot.next[l]) if (Tot.val[l] && dfn[Tot.last[l]] > dfn[x])
            findGraph(Tot, l, temp);
        ret.push_back(temp);
    }
}

void findCircle(Graph& G) {
    int x = G.last[2];
    while (!vis[x]) {
        vis[x] = true;
        for (int l = G.head[x]; l; l = G.next[l]) if ((l ^ 1) != sta[sta[0]]) {
            x = G.last[l], sta[++sta[0]] = l;
            break;
        }
    }
    int l = 1, r = sta[0];
    while (G.last[sta[l] ^ 1] != x) l++;
    sta[0] = 0;
    for (int i = l; i <= r; i++)
        G.val[sta[i]] = G.val[sta[i] ^ 1] = 0, sta[++sta[0]] = G.last[sta[i] ^ 1];
}

bool canMap(vector<vector<int>> matrix,int n,int m) {
	vector<Graph> Div;
	Tot.clear();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j])
                Tot.add(i, j);
        }
    }

    if (m > 3 * n - 6 && m > 1)
        return false;

	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
	memset(isEmbed, 0, sizeof(isEmbed));
	memset(vis, 0, sizeof(vis));

	for (int i = 1; i <= n; i++) if (!dfn[i])
		Tarjan(i, -1, Div);

	bool flag = true;

	for (int i = 0; i < Div.size(); i++) {
		if (!Div[i].head.size())
            continue;

		sta[0] = 0, findCircle(Div[i]);

		Plane[1].push_back(sta[1]), Plane[1].push_back(sta[sta[0]]);
		
        embed(1);

		list<Graph> ret;
		memset(vis, 0, sizeof(vis));

		for (int j = 1; j <= sta[0]; j++) for (int l = Div[i].head[sta[j]]; l; l = Div[i].next[l]) if (Div[i].val[l]) {
			Graph temp;
			findGraph(Div[i], l, temp);
			if (!vis[sta[j]]) 
                temp.att.push_back(sta[j]);
			for (int k = 0; k < temp.att.size(); k++)
                vis[temp.att[k]] = 0;
			ret.push_back(temp);
		}

		flag &= Solve(ret);
		for (int j = 1; j <= PlaneNum; j++) 
            Plane[j].clear(), book[j].clear();
		PlaneNum = 1;

		if (!flag) break;
	}
    if (flag)
        return true;
    else
        return false;
}
```

#### 输出此图的面矩阵

1. 读入面矩阵后，将矩阵打印出来即可，这次依然采用矩阵输出函数`showMatrix`

```c++
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
```

#### 输出此平面图的顶点数n、边数m和面数r

1. 顶点数和边数在读入邻接矩阵时已获知，面数r可通过欧拉公式计算得出

   欧拉公式： `n - m + r = 2`

   输出即可

   ```c++
   	//输出此平面图的顶点数 n、边数 m 和面数 r
   	cout << endl;
   	cout << "#==========输出此平面图的顶点数n、边数m和面数r" << endl;
   	cout << endl;
   
   	cout << "顶点数：" << method.n << " "
   		<< "边数：" << method.m << " "
   		<< "面数：" << method.r << endl;
   ```

#### 输出此平面图的对偶图的顶点数n\*、边数m\*和面数r\*

1. 要输出对偶图相关信息，我们首先得求出对偶图，我们从面矩阵入手，平面图中的面既是对偶图中的点，如果平面图中一条边是两个面的边界，则说明与这条边关联的两个面在对偶图中有边连接，既对偶图中这两个点有一条边连接，由此我们可以从面矩阵生成出对偶图的基本关联矩阵

   ```c++
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
   }
   ```

2. 有了关联矩阵我们便很容易得到邻接矩阵

   ```c++
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
   }
   ```

3. 在求关联矩阵与邻接矩阵的过程中，我们可以获得对偶图的顶点数`n*`、边数`m*`，根据**对偶图的性质**

   - 任何平面图的**对偶图总是连通的**
   - 如果G是平面图,则G\*也是平面图
   
4. 又对于**连通平面图**，有欧拉公式

   - `n - m + r = 2`

   因此面数`*r`也可求得，输出即可

   ```c++
   //输出此平面图的对偶图的顶点数n*、边数m*和面数r*
   	cout << endl;
   	cout << "#==========输出此平面图的对偶图的顶点数n*、边数m*和面数r*" << endl;
   	cout << endl;
   
   	cout << "顶点数：" << method.dn << " "
   		<< "边数：" << method.dm << " "
   		<< "面数：" << method.dr << endl;
   ```

#### 输出此平面图的对偶图的关联矩阵或邻接矩阵

1. 相关矩阵我们已求得，直接输出即可

```c++
	method.showMatrix(method.dualAdjMatrix, method.dn, method.dn, 0);
	method.showMatrix(method.dualIncMatrix, method.dn, method.dm, 1);
```

## 实验测试数据、代码及相关结果分析

### 源代码

#### booster.h

```c++
/*booster.h*/
#pragma once
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <map>
#include <list>
using namespace std;

inline long long read() {
    long long ans = 0, f = 1;
    char ch = getchar();
    while (!isdigit(ch))
        f *= (ch == '-') ? -1 : 1, ch = getchar();
    do ans = (ans << 1) + (ans << 3) + (ch ^ 48), ch = getchar();
    while (isdigit(ch));
    return ans * f;
}

const int MAXN = 205;
int sta[MAXN], dfn[MAXN], low[MAXN], vis[MAXN], isEmbed[MAXN];
vector<int> Plane[MAXN << 1], book[MAXN << 1];
int PlaneNum = 1;

struct Graph {
    map<int, int> head;
    vector<int> next, last, val, att;
    int atp, atpPos;
    void clear() {
        head.clear(), next.clear(), last.clear(), val.clear(), att.clear(), atp = atpPos = 0;
        next.push_back(0), last.push_back(0), val.push_back(0);
        next.push_back(0), last.push_back(0), val.push_back(0);
    }
    Graph() { clear(); }
    void add(int x, int y) {
        next.push_back(head[x]), last.push_back(y), val.push_back(1), head[x] = next.size() - 1;
    }
    const bool operator < (const Graph& temp) const {
        return atp < temp.atp;
    }
}Tot;

void getAtp(Graph& G) {
    sort(G.att.begin(), G.att.end()), G.atp = 0;
    for (int i = 1; i <= PlaneNum; i++) {
        if (book[i].size() < G.att.size()) continue;
        int now = 0;
        for (int j = 0; j < G.att.size(); j++) {
            while (now < book[i].size() - 1 && book[i][now] < G.att[j]) now++;
            if (book[i][now] != G.att[j]) break;
            else if (j == G.att.size() - 1)
                G.atp++, G.atpPos = i;
        }
    }
}

void embed(int pos) {
    for (int i = 1; i <= sta[0]; i++) isEmbed[sta[i]] = true;
    int l = 0, r = Plane[pos].size() - 1;
    while (Plane[pos][l] != sta[1] && Plane[pos][l] != sta[sta[0]]) l++;
    while (Plane[pos][r] != sta[1] && Plane[pos][r] != sta[sta[0]]) r--;
    vector<int> temp1, temp2;
    for (int i = 0; i < l; i++) temp1.push_back(Plane[pos][i]);
    if (Plane[pos][l] == sta[1]) for (int i = 1; i <= sta[0]; i++) temp1.push_back(sta[i]);
    else for (int i = sta[0]; i >= 1; i--) temp1.push_back(sta[i]);
    for (int i = r + 1; i < Plane[pos].size(); i++) temp1.push_back(Plane[pos][i]);
    for (int i = r - 1; i > l; i--) temp2.push_back(Plane[pos][i]);
    if (Plane[pos][l] == sta[1]) for (int i = 1; i <= sta[0]; i++) temp2.push_back(sta[i]);
    else for (int i = sta[0]; i >= 1; i--) temp2.push_back(sta[i]);
    Plane[pos] = book[pos] = temp1, ++PlaneNum;
    Plane[PlaneNum] = book[PlaneNum] = temp2;
    sort(book[pos].begin(), book[pos].end()), sort(book[PlaneNum].begin(), book[PlaneNum].end());
}

bool match(int x, int goal, Graph& G) {
    vis[x] = true;
    for (int l = G.head[x]; l; l = G.next[l]) {
        int y = G.last[l];
        if (vis[y]) continue;
        if (y == goal || (!isEmbed[y] && match(y, goal, G))) {
            G.val[l] = G.val[l ^ 1] = 0;
            if (y == goal) sta[++sta[0]] = y;
            sta[++sta[0]] = x;
            return true;
        }
    }
    return false;
}

void findGraph(Graph& G, int l, Graph& ret) {
    int x = G.last[l], fa = G.last[l ^ 1];
    ret.add(x, fa), ret.add(fa, x), G.val[l] = G.val[l ^ 1] = 0;
    if (!isEmbed[x]) for (int lk = G.head[x]; lk; lk = G.next[lk]) {
        if (G.val[lk]) findGraph(G, lk, ret);
    }
    else if (!vis[x])
        ret.att.push_back(x), vis[x] = true;
}

bool Solve(list<Graph>& Lis) {
    if (!Lis.size()) return true;
    list<Graph>::iterator it = Lis.begin();
    int cnt = Lis.size() - 1;
    while (!Lis.empty()) {
        Graph& Now = *it;
        getAtp(Now), cnt++;
        if (!Now.atp) return false;
        if (cnt == Lis.size() || Now.atp == 1) {
            memset(vis, 0, sizeof(vis));
            sta[0] = 0, match(Now.att[0], Now.att[1], Now);
            embed(Now.atpPos), memset(vis, 0, sizeof(vis));
            for (int j = 2; j < sta[0]; j++) for (int l = Now.head[sta[j]]; l; l = Now.next[l]) if (Now.val[l]) {
                Graph temp;
                findGraph(Now, l, temp);
                if (!vis[sta[j]]) temp.att.push_back(sta[j]);
                for (int k = 0; k < temp.att.size(); k++) vis[temp.att[k]] = 0;
                Lis.push_back(temp);
            }
            list<Graph>::iterator temp = it++;
            Lis.erase(temp), cnt = 0, it--;
        }
        it++;
        if (it == Lis.end()) it = Lis.begin();
    }
    return true;
}

void Tarjan(int x, int fa, vector<Graph>& ret) {
    dfn[x] = low[x] = ++dfn[0];
    for (int l = Tot.head[x]; l; l = Tot.next[l]) {
        int y = Tot.last[l];
        if (y == fa) continue;
        if (!dfn[y]) Tarjan(y, x, ret), low[x] = min(low[x], low[y]);
        else low[x] = min(low[x], dfn[y]);
    }
    if (dfn[x] <= low[x]) {
        Graph temp;
        for (int l = Tot.head[x]; l; l = Tot.next[l]) if (Tot.val[l] && dfn[Tot.last[l]] > dfn[x])
            findGraph(Tot, l, temp);
        ret.push_back(temp);
    }
}

void findCircle(Graph& G) {
    int x = G.last[2];
    while (!vis[x]) {
        vis[x] = true;
        for (int l = G.head[x]; l; l = G.next[l]) if ((l ^ 1) != sta[sta[0]]) {
            x = G.last[l], sta[++sta[0]] = l;
            break;
        }
    }
    int l = 1, r = sta[0];
    while (G.last[sta[l] ^ 1] != x) l++;
    sta[0] = 0;
    for (int i = l; i <= r; i++)
        G.val[sta[i]] = G.val[sta[i] ^ 1] = 0, sta[++sta[0]] = G.last[sta[i] ^ 1];
}

bool canMap(vector<vector<int>> matrix,int n,int m) {
	vector<Graph> Div;
	Tot.clear();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j])
                Tot.add(i, j);
        }
    }

    if (m > 3 * n - 6 && m > 1)
        return false;

	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
	memset(isEmbed, 0, sizeof(isEmbed));
	memset(vis, 0, sizeof(vis));

	for (int i = 1; i <= n; i++) if (!dfn[i])
		Tarjan(i, -1, Div);

	bool flag = true;

	for (int i = 0; i < Div.size(); i++) {
		if (!Div[i].head.size())
            continue;

		sta[0] = 0, findCircle(Div[i]);

		Plane[1].push_back(sta[1]), Plane[1].push_back(sta[sta[0]]);
		
        embed(1);

		list<Graph> ret;
		memset(vis, 0, sizeof(vis));

		for (int j = 1; j <= sta[0]; j++) for (int l = Div[i].head[sta[j]]; l; l = Div[i].next[l]) if (Div[i].val[l]) {
			Graph temp;
			findGraph(Div[i], l, temp);
			if (!vis[sta[j]]) 
                temp.att.push_back(sta[j]);
			for (int k = 0; k < temp.att.size(); k++)
                vis[temp.att[k]] = 0;
			ret.push_back(temp);
		}

		flag &= Solve(ret);
		for (int j = 1; j <= PlaneNum; j++) 
            Plane[j].clear(), book[j].clear();
		PlaneNum = 1;

		if (!flag) break;
	}
    if (flag)
        return true;
    else
        return false;
}
```

#### matrix.h

```c++
/*Matrix.h*/
#include <vector>

using namespace std;

// 传入的参数依次为行列式的阶数和行列式数组,，返回值为行列式的值
int valueOfMatrix(int n, vector<vector<int> >a)
{
    // 使用递归算法计算行列式的值
    if (n == 1)
    {
        return a[0][0];
    }
    else if(n == 2)
        return a[0][0]*a[1][1]-a[0][1]*a[1][0];
    else
    {
        int sum = 0;
        for(int k=0; k<n; k++)
        {
            vector<vector<int> > b;
            for(int i=1; i<n; i++)
            {
                vector<int>c;
                for(int j=0; j<n; j++)
                {
                    if(j == k)
                        continue;
                    c.push_back(a[i][j]);
                }
                b.push_back(c);
            }
            sum = k%2==0 ? sum+a[0][k]*valueOfMatrix(n-1, b) : sum-a[0][k]*valueOfMatrix(n-1, b);
        }
        return sum;
    }
}

// 传入的参数为行列式的阶数和行列式数组，返回值为矩阵的秩
int rankOfDeterminant(int n, vector<vector<int> > matrix)
{
    // 求二进制矩阵的秩，即消元，最后看斜对角线上有几个1
    int row = 0;
    for(int col=0; col < n && row < n; col++, row++)  // 从每一列开始，将每一列消到只有 1 个 1 或者全 0
    {
        int i = 0;
        for(i = row; i < n; ++i)  // 寻找这一列第一个非 0 的行
        {
            if(matrix[i][col] != 0)
                break;
        }
        if(n == i)
            --row;
        else
        {
            swap(matrix[row], matrix[i]);
            for(int k = i+1; k < n; k++)
            {
                if(0 != matrix[k][col])
                {
                    for(int j = col; j < n; j++)
                    {
                        matrix[k][j] ^= matrix[row][j];// 用第 r 行的 1 消除这一列上其余全部行的 1
                    }
                }
            }
        }
    }
    return row;
}
```

#### main.cpp

```c++
/*main.cpp*/
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
```

### 测试数据

#### case 1

`input`

```
//n
4
//adjMatrix
0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0
//faceMatrix
1 0 1 1 0
0 1 1 0 1
1 1 0 1 1
```

`output`

```

#==========读入矩阵阶数n

4

#==========读入邻接矩阵

0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0

#==========输出邻接矩阵

index      v1   v2   v3   v4
   v1       0    1    1    1
   v2       1    0    0    1
   v3       1    0    0    1
   v4       1    1    1    0

#==========平面图判断

是平面图！

#==========读入面矩阵

1 0 1 1 0
0 1 1 0 1
1 1 0 1 1

#==========输出面矩阵

index      e1   e2   e3   e4   e5
   r1       1    0    1    1    0
   r2       0    1    1    0    1
   r3       1    1    0    1    1

#==========输出此平面图的顶点数n、边数m和面数r

顶点数：4 边数：5 面数：3

#==========输出此平面图的对偶图的顶点数n*、边数m*和面数r*

顶点数：3 边数：5 面数：4

#==========输出邻接矩阵

index      v1   v2   v3
   v1       0    1    1
   v2       1    0    1
   v3       1    1    0

#==========输出关联矩阵

index      e1   e2   e3   e4   e5
   v1       1    1    1    0    0
   v2       1    0    0    1    1
   v3       0    1    1    1    1
```

#### case 2

`input`

```
4
0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0
1 1 0 1 1
0 1 1 0 1
1 0 1 1 0
```

`output`

```

#==========读入矩阵阶数n

4

#==========读入邻接矩阵

0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0

#==========输出邻接矩阵

index      v1   v2   v3   v4
   v1       0    1    1    1
   v2       1    0    0    1
   v3       1    0    0    1
   v4       1    1    1    0

#==========平面图判断

是平面图！

#==========读入面矩阵

1 1 0 1 1
0 1 1 0 1
1 0 1 1 0

#==========输出面矩阵

index      e1   e2   e3   e4   e5
   r1       1    1    0    1    1
   r2       0    1    1    0    1
   r3       1    0    1    1    0

#==========输出此平面图的顶点数n、边数m和面数r

顶点数：4 边数：5 面数：3

#==========输出此平面图的对偶图的顶点数n*、边数m*和面数r*

顶点数：3 边数：5 面数：4

#==========输出邻接矩阵

index      v1   v2   v3
   v1       0    1    1
   v2       1    0    1
   v3       1    1    0

#==========输出关联矩阵

index      e1   e2   e3   e4   e5
   v1       1    1    1    1    0
   v2       1    1    0    0    1
   v3       0    0    1    1    1
```

## 实验总结

本次实验比起前两次实验难度降低了许多，不过附加项还是很难做。如果单单考虑控制台程序，输出图想不到有什么好的办法可以实现，判断是否是平面图用到的`kuratowski`定理也比较复杂，更难的是从邻接矩阵直接输出一个面矩阵，因此不得已我采用了输入两个矩阵来降低难度的做法，其他实验内容很基础，就是考察对平面图以及对偶图的概念的掌握情况，编程部分不复杂。

