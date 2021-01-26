#include<iostream>
#include<cstdio>
#include<cmath>
#include<Windows.h>
#include<ctime>
using namespace std;
int map[100][100];//地图 0：无障碍 1：移动障碍 2：固定障碍 
int map_step[100][100];//从起始点到坐标点所需步数 
int map_forecast[100][100];//预估值
int map_value[100][100];//总值 
bool map_flag[100][100]={false};//标记盖地那是否是路径上的点
int n,m;//矩阵大小 
 
int start_x,start_y;//起点坐标

int line_now,line_count=1;//现在判断到的点 总共多少个点 

struct coordinate
{
	int x,y;
}line[100000];//更新队列 

bool flag[100][100]={false};//该点是否已经在修改队列 

void insert(int update_x,int update_y,int value,int next);//堆的插入 
void pop(int update_x,int update_y,int value,int next);//堆的弹出 
void add(int x1,int y1,int x2,int y2);//判断这两点间是否连通可走 
void add_line(int x,int y);//对该点前后左右判断 
void read(void);//读入 
void A_(void);//A* add add_line 
void write_step(int x,int y);//按步输出 
void map_line_flag(void);//查找路径标记点
void step_update(int x,int y,int x1,int y1);//遇到路障更新 
void write(void);//输出 
void file(void);//文件 
int main()
{
	file();
	read();
	A_();
	write();
	map_line_flag();
/*	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(map_flag[i][j])
			printf("# ");
			else printf("%d ",map[i][j]);
		}
		printf("\n");
	}*/
	printf("%d",clock());
	return 0;	
 } 
 
 void insert(int update_x,int update_y,int value,int next)//堆的插入 
{
	line[next].x=update_x;//该点更新 
	line[next].y=update_y;
	if(value<map_value[line[next>>1].x][line[next>>1].y])//判断与堆的下一位相比 
	{
		line[next].x=line[next>>1].x;
		line[next].y=line[next>>1].y;
		insert(update_x,update_y,value,next>>1);
	}
}

void pop(int update_x,int update_y,int value,int next)//堆的弹出 
{
	line[next].x=update_x;//该点更新 
	line[next].y=update_y;
	if(((map_value[line[next<<1].x][line[next<<1].y]<=value)&&(next<<1)<line_count)||((map_value[line[next<<1|1].x][line[next<<1|1].y]<=value)&&(((next<<1)|1)<line_count)))//是否更新 
	{
		if((map_value[line[next<<1].x][line[next<<1].y]<map_value[line[next<<1|1].x][line[next<<1|1].y]))//||(line_count==(next<<1)|1))//更新到左儿子 
		{
			line[next].x=line[next<<1].x;
			line[next].y=line[next<<1].y;
			pop(update_x,update_y,value,next<<1);
		}
		else//更新到右儿子 
		{
			line[next].x=line[next<<1|1].x;
			line[next].y=line[next<<1|1].y;
			pop(update_x,update_y,value,(next<<1)|1);
		}
	}
}

void add(int x1,int y1,int x2,int y2)
{
	if((map_step[x1][y1]+1<map_step[x2][y2])&&(map[x2][y2]==0))//该点可走且可被更新 
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
//	printf("%d %d\n",x,y);
//	write();
}

void read(void)
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)//坐标原点从1.1开始 
	for(int j=1;j<=m;j++)
	{
		scanf("%d",&map[i][j]);
		map_step[i][j]=1<<29;//初始化路径极大值 
	}
	scanf("%d%d%d%d",&start_x,&start_y,&line[1].x,&line[1].y);//读入起点 终点 
	map_step[line[1].x][line[1].y]=0; //起点初始化 
	for(int i=1;i<=n;i++)
	for(int j=1;j<=m;j++)
	map_forecast[i][j]=abs(start_x-i)+abs(start_y-j);
}

void A_(void)
{
	while(flag[start_x][start_y]==false)
	{
		//推进到下一个点 
		add_line(line[1].x,line[1].y);//加点
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//弹出 
	//	printf("%d\n",line_count);
		line_count--;
	}
}

void step_update(int x,int y,int x1,int y1)
{
	
}

void write_step(int x,int y)
{
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if((i!=x)||(j!=y))
			printf("%d ",map[i][j]);
			else printf("# ");
		}
		printf("\n");
	}
	map[x][y]='.';
	printf("\n====================================================================\n");
	if((rand()%2==0)&&(rand()%2==0)&&(rand()%2==0))
	{
		int lx=rand()%6-3,ly=rand()%6-3;
		if((lx<=n)&&(lx>=1)&&(ly<=m)&&(ly>=1))
		{
			map[lx][ly]=1;
			if(map_flag[lx][ly])
			step_update(x,y,lx,ly);
		}
	}
	
 }
 
void map_line_flag(void)
{
 	map_flag[start_x][start_y]=true;
	while(map_step[start_x][start_y]!=0)
	{
		if(start_x>1)
		if(map_step[start_x][start_y]==map_step[start_x-1][start_y]+1)
		{
			start_x--;
			map_flag[start_x][start_y]=true;
		}
		if(start_x<n)
		if(map_step[start_x][start_y]==map_step[start_x+1][start_y]+1)
		{
			start_x++;
			map_flag[start_x][start_y]=true;
		}
		if(start_y<m)
		if(map_step[start_x][start_y]==map_step[start_x][start_y+1]+1)
		{
			start_y++;
			map_flag[start_x][start_y]=true;
		}
		if(start_y>1)
		if(map_step[start_x][start_y]==map_step[start_x][start_y-1]+1)
		{
			start_y--;
			map_flag[start_x][start_y]=true;
		}
	}
 	
}
void write(void)
{
	for(int i=1;i<=n;i++)//输出主地图 
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
	write_step(start_x,start_y);
	while(map_step[start_x][start_y]!=0)
	{
		if(start_x>1)
		if(map_step[start_x][start_y]==map_step[start_x-1][start_y]+1)
		{
			start_x--;
			write_step(start_x,start_y);
		}
		if(start_x<n)
		if(map_step[start_x][start_y]==map_step[start_x+1][start_y]+1)
		{
			start_x++;
			write_step(start_x,start_y);
		}
		if(start_y<m)
		if(map_step[start_x][start_y]==map_step[start_x][start_y+1]+1)
		{
			start_y++;
			write_step(start_x,start_y);
		}
		if(start_y>1)
		if(map_step[start_x][start_y]==map_step[start_x][start_y-1]+1)
		{
			start_y--;
			write_step(start_x,start_y);
		}
	}
	
}
  
/*void write(void)
{
	for(int i=1;i<=n;i++)//输出主地图 
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
	write_step(start_x,start_y);
	while(map_step[start_x][start_y]!=0)
	{
		if(start_x>1)
		if(map_step[start_x][start_y]==map_step[start_x-1][start_y]+1)
		{
			start_x--;
			write_step(start_x,start_y);
		}
		if(start_x<n)
		if(map_step[start_x][start_y]==map_step[start_x+1][start_y]+1)
		{
			start_x++;
			write_step(start_x,start_y);
		}
		if(start_y<m)
		if(map_step[start_x][start_y]==map_step[start_x][start_y+1]+1)
		{
			start_y++;
			write_step(start_x,start_y);
		}
		if(start_y>1)
		if(map_step[start_x][start_y]==map_step[start_x][start_y-1]+1)
		{
			start_y--;
			write_step(start_x,start_y);
		}
	}
	
}*/

void file(void)
{
	freopen("test.txt","r",stdin);
	freopen("ans.txt","w",stdout);
 }

