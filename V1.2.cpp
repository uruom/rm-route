#include<iostream>
#include<cstdio>
#include<cmath>
#include<Windows.h>
#include<ctime>
using namespace std;
int map[100][100];//��ͼ 0�����ϰ� 1���ƶ��ϰ� 2���̶��ϰ� 
int map_step[100][100];//����ʼ�㵽��������貽�� 
int map_forecast[100][100];//Ԥ��ֵ
int map_value[100][100];//��ֵ 
int n,m;//�����С 

int start_x,start_y;//�������

int line_now,line_count=1;//�����жϵ��ĵ� �ܹ����ٸ��� 

struct coordinate
{
	int x,y;
}line[100000];//���¶��� 

bool flag[100][100]={false};//�õ��Ƿ��Ѿ����޸Ķ��� 

 void insert(int update_x,int update_y,int value,int next);//�ѵĲ��� 
void pop(int update_x,int update_y,int value,int next);//�ѵĵ��� 
void add(int x1,int y1,int x2,int y2);//�ж���������Ƿ���ͨ���� 
void add_line(int x,int y);//�Ըõ�ǰ�������ж� 
void read(void);//���� 
void A_(void);//A* add add_line 
void write(void);//��� 
int main()
{
	freopen("test.txt","r",stdin);
	freopen("ans.txt","w",stdout);
	read();
	A_();
	write();
	return 0;	
 } 
 
 void insert(int update_x,int update_y,int value,int next)//�ѵĲ��� 
{
	line[next].x=update_x;//�õ���� 
	line[next].y=update_y;
	if(value<map_value[line[next>>1].x][line[next>>1].y])//�ж���ѵ���һλ��� 
	{
		line[next].x=line[next>>1].x;
		line[next].y=line[next>>1].y;
		insert(update_x,update_y,value,next>>1);
	}
}

void pop(int update_x,int update_y,int value,int next)//�ѵĵ��� 
{
	line[next].x=update_x;//�õ���� 
	line[next].y=update_y;
	if(((map_value[line[next<<1].x][line[next<<1].y]<value)&&(next<<1)<line_count)||((map_value[line[next<<1|1].x][line[next<<1|1].y]<value)&&(((next<<1)|1)<line_count)))//�Ƿ���� 
	{
		if((map_value[line[next<<1].x][line[next<<1].y]<map_value[line[next<<1|1].x][line[next<<1|1].y])||(line_count==(next<<1)|1))//���µ������ 
		{
			line[next].x=line[next<<1].x;
			line[next].y=line[next<<1].y;
			pop(update_x,update_y,value,next<<1);
		}
		else//���µ��Ҷ��� 
		{
			line[next].x=line[next<<1|1].x;
			line[next].y=line[next<<1|1].y;
			pop(update_x,update_y,value,(next<<1)|1);
		}
	}
}

void add(int x1,int y1,int x2,int y2)
{
	if((map_step[x1][y1]+1<map_step[x2][y2])&&(map[x2][y2]==0))//�õ�����ҿɱ����� 
	{
		map_step[x2][y2]=map_step[x1][y1]+1;
		flag[x2][y2]=true;
		map_value[x2][y2]=map_step[x2][y2]+map_forecast[x2][y2]; 
		line_count++;
		insert(x2,y2,map_value[x2][y2],line_count); 
	}
}

void add_line(int x,int y)
{
	if(x<n) add(x,y,x+1,y);
	if(x>1) add(x,y,x-1,y);
	if(y<m) add(x,y,x,y+1);
	if(y>1) add(x,y,x,y-1);
}

void read(void)
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)//����ԭ���1.1��ʼ 
	for(int j=1;j<=m;j++)
	{
		scanf("%d",&map[i][j]);
		map_step[i][j]=1<<29;//��ʼ��·������ֵ 
	}
	scanf("%d%d%d%d",&start_x,&start_y,&line[1].x,&line[1].y);//������� �յ� 
	map_step[line[1].x][line[1].y]=0; //����ʼ�� 
	for(int i=1;i<=n;i++)
	for(int j=1;j<=m;j++)
	map_forecast[i][j]=abs(start_x-i)+abs(start_y-j);
}

void A_(void)
{
	while(flag[start_x][start_y]==false)
	{
		//�ƽ�����һ���� 
		add_line(line[1].x,line[1].y);//�ӵ�
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//���� 
		line_count--;
	}
}

void write(void)
{
	for(int i=1;i<=n;i++)//�������ͼ 
	{
		for(int j=1;j<=m;j++)
		{
			if(map_step[i][j]<=9) 
			printf("%d    ||",map_step[i][j]);
			else
			{
				if(map_step[i][j]<=999)
				printf("%d   ||",map_step[i][j]);
				else
				printf("#    ||");
			}
		} 
		
		printf("\n");
	}
	printf("%d",clock());
}


