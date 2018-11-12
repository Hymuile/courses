# Learning C language
/*判断输入数字的奇偶个数 
#include<stdio.h>
int main()
{
	int a,b,c,d,e,f,g,h,i,j,n=1,x=0,y=0,z=0;
	scanf("%d %d %d %d %d %d %d %d %d %d",&a,&b,&c,&d,&e,&f,&g,&h,&i,&j);	
	while(n<11)
	{	
		if(a%2==0){x++;z++;n++;}
		else{y++;z++;n++;}
		
		if(z==1){a=b;}
		else if(z==2){a=c;}
		else if(z==3){a=d;}
		else if(z==4){a=e;}
		else if(z==5){a=f;}
		else if(z==6){a=g;}
		else if(z==7){a=h;}
		else if(z==8){a=i;}
		else if(z==9){a=j;}
	}
	printf("奇数y=%d,偶数x=%d\n",y,x);
	return 0;}*/
	
/*情侣最佳身高差
版本1： 
#include<stdio.h>
int main()	
{	
	int i=0;
	while(i<5)
	{
		float b,c;
		char a;
		scanf("%c %f",&a,&b);
			if (a=='f')
			{
				c=1.09*b;
				i++;
				printf("%d %.2f\n",i,c);
			}
			else if (a=='m')
			{
				c=b/1.09;
				i++;
				printf("%d %.2f\n",i,c);
			}	
		
	}
	return 0;}
版本2：	
#include<stdio.h>
int main()
{
	int i=0;
	for(i;i<5;i++)
	{
		float b,c;
		char a;
		scanf("%c %f",&a,&b);
		if (a=='f')
			c=b*1.09;
		else
			c=b/1.09;
		printf("%d %.2f\n",i,c);
		fflush(stdin);/a=getchar();/while(getchar()!='\n');/
	}
	return 0;}*/

/*统计输入字符的类别及个数 
#include<stdio.h>
#include<windows.h>
int main()
{
	char c;
	int letters = 0;
	int space = 0;
	int digit = 0;
	int others = 0;
	printf("please input a string:>");
	while ((c = getchar()) != '\n')
	{
		if (c >= 'a'&&c <= 'z' || c >= 'A'&&c <= 'Z')
			letters++;
		else if (c == ' ')
			space++;
		else if (c >= '0'&&c <= '9')
			digit++;
		else
			others++;
	}
	printf("字母=%d 空格=%d 数字=%d 其他=%d\n", letters, space, digit, others);
	return 0;
}*/

/*存素数 
版本一 
#include<stdio.h> 
int isprime(int n)
{
	int i;
	for(i=2;i<=(int)sqrt(n);i++)
		if(n%i==0)
			return 0;
	return 1;
}
int main()
{
	int n;
	scanf("%d",&n);
	for (int a=0;a<n;a++)
	{
		int i,m;
		scanf("%d",&m);
		if (m<2)
		{printf("Input Error\n");}
		else if(m>546)
		{printf("Overflow\n");}
		else
		{
			for (i=2;i<=m;i++)
			{
				if(isprime(i))
					printf("%d,",i);
			}
			printf("\n");
		}
	}
	return 0; 
}
版本二 
#include<stdio.h>
#include<math.h>
float abd(int x)
{
	double k;
	int i=2;
	k=sqrt(x);
	for(i;i<=k;i++)
		if(x%i==0)
			return 0;
	if (i>k)
		return 1;
}
int main()
{
	int m;
	scanf("%d",&m);
	for (int a=0;a<m;a++)
	{
		int b;
		scanf("%d",&b);
		if (b<2){printf("Input Error\n");}
		else if(b>546){printf("Overflow\n");}
		else
		{	for(int i=2;i<=b;i++)
				if(abd(i))
					printf("%d,",i);
			printf("\n");
		}
	}
	return 0;
}*/

/*#计算奇数和 
include<stdio.h>
int sum(int x,int y);
int main()
{
	int a;
	scanf("%d",&a);
	for(int i=0;i<a;i++)
	{
		int b,c,s;
		scanf("%d %d",&b,&c);
		if(b<1||c<1){printf("error\n");continue;}
		s=sum(b,c);
		printf("%d\n",s);
	}
	return 0;
} 
int sum(int x,int y)
{
	int t,s=0;
	if(x>y){t=x;x=y;y=t;}
	if(x%2==0)
	{
		x+=1;
		for(x;x<=y;x+=2)
			s=s+x;
	}
	else if(x%2==1)
	{
		for(x;x<=y;x+=2)
		{
			s=s+x;
		}
	}
	return s;
} */ 

/*分数序列求和 
#include<stdio.h>
float sum(int x);
int main()
{
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		int j,k;
		scanf("%d",&j);
		printf("%.2f\n",sum(j));
		getchar();
	}
	return 0;
}
float sum(int x)
{
	float a=1,b=2,s=0,t;
	for(int h=0;h<x;h++)
	{
		s=s+a/b;
		t=a;
		a=b;
		b=t+b;
	}
	return s;
}*/

/*找丑数 
#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++)
	{
		int a;
		scanf("%d",&a);
		if(a==1)
			printf("false\n"); 
		else
		{	while(a%2==0)
				{a=a/2;}
			while(a%3==0)
				{a=a/3;}
			while(a%5==0)
				{a=a/5;}
			if (a==1)
				printf("true\n");
			else
				printf("false\n");
		}
	} 
	return 0;
}*/
	
/*输入十组数判定奇偶个数 
#include<stdio.h>
int main()
{
	int a,b,c,d,e,f,g,h,i,j,n=0,x=0,y=0,z=0;
	//a-j为输入10个变量；n结束循环；x，y代表偶数奇数个数；z为了从第一个变量判断换为下一个变量判断 
	scanf("%d %d %d %d %d %d %d %d %d %d",&a,&b,&c,&d,&e,&f,&g,&h,&i,&j);	
	while(n<10)
	{	
		if(a%2==0){x++;z++;}
		else{y++;z++;}
		switch(z)
		{
			case 1:{a=b;n++;break;}
			case 2:{a=c;n++;break;}
			case 3:{a=d;n++;break;}
			case 4:{a=e;n++;break;}
			case 5:{a=f;n++;break;}
			case 6:{a=g;n++;break;}
			case 7:{a=h;n++;break;}
			case 8:{a=i;n++;break;}
			case 9:{a=j;n++;break;}
			default:{n++;break;}
		}
	}
	printf("奇数y=%d,偶数x=%d\n",y,x);
	return 0;
}*/

/*北大1003 
#include<stdio.h>
float f(float x)
{
	float n=1;
	double s=0;
	while(1)
	{	
		n+=1;
		s=s+1/n;
		if (s>x)
		{	
			n-=1;
			break;}	
	}
	return n;
} 

int main(void)
{
	while(1)
	{	
		float b;
		int t;
		scanf("%f",&b);
		if(b==0.00)
			break;
		else if(b>=0.01&&b<=5.20)
		{	
			t=f(b);
			printf("%d card(s) \n",t);
		}
	}
	return 0;
}*/
