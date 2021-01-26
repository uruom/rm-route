#include<iostream>
#include<cstdio>
#include<cmath>
#include<Windows.h>
#include<ctime>
using namespace std;
int map[100][100];//地图 0：无障碍 1：移动障碍 2：固定障碍 8：已走路径 

int map_step[100][100];//从起始点到坐标点所需步数，即G 

int map_forecast[100][100];//预估值，即H 

int map_value[100][100];//A*及D*的算术值，即F，F=G+H 

bool map_flag[100][100]={false};//标记盖地那是否是路径上的点 

int n,m;//矩阵大小 

int t=3;

int D_flag;//在D*中的终点值，搜索中不确定，是在障碍物后已知路径中任意一点 

int end_x,end_y,/*终点值，不改变*/start_x,start_y,/*起点值，随着推进直接改变*/move_x,move_y/*可改变的终点值，在搜索中的终点值*/;

int line_now,line_count=1;//现在判断到的点 总共多少个点 

struct coordinate
{
	int x,y;
}line[100000];//更新队列 

struct link//链表，表明该坐标的上位坐标与下位坐标 
{
	int next,root;//下一位和上一位 
}map_link[100][100];

bool flag[100][100]={false};//该点是否已经在修改队列 



void insert(int update_x,int update_y,int value,int next);//堆的插入 

void pop(int update_x,int update_y,int value,int next);//堆的弹出 

void add(int x1,int y1,int x2,int y2);//判断这两点间是否连通可走 

void add_line(int x,int y);//对该点前后左右边界判断，如未越界，进入add  

void read(void);//读入 + 赋初值 

void A_(void);//A* add add_line 

void write_step(int x,int y);//按步输出 

void map_line_flag(void);//查找路径标记点

void step_update(int x,int y,int x1,int y1);//遇到路障更新 

void write(void);//输出 

void file(void);//文件 

void writetest(void);//输出实验 

void writetest2(void);//输出实验2 

int main()
{
	file();//文件输入输出 
	srand(time(NULL));//随机数初始化 
	read();//读入 
	A_();//跑第一遍的A*，算出静态路径 
	map_line_flag();//连线，将静态路径上的点连接且标记 
	writetest();
	write();//输出 
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
		if((map_value[line[next<<1].x][line[next<<1].y]<map_value[line[next<<1|1].x][line[next<<1|1].y]))
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

void add(int x1,int y1,int x2,int y2)//当前点为x1,y1,下一个点为x2,y2,判断是否可以更新到这个点 
{
	if((map_step[x1][y1]+1<map_step[x2][y2])&&(map[x2][y2]!=1&&map[x2][y2]!=2))//该点可走且可被更新 
	{
		map_step[x2][y2]=map_step[x1][y1]+1;//从起点开始走到x2,y2所需路程为x1，y1+1 
		flag[x2][y2]=true;//标记点x2，y2已经走过 
		map_value[x2][y2]=map_step[x2][y2]+map_forecast[x2][y2];//算出点x2，y2的权值，方便排序 
		line_count++;//栈中数量+1 
		if(map_flag[x2][y2]==true)//如果该点在之前已经走过，且路径没有封锁 
		D_flag=x2*m+y2;//将该点作为终止点标记下来 
		insert(x2,y2,map_value[x2][y2],line_count);//在栈中插入 
	}
}

void add_line(int x,int y)//边界判断，是否可以进行add操作 
{
	if(x<n) add(x,y,x+1,y);//下边界 
	if(x>1) add(x,y,x-1,y);//上边界 
	if(y<m) add(x,y,x,y+1);//右边界 
	if(y>1) add(x,y,x,y-1);//左边界 
}

void read(void)//读入以及初始化 
{
	scanf("%d%d",&n,&m);//读入方阵大小 
	for(int i=1;i<=n;i++)//坐标原点从1.1开始 
	for(int j=1;j<=m;j++)
	{
		scanf("%d",&map[i][j]);//读入障碍 
		map_step[i][j]=1<<29;//初始化路径极大值 
	}
	scanf("%d%d%d%d",&line[1].x,&line[1].y,&end_x,&end_y);//读入起点 终点 
	start_x=line[1].x; 
	start_y=line[1].y;//起点赋值 
	move_x=end_x;
	move_y=end_y;//移动终点变量赋值 
	map_step[line[1].x][line[1].y]=0; //起点初始化 
	for(int i=1;i<=n;i++)
	for(int j=1;j<=m;j++)
	map_forecast[i][j]=abs(end_x-i)+abs(end_y-j);//预估值即H初始化 
}

void A_(void)
{
	int lx,ly;
	while(flag[end_x][end_y]==false)//直到终点被搜索到之前，一直循环 
	{
		lx=line[1].x;
		ly=line[1].y;
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//弹出栈首元素，重新排序 
		line_count--;//栈数量-1 
		add_line(lx,ly);//对现在放在栈首的坐标进行扩张 
		
	}
}

void D_(void)
{
	D_flag=0;//重置D*终点值，初始化 
	int lx,ly,p=0;
	while(D_flag==0&&p<=1000)//当没有找到已走路径上任一点时，继续搜索 
	{
		p++;
		lx=line[1].x;
		ly=line[1].y;
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);///弹出栈首元素，重新排序 
		line_count--;//栈数量-1
		add_line(lx,ly);//对现在放在栈首的坐标进行扩张 
		 
	}
	if(D_flag==0)
	{
		writetest();
		printf("\n\n\n");
		writetest2();
		printf("无解\n");
		exit(0);
	}
} 

 
void step_update(int x,int y,int x1,int y1)//当出现有障碍物出现在路径上时，开始更新 
{
	t++;
	line_count=1;//初始化栈中元素数量 
	
	line[1].x=x;
	line[1].y=y;
	//赋值坐标,首元素即起始点为当前点 
	
	for(int i=0;i<=n;i++)
	for(int j=0;j<=m;j++)
	map_step[i][j]=1<<29;
	map_step[x][y]=0;
	//初始化G，即实际值 
	 
	D_();//开始D* 
	
	move_x=(D_flag-1)/m; 
	move_y=(D_flag-1)%m+1;
	//转化终点坐标
	
	
	int px=move_x,py=move_y;
	int temp_x=px,temp_y=py;
	while(map_link[px][py].root!=x*m+y)
	{
		map_flag[px][py]=false;
		px=(map_link[temp_x][temp_y].root-1)/m;
		py=(map_link[temp_x][temp_y].root-1)%m+1;
		temp_x=px;
		temp_y=py;
//		cout<<"kk"<<endl;
	}
	//将终点坐标到障碍物出现之间的路径标记删除 
	
	map_line_flag();//将发现障碍物点（现在起始点）与现在的终点连接起来
	 
	writetest();//输出 
	
//	exit(0);
	D_flag=0;
}

void write_step(int x,int y)
{
	map[x][y]=t;//经过当前点，标记为8 
	
	map_flag[x][y]=false;//当前点标记清空，不在未走的路径中 
	
	if((rand()%1==0))//随机数判断是否出现障碍 
	{
		int lx=x+rand()%6-3,ly=y+rand()%6-3;//随机数障碍出现坐标，标定为上下左右3格中，lx是横坐标，ly是纵坐标 
		 
		if((lx<=n)&&(lx>=1)&&(ly<=m)&&(ly>=1)&&(map[lx][ly]==0))//如果该障碍没有越界 
		{
			map[lx][ly]=1;//障碍标记
			
			 
			if(map_flag[lx][ly])//判断障碍是否出现在路径上 
			{
				int px=lx,py=ly; 
				int temp_x=px,temp_y=py;
				while(map_link[px][py].root!=x*m+y)
				{
					map_flag[px][py]=false;
					px=(map_link[temp_x][temp_y].root-1)/m;
					py=(map_link[temp_x][temp_y].root-1)%m+1;
					temp_x=px;
					temp_y=py;
				//	cout<<"pp"<<endl;
				}
				map_flag[px][py]=false;
				//消除所在点到障碍点路径上的所有标注点
				
				map_flag[x][y]=false;
				step_update(x,y,lx,ly);//更新，其中起点坐标为当前点，
				map_flag[x][y]=false; 
			}
			
			
		}
	}
 }
 
 
 
void map_line_flag(void)//将标记点连线 
{
 	map_flag[move_x][move_y]=true;//该点会被经过 
	
	while(map_step[move_x][move_y]!=0)//直至将终点走到起点时，终点倒着走，数字小的一点必然可以到达起点，反之则不然 
	{
		if(move_x>1)
		if(map_step[move_x][move_y]==map_step[move_x-1][move_y]+1)
		{
			map_link[move_x][move_y].root=(move_x-1)*m+move_y;
			move_x--;
			map_flag[move_x][move_y]=true;
			map_link[move_x][move_y].next=(move_x+1)*m+move_y;
		}
		if(move_x<n)
		if(map_step[move_x][move_y]==map_step[move_x+1][move_y]+1)
		{
			map_link[move_x][move_y].root=(move_x+1)*m+move_y;
			move_x++;
			map_flag[move_x][move_y]=true;
			map_link[move_x][move_y].next=(move_x-1)*m+move_y;
		}
		if(move_y<m)
		if(map_step[move_x][move_y]==map_step[move_x][move_y+1]+1)
		{
			map_link[move_x][move_y].root=(move_x)*m+move_y+1;
			move_y++;
			map_flag[move_x][move_y]=true;
			map_link[move_x][move_y].next=(move_x)*m+move_y-1;
		}
		if(move_y>1)
		if(map_step[move_x][move_y]==map_step[move_x][move_y-1]+1)
		{
			map_link[move_x][move_y].root=(move_x)*m+move_y-1;
			move_y--;
			map_flag[move_x][move_y]=true;
			map_link[move_x][move_y].next=(move_x)*m+move_y+1;
		}
//		cout<<"cc"<<endl;
	}	
	//连接 root为该点的上一点，next为该点的下一点 起点没有root，终点没有next 
}



void write(void)
{	
	int temp_x=start_x,temp_y=start_y;//暂时储存 
	while((start_x!=end_x)||(start_y!=end_y))
	{
		write_step(start_x,start_y);
		start_x=(map_link[temp_x][temp_y].next-1)/m;
		start_y=(map_link[temp_x][temp_y].next-1)%m+1;
		temp_x=start_x;
		temp_y=start_y;
//		cout<<"ww"<<endl;
	}
		
}
  

void file(void)
{
	freopen("test.txt","r",stdin);
	freopen("ans2.txt","w",stdout);
}

 void writetest2(void)//输出各点的实际步数 
 {
 	for(int i=1;i<=n;i++)//输出
	{
		for(int j=1;j<=m;j++)
		{
			if(map_step[i][j]<=9) 
			printf("%d |",map_step[i][j]);
			else
			{
				if(map_step[i][j]<=999)
				printf("%d|",map_step[i][j]);
				else
				printf("# |");
			}
		} 
		
		printf("\n");
	}
} 

void writetest(void)//输出标记步数 
{
 	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(map_flag[i][j])
			printf("# ");
			else printf("%d ",map[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
} 


