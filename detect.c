#include<stdio.h>
#define MAX 4
int main()
{
  int N,M,P,Q;
  double z[MAX][2];
  int i,j,r[4];
  double check,x,y,s,t;
  double A[2][2];
  double B[2];
  double EPS1 = 0.0000001,EPS2 = 0.00000001;

  scanf("%d %d %d %d",&N,&M,&P,&Q);
  for(i=0;i<N;i++)
    {
      scanf("%lf %lf",&z[i][0],&z[i][1]);
      /* printf("%f %f\n",z[i][0],z[i][1]);*/
      
    }
  
  scanf("%d %d %d %d",&r[0],&r[1],&r[2],&r[3]);
  /*printf("%d %d %d %d\n",r[0],r[1],r[2],r[3]);*/

  for(i=0;i<N;i++)
    {
      r[i]-=1;
    }
  
  check = (z[r[1]][0] - z[r[0]][0])*(z[r[2]][1] - z[r[3]][1]) + (z[r[3]][0] - z[r[2]][0])*(z[r[1]][1] - z[r[0]][1]);
  /*for(i=0;i<N;i++)
    {
      printf("%f %f\n",z[r[i]][0],z[r[i]][1]);
      }*/    
    if(check < 0) check *= -1;
    if(check == 0)
      {
	printf("NA\n");
	return 0;
      }
    if(EPS2<=check && check<=EPS1)
      {
	printf("NA\n");
	return 0;
      }
   
    A[0][0] = z[r[2]][1] - z[r[3]][1];
    A[0][1] = z[r[3]][0] - z[r[2]][0];
    A[1][0] = z[r[0]][1] - z[r[1]][1];
    A[1][1] = z[r[1]][0] - z[r[0]][0];
    B[0] = z[r[2]][0] - z[r[0]][0];
    B[1] = z[r[2]][1] - z[r[0]][1];

    s = (A[0][0] * B[0] + A[0][1] * B[1]) / check;
    t = (A[1][0] * B[0] + A[1][1] * B[1]) / check;
    /*printf("%f %f %f\n",check,s,t);*/

    if(0<s && s<=1)
      {
	if(0<t && t<=1)
	  {
	x = z[r[0]][0] + (z[r[1]][0] - z[r[0]][0]) * s;
	y = z[r[0]][1] + (z[r[1]][1] - z[r[0]][1]) * s;
	printf("%f %f\n",x,y);
	return 0;
	  }
      }
    else
      {
	printf("NA\n");
	return 0;
      }
}
