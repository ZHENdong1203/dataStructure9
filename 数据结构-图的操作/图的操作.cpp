#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_VERTEX_NUM 20
#define ERROR 0
#define MAXQSIZE 100
#define OVERFLOW -2
#define OK 1
#define MAX_NAME 4
typedef char VertexType[MAX_NAME];
typedef int Status;
typedef int QElemType;
typedef struct ArcNode
{
	int adjvex;
	struct ArcNode *nextarc;
}ArcNode;
typedef struct VNode
{
	VertexType data;
	ArcNode *firstarc;
}VNode, AdjList[MAX_VERTEX_NUM];
typedef struct
{
	AdjList vertices;
	int vexnum, arcnum;
}ALGraph;
typedef struct {
	QElemType *base;
	int front;
	int rear;
}SqQueue;
int FirstAdjVex(ALGraph G, VertexType v);
int NextAdjVex(ALGraph G, VertexType v, VertexType w);
Status InitQueue(SqQueue &Q)
{
	Q.base = (QElemType*)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q.base)
	{
		exit(OVERFLOW);
	}
	Q.front = Q.rear = 0;
	return OK;
}//构造空队列
Status EnQueue(SqQueue &Q, QElemType e)
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front)
	{
		return ERROR;
	}
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MAXQSIZE;
	return OK;
}//入队
Status DeQueue(SqQueue &Q, QElemType &e)
{
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % MAXQSIZE;
	return e;
}//出队
Status QueueEmpty(SqQueue Q)
{ 
	if (Q.front == Q.rear)    
		return true;
	else
		return false;
}
int LocateVex(ALGraph G, VertexType v)
{
	int i;
	for (i = 0; i<G.vexnum; ++i)
		if (strcmp(G.vertices[i].data,v) == 0)
			return i;
	return -1;
}//定位函数
void CreateUDG(ALGraph &G) 
{
	ArcNode *p, *q;
	int i, j, k;
	VertexType v1, v2;
	printf("分别输入顶点个数和边的数目：\n");
	scanf("%d %d", &G.vexnum, &G.arcnum);
	printf("分别输入各个顶点值：\n");
	for (i = 0; i<G.vexnum; i++) 
	{
		scanf("%s", G.vertices[i].data);
		G.vertices[i].firstarc = NULL;  
	}
	printf("分别输入各条边的两个顶点：\n");
	for (k = 0; k<G.arcnum; k++) 
	{
		scanf("%s %s", v1, v2);
		i = LocateVex(G, v1); 
		j = LocateVex(G, v2); 
		p = (ArcNode*)malloc(sizeof(ArcNode));  
		p->adjvex = j; 
		p->nextarc = G.vertices[i].firstarc;
		G.vertices[i].firstarc = p; 
		q = (ArcNode*)malloc(sizeof(ArcNode));
		q->adjvex = i;
		q->nextarc = G.vertices[j].firstarc;
		G.vertices[j].firstarc = q;
	}
} //邻接表建立图
void PrintUDG(ALGraph G) 
{  
	int k;
		for (k = 0; k < G.vexnum; k++)
		{
			printf("%d:(%s)", k, G.vertices[k].data);
			ArcNode *p;
				p = G.vertices[k].firstarc;
				while (p != NULL)
				{
					printf("->%d", p->adjvex);
					p = p->nextarc;
				}
			printf("\n");
		}
}//邻接表的输出
bool visited[MAX_VERTEX_NUM]; 
void(*VisitFunc)(char* v);
void DFS(ALGraph G, int v)
{
	int w;
	visited[v] =true;
	VisitFunc(G.vertices[v].data);
	for (w = FirstAdjVex(G, G.vertices[v].data); w != -1; w = NextAdjVex(G, G.vertices[v].data, G.vertices[w].data))
		if (!visited[w])
			DFS(G, w);

}
void DFSTraverse(ALGraph G, void(*print)(char*))
{
	int i;
	VisitFunc = print;
	for (i = 0; i<G.vexnum; i++)
		visited[i] = 0;
	for (i = 0; i<G.vexnum; i++)
		if (!visited[i])
			DFS(G, i);
	printf("\n");
}

void print(char *i)
{
	printf("%s ", i);
}

int FirstAdjVex(ALGraph G, VertexType v)
{
	ArcNode *p;
	int v1;
	v1 = LocateVex(G, v);
	p = G.vertices[v1].firstarc;
	if (p)
		return p->adjvex;
	else
		return -1;
}
int NextAdjVex(ALGraph G, VertexType v, VertexType w)
{
	ArcNode *p;
	int v1, w1;
	v1 = LocateVex(G, v);
	w1 = LocateVex(G, w);
	p = G.vertices[v1].firstarc;
	while (p&&p->adjvex != w1)
		p = p->nextarc;
	if (!p || !p->nextarc)
		return -1;
	else
		return p->nextarc->adjvex;
}
void BFSTraverse(ALGraph G, void(*print)(char*))
{
	SqQueue Q;
	int v, u, w;
	for (v = 0; v < G.vexnum; ++v)
	{
		visited[v] = false;
	}
	InitQueue(Q);
	for (v = 0; v<G.vexnum; ++v)
	{
		if (!visited[v])
		{
			visited[v] = true;
			VisitFunc(G.vertices[v].data);
			EnQueue(Q, v);
			while (!QueueEmpty(Q))
			{
				DeQueue(Q, u);
				for (w = FirstAdjVex(G, G.vertices[v].data); w != -1; w = NextAdjVex(G, G.vertices[v].data, G.vertices[w].data))
				{
					if (!visited[w])
					{
						visited[w] = true;
						VisitFunc(G.vertices[w].data);
						EnQueue(Q, w);
					}
				}
			}
		}
	}
}
int main()
{
	ALGraph G;
	CreateUDG(G);
	printf("无向图G打印如下:\n");
	PrintUDG(G);
	printf("深度优先遍历:\n");
	DFSTraverse(G, print);
	printf("广度优先遍历:\n");
	BFSTraverse(G, print);
	printf("\n");
	system("pause");
	return 0;
}