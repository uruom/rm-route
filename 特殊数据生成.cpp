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
    n=20;m=20;
    printf("%d %d\n",n,m); 
    for(int i=1;i<=n;i++)
    {
    	for(int j=1;j<=m;j++)
    	printf("0 ");
    	printf("\n");
	}
	printf("7 8 12 16");
    return 0;
}
