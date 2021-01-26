#include<iostream>
#include<ctime>
#include<stdlib.h> 
using namespace std;
int ss[104][104];
int main()
{
	
	srand(time(0));
	freopen("test.txt","w",stdout);
    
    int n=rand()%99+1,m=rand()%99+1;
    cout<<n<<" "<<m<<endl;
    for(int i=1;i<=n;i++)
    {
    	for(int j=1;j<=m;j++)
    	{
    		if((rand()%2==0)&&(rand()%2==0))
    		ss[i][j]=2;
    		else ss[i][j]=0;
		}
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		cout<<ss[i][j]<<" ";
		cout<<endl;
	}
	int x1=rand()%n+1;
	int y1=rand()%m+1;
	while(ss[x1][y1]!=0)
	{
		x1=rand()%n+1;
		y1=rand()%m+1;
	}
	cout<<x1<<" "<<y1<<" ";
	x1=rand()%n+1;
	y1=rand()%m+1;
	while(ss[x1][y1]!=0)
	{
		x1=rand()%n+1;
		y1=rand()%m+1;
	}
	cout<<x1<<" "<<y1<<" ";
    return 0;
}
