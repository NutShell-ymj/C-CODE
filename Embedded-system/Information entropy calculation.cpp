#include<stdio.h>
#include<math.h>

 
int main()//������Ϣ�� H(X) 
{
	while(1)
	{
		
		int n=0;
 
		double result=0;
		printf("������H(X)������Ԫ�ظ�����");	
		scanf("%d",&n);
		double A[n];
		double B[n];
		for(int i=0;i<n;i++)
		{
			printf("H(X)�е�%d��Ԫ��Ϊ��",i+1);
	
			scanf("%lf",&A[i]);	
		}
		for(int m=0;m<n;m++)
		{
			B[m]=-((A[m])*log2(A[m]));
			result += B[m];
		}
		printf("The result is %lf\n\n",result);
	}
} 


