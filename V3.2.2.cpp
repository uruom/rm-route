#include<iostream>
#include<cstdio>
#include<cmath>
#include<Windows.h>
#include<ctime>
using namespace std;
int map[100][100];//��ͼ 0�����ϰ� 1���ƶ��ϰ� 2���̶��ϰ� 8������·�� 

int map_step[100][100];//����ʼ�㵽��������貽������G 

int map_forecast[100][100];//Ԥ��ֵ����H 

int map_value[100][100];//A*��D*������ֵ����F��F=G+H 

bool map_flag[100][100]={false};//��Ǹǵ����Ƿ���·���ϵĵ� 

int n,m;//�����С 

int t=3;

int D_flag;//��D*�е��յ�ֵ�������в�ȷ���������ϰ������֪·��������һ�� 

int end_x,end_y,/*�յ�ֵ�����ı�*/start_x,start_y,/*���ֵ�������ƽ�ֱ�Ӹı�*/move_x,move_y/*�ɸı���յ�ֵ���������е��յ�ֵ*/;

int line_now,line_count=1;//�����жϵ��ĵ� �ܹ����ٸ��� 

struct coordinate
{
	int x,y;
}line[100000];//���¶��� 

struct link//�����������������λ��������λ���� 
{
	int next,root;//��һλ����һλ 
}map_link[100][100];

bool flag[100][100]={false};//�õ��Ƿ��Ѿ����޸Ķ��� 



void insert(int update_x,int update_y,int value,int next);//�ѵĲ��� 

void pop(int update_x,int update_y,int value,int next);//�ѵĵ��� 

void add(int x1,int y1,int x2,int y2);//�ж���������Ƿ���ͨ���� 

void add_line(int x,int y);//�Ըõ�ǰ�����ұ߽��жϣ���δԽ�磬����add  

void read(void);//���� + ����ֵ 

void A_(void);//A* add add_line 

void write_step(int x,int y);//������� 

void map_line_flag(void);//����·����ǵ�

void step_update(int x,int y,int x1,int y1);//����·�ϸ��� 

void write(void);//��� 

void file(void);//�ļ� 

void writetest(void);//���ʵ�� 

void writetest2(void);//���ʵ��2 

int main()
{
	file();//�ļ�������� 
	srand(time(NULL));//�������ʼ�� 
	read();//���� 
	A_();//�ܵ�һ���A*�������̬·�� 
	map_line_flag();//���ߣ�����̬·���ϵĵ������ұ�� 
	writetest();
	write();//��� 
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
		if((map_value[line[next<<1].x][line[next<<1].y]<map_value[line[next<<1|1].x][line[next<<1|1].y]))
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

void add(int x1,int y1,int x2,int y2)//��ǰ��Ϊx1,y1,��һ����Ϊx2,y2,�ж��Ƿ���Ը��µ������ 
{
	if((map_step[x1][y1]+1<map_step[x2][y2])&&(map[x2][y2]!=1&&map[x2][y2]!=2))//�õ�����ҿɱ����� 
	{
		map_step[x2][y2]=map_step[x1][y1]+1;//����㿪ʼ�ߵ�x2,y2����·��Ϊx1��y1+1 
		flag[x2][y2]=true;//��ǵ�x2��y2�Ѿ��߹� 
		map_value[x2][y2]=map_step[x2][y2]+map_forecast[x2][y2];//�����x2��y2��Ȩֵ���������� 
		line_count++;//ջ������+1 
		if(map_flag[x2][y2]==true)//����õ���֮ǰ�Ѿ��߹�����·��û�з��� 
		D_flag=x2*m+y2;//���õ���Ϊ��ֹ�������� 
		insert(x2,y2,map_value[x2][y2],line_count);//��ջ�в��� 
	}
}

void add_line(int x,int y)//�߽��жϣ��Ƿ���Խ���add���� 
{
	if(x<n) add(x,y,x+1,y);//�±߽� 
	if(x>1) add(x,y,x-1,y);//�ϱ߽� 
	if(y<m) add(x,y,x,y+1);//�ұ߽� 
	if(y>1) add(x,y,x,y-1);//��߽� 
}

void read(void)//�����Լ���ʼ�� 
{
	scanf("%d%d",&n,&m);//���뷽���С 
	for(int i=1;i<=n;i++)//����ԭ���1.1��ʼ 
	for(int j=1;j<=m;j++)
	{
		scanf("%d",&map[i][j]);//�����ϰ� 
		map_step[i][j]=1<<29;//��ʼ��·������ֵ 
	}
	scanf("%d%d%d%d",&line[1].x,&line[1].y,&end_x,&end_y);//������� �յ� 
	start_x=line[1].x; 
	start_y=line[1].y;//��㸳ֵ 
	move_x=end_x;
	move_y=end_y;//�ƶ��յ������ֵ 
	map_step[line[1].x][line[1].y]=0; //����ʼ�� 
	for(int i=1;i<=n;i++)
	for(int j=1;j<=m;j++)
	map_forecast[i][j]=abs(end_x-i)+abs(end_y-j);//Ԥ��ֵ��H��ʼ�� 
}

void A_(void)
{
	int lx,ly;
	while(flag[end_x][end_y]==false)//ֱ���յ㱻������֮ǰ��һֱѭ�� 
	{
		lx=line[1].x;
		ly=line[1].y;
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);//����ջ��Ԫ�أ��������� 
		line_count--;//ջ����-1 
		add_line(lx,ly);//�����ڷ���ջ�׵������������ 
		
	}
}

void D_(void)
{
	D_flag=0;//����D*�յ�ֵ����ʼ�� 
	int lx,ly,p=0;
	while(D_flag==0&&p<=1000)//��û���ҵ�����·������һ��ʱ���������� 
	{
		p++;
		lx=line[1].x;
		ly=line[1].y;
		pop(line[line_count].x,line[line_count].y,map_value[line[line_count].x][line[line_count].y],1);///����ջ��Ԫ�أ��������� 
		line_count--;//ջ����-1
		add_line(lx,ly);//�����ڷ���ջ�׵������������ 
		 
	}
	if(D_flag==0)
	{
		writetest();
		printf("\n\n\n");
		writetest2();
		printf("�޽�\n");
		exit(0);
	}
} 

 
void step_update(int x,int y,int x1,int y1)//���������ϰ��������·����ʱ����ʼ���� 
{
	t++;
	line_count=1;//��ʼ��ջ��Ԫ������ 
	
	line[1].x=x;
	line[1].y=y;
	//��ֵ����,��Ԫ�ؼ���ʼ��Ϊ��ǰ�� 
	
	for(int i=0;i<=n;i++)
	for(int j=0;j<=m;j++)
	map_step[i][j]=1<<29;
	map_step[x][y]=0;
	//��ʼ��G����ʵ��ֵ 
	 
	D_();//��ʼD* 
	
	move_x=(D_flag-1)/m; 
	move_y=(D_flag-1)%m+1;
	//ת���յ�����
	
	
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
	//���յ����굽�ϰ������֮���·�����ɾ�� 
	
	map_line_flag();//�������ϰ���㣨������ʼ�㣩�����ڵ��յ���������
	 
	writetest();//��� 
	
//	exit(0);
	D_flag=0;
}

void write_step(int x,int y)
{
	map[x][y]=t;//������ǰ�㣬���Ϊ8 
	
	map_flag[x][y]=false;//��ǰ������գ�����δ�ߵ�·���� 
	
	if((rand()%1==0))//������ж��Ƿ�����ϰ� 
	{
		int lx=x+rand()%6-3,ly=y+rand()%6-3;//������ϰ��������꣬�궨Ϊ��������3���У�lx�Ǻ����꣬ly�������� 
		 
		if((lx<=n)&&(lx>=1)&&(ly<=m)&&(ly>=1)&&(map[lx][ly]==0))//������ϰ�û��Խ�� 
		{
			map[lx][ly]=1;//�ϰ����
			
			 
			if(map_flag[lx][ly])//�ж��ϰ��Ƿ������·���� 
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
				//�������ڵ㵽�ϰ���·���ϵ����б�ע��
				
				map_flag[x][y]=false;
				step_update(x,y,lx,ly);//���£������������Ϊ��ǰ�㣬
				map_flag[x][y]=false; 
			}
			
			
		}
	}
 }
 
 
 
void map_line_flag(void)//����ǵ����� 
{
 	map_flag[move_x][move_y]=true;//�õ�ᱻ���� 
	
	while(map_step[move_x][move_y]!=0)//ֱ�����յ��ߵ����ʱ���յ㵹���ߣ�����С��һ���Ȼ���Ե�����㣬��֮��Ȼ 
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
	//���� rootΪ�õ����һ�㣬nextΪ�õ����һ�� ���û��root���յ�û��next 
}



void write(void)
{	
	int temp_x=start_x,temp_y=start_y;//��ʱ���� 
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

 void writetest2(void)//��������ʵ�ʲ��� 
 {
 	for(int i=1;i<=n;i++)//���
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

void writetest(void)//�����ǲ��� 
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


