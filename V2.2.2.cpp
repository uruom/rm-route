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
bool map_flag[100][100]={false};//��Ǹǵ����Ƿ���·���ϵĵ� 
int n,m;//�����С 
int D_flag;
int end_x,end_y,start_x,start_y,move_x,move_y;//�յ㡢�������

int line_now,line_count=1;//�����жϵ��ĵ� �ܹ����ٸ��� 

struct coordinate
{
	int x,y;
}line[100000];//���¶��� 

struct link//���� 
{
	int next,root;//��һλ����һλ 
}map_link[100][100];

bool flag[100][100]={false};//�õ��Ƿ��Ѿ����޸Ķ��� 

void insert(int update_x,int update_y,int value,int next);//�ѵĲ��� 
void pop(int update_x,int update_y,int value,int next);//�ѵĵ��� 
void add(int x1,int y1,int x2,int y2);//�ж���������Ƿ���ͨ���� 
void add_line(int x,int y);//�Ըõ�ǰ�������ж� 
void read(void);//���� 
void A_(void);//A* add add_line 
void write_step(int x,int y);//������� 
void map_line_flag(void);//����·����ǵ�
void step_update(int x,int y,int x1,int y1);//����·�ϸ��� 
void write(void);//��� 
void file(void);//�ļ� 
void writetest(void);//���ʵ�� 
void clean(void);
void map_link_delete(int sta_x,int sta_y,int ed_x,int ed_y);
int main()
{
	file();
	srand(time(NULL));
	read();
	A_();
	map_line_flag();
	write();
//	writetest();
	printf("%d",clock());
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
	if(((map_value[line[next<<1].x][line[next<<1].y]<=value)&&(next<<1)<line_count)||((map_value[line[next<<1|1].x][line[next<<1|1].y]<=value)&&(((next<<1)|1)<line_count)))//�Ƿ���� 
	{
		if((map_value[line[next<<1].x][line[next<<1].y]<map_value[line[next<<1|1].x][line[next<<1|1].y]))//||(line_count==(next<<1)|1))//���µ������ 
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
	if((map_step[x1][y1]+1<map_step[x2][y2])&&(map[x2][y2]==0||map[x2][y2]==3))//�õ�����ҿɱ����� 
	{
		map_step[x2][y2]=map_step[x1][y1]+1;
		flag[x2][y2]=true;
		map_value[x2][y2]=map_step[x2][y2]+map_forecast[x2][y2]; 
		line_count++;
		if(map_flag[x2][y2]==true)
		D_flag=x2*m+y2;
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
	for(int i=1;i<=n;i++)//����ԭ���1.1��ʼ 
	for(int j=1;j<=m;j++)
	{
		scanf("%d",&map[i][j]);
		map_step[i][j]=1<<29;//��ʼ��·������ֵ 
	}
	scanf("%d%d%d%d",&line[1].x,&line[1].y,&end_x,&end_y);//������� �յ� 
	start_x=line[1].x;
	start_y=line[1].y;
	move_x=end_x;
	move_y=end_y;
	map_step[line[1].x][line[1].y]=0; //����ʼ�� 
	for(int i=1;i<=n;i++)
	for(int j=1;j<=m;j++)
	map_forecast[i][j]=abs(end_x-i)+abs(end_y-j);
}

void A_(void)
{
	while(flag[end_x][end_y]==false)
	{
		//�ƽ�����һ���� 
		add_line(line[1].x,line[1].y);//�ӵ�
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//���� 
	//	printf("%d\n",line_count);
		line_count--;
	}
}

void D_(void)
{
	D_flag=0;
	while(D_flag==0)
	{
		//�ƽ�����һ���� 
		add_line(line[1].x,line[1].y);//�ӵ�
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//���� 
	//	writetest();
	//	printf("%d\n",line_count);
		line_count--;
	}
	
} 
void clean(void)
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
		map_step[i][j]=1<<29;
	
}
 
void map_link_delete(int sta_x,int sta_y,int ed_x,int ed_y)
{
	//�������ڵ㵽�ϰ���·���ϵ����б�ע�� 
		int temp_x=ed_x,temp_y=ed_y;
		while(map_link[ed_x][ed_y].root!=sta_x*m+sta_y)
		{
			map_flag[ed_x][ed_y]=false;
			map[ed_x][ed_y]=3;
			ed_x=(map_link[temp_x][temp_y].root-1)/m;
			ed_y=(map_link[temp_x][temp_y].root-1)%m+1;
			map_link[temp_x][temp_y].root=0;
			map_link[temp_x][temp_y].next=0;
			temp_x=ed_x;
			temp_y=ed_y;
		}
		map_flag[ed_x][ed_y]=false;
		map_link[temp_x][temp_y].root=0;
		map_link[temp_x][temp_y].next=0;
}
void step_update(int x,int y,int x1,int y1)
{
	cout<<endl<<"ssss"<<endl;
	line_count=1;
	line[1].x=x;
	line[1].y=y;
	clean();
	map_step[x][y]=0;
	D_();
	move_x=(D_flag-1)/m;
	move_y=(D_flag-1)%m+1;
	D_flag=0;
	cout<<"llll"<<endl;
	writetest();
	printf("x=%d y=%d move_x=%d,move_y=%d",x,y,move_x,move_y);
	map_link_delete((map_link[x1][y1].root-1)/m,(map_link[x1][y1].root-1)%m+1,move_x,move_y);
	cout<<endl<<"oooo"<<endl;
	map_line_flag();
	cout<<endl<<"pppp"<<endl;
	
}

void write_step(int x,int y)
{
/*	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if((i!=x)||(j!=y))
			printf("%d ",map[i][j]);
			else printf("# ");
		}
		printf("\n");
	}*/
	map[x][y]=8;
	map_flag[x][y]=false;
	printf("\n====================================================================\n");
	if((rand()%1==0))
	{
		int lx=x+rand()%6-3,ly=y+rand()%6-3;
		if((lx<=n)&&(lx>=1)&&(ly<=m)&&(ly>=1)&&(map[lx][ly]==0))
		{
			map[lx][ly]=1;
			if(map_flag[lx][ly])
			{
			/*	int px=lx,py=ly;//�������ڵ㵽�ϰ���·���ϵ����б�ע�� 
				int temp_x=px,temp_y=py;
				while(map_link[px][py].root!=x*m+y)
				{
					map_flag[px][py]=false;
					map[px][py]=3;
					px=(map_link[temp_x][temp_y].root-1)/m;
					py=(map_link[temp_x][temp_y].root-1)%m+1;
					temp_x=px;
					temp_y=py;
				}
				map_flag[px][py]=false;*/
				
				writetest();
				printf("x=%d y=%d move_x=%d,move_y=%d",x,y,move_x,move_y);
				map_link_delete(x,y,lx,ly);
				map_link[lx][ly].root=lx*m+ly;
				//map[px][py]=3;
				map[lx][ly]=1;
//				writetest();
//				exit(0);
				step_update(x,y,lx,ly);
			}
			
		}
	}
	
 }
 
void map_line_flag(void)
{
 	map_flag[move_x][move_y]=true;
 	printf("%d %d ccc\n",move_x,move_y);
	while(map_step[move_x][move_y]!=0)
	{
		printf("move_x=%d move_y=%d %d iii\n",move_x,move_y,map_step[move_x][move_y]);
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
		if(move_x==20&&move_y==1)
		{
			writetest();
			exit(0);
		}
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
	
	int temp_x=start_x,temp_y=start_y;//��ʱ���� 
	while((start_x!=end_x)||(start_y!=end_y))
	{
		write_step(start_x,start_y);
		start_x=(map_link[temp_x][temp_y].next-1)/m;
		start_y=(map_link[temp_x][temp_y].next-1)%m+1;
		temp_x=start_x;
		temp_y=start_y;
	}
		
}
  

void file(void)
{
// 	freopen("test.txt","r",stdin);
//	freopen("ans1.txt","w",stdout);
}

 
 void writetest(void)
 {
 /*	for(int i=1;i<=n;i++)//�������ͼ 
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
	}*/
 /*	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(map_flag[i][j])
			printf("# ");
			else printf("%d ",map[i][j]);
		}
		printf("\n");
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			printf("%d %d  ||",(map_link[i][j].next-1)/m,(map_link[i][j].next-1)%m+1);
		}
		printf("\n");
	}
	printf("\n==========================\n");
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			printf("%d %d  ||",(map_link[i][j].root-1)/m,(map_link[i][j].root-1)%m+1);
		}
		printf("\n");
	}*/
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
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(map_link[i][j].root)
			{
				printf("i=%d j=%d x=%d y=%d\n",i,j,(map_link[i][j].root-1)/m,(map_link[i][j].root-1)%m+1);
			}
		//	printf("%d ",map_link[i][j].root);
		}
		//printf("\n");
	}
	
  } 


/*void write(void)
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
	write_step(end_x,end_y);
	while(map_step[end_x][end_y]!=0)
	{
		if(end_x>1)
		if(map_step[end_x][end_y]==map_step[end_x-1][end_y]+1)
		{
			end_x--;
			write_step(end_x,end_y);
		}
		if(end_x<n)
		if(map_step[end_x][end_y]==map_step[end_x+1][end_y]+1)
		{
			end_x++;
			write_step(end_x,end_y);
		}
		if(end_y<m)
		if(map_step[end_x][end_y]==map_step[end_x][end_y+1]+1)
		{
			end_y++;
			write_step(end_x,end_y);
		}
		if(end_y>1)
		if(map_step[end_x][end_y]==map_step[end_x][end_y-1]+1)
		{
			end_y--;
			write_step(end_x,end_y);
		}
	} 
	
}*/

