#include<stdio.h>
#define MAX 100


  struct path{
    double A1;/*行列式|A|を表す*/
    int P1[2];
    int P2[2];
    int Q1[2];
    int Q2[2];
    int flag;
  };

int main()
{
  struct path  check[100];
  int N,M,P,Q;
  double z[MAX][2];/*座標を格納する。*/
  int i,j,r[100][2],k=0,l=0,flag[100];/*rは道をつくる座標番号を格納する。*/
  double x,y,s,t,keyx,keyy;
  double A[2][2];/*s,tを求める際の左行列*/
  double B[2];/*s,tを求める際の右行列*/
  double EPS1 = 0.0000001,EPS2 = 0.00000001;
  double Z[MAX][2];
  double cross[100][2];

  scanf("%d %d %d %d",&N,&M,&P,&Q);
  for(i=0;i<N;i++)/*座標の値を入力する*/
    {
      scanf("%lf %lf",&z[i][0],&z[i][1]);
      printf("%f %f\n",z[i][0],z[i][1]);
      
    }
  for(i=0;i<M;i++)
    {
      for(j=0;j<2;j++)
	{
	  scanf("%d",&r[i][j]);/*座標の値を入力する*/
	  r[i][j]-=1;
	  printf(" %d %d\n",i,r[i][j]);
	}
    }
  

  for(i=0;i<M-1;i++)/*P1,Q1の値を固定してまわす*/
    {
      for(j=i+1;j<M;j++)/*P2,Q2の値をかえてまわす*/
	{
	   check[k].A1 = (z[r[i][1]][0]-z[r[i][0]][0])*(z[r[j][0]][1]-z[r[j][1]][1])+(z[r[j][1]][0]-z[r[j][0]][0])*(z[r[i][1]][1]-z[r[i][0]][1]);
	  check[k].P1[0]=z[r[i][0]][0];
	  check[k].P1[1]=z[r[i][0]][1];
	  check[k].P2[0]=z[r[j][0]][0];
	  check[k].P2[1]=z[r[j][0]][1];
	  check[k].Q1[0]=z[r[i][1]][0];
	  check[k].Q1[1]=z[r[i][1]][1];
	  check[k].Q2[0]=z[r[j][1]][0];
	  check[k].Q2[1]=z[r[j][1]][1];
	
	  /*printf("%f ",check[k]);*/
	  k++;
	  
	}
    }

  
  /*scanf("%d",&r[M*2-1]);
    printf("%d",r[M*2-1]);*/
 
  /*for(i=0;i<N;i++)
    {
      printf("%f %f\n",z[r[i]][0],z[r[i]][1]);
      }*/
  for(i=0;i<k;i++)
    {
      if(check[i].A1 < 0) check[i].A1 *= -1;
      if(check[i].A1 == 0)
	{
	  check[i].flag = 0;/*交わらない flag=0*/
	  continue;
	}
      if(EPS2<=check[i].A1 && check[i].A1<=EPS1)
	{
	  check[i].flag = 0;
	  continue;
	}
      check[i].flag=1;/*交わる*/
    }

  for(i=0;i<k;i++)
    {
      if(check[i].flag=1){
	A[0][0] = check[i].P2[1] - check[i].Q2[1];
	A[0][1] = check[i].Q2[0] - check[i].P2[0];
	A[1][0] = check[i].P1[1] - check[i].Q1[1];
	A[1][1] = check[i].Q1[0] - check[i].P1[0];
	B[0] = check[i].P2[0] - check[i].P1[0];
	B[1] = check[i].P2[1] - check[i].P1[1];

	s = (A[0][0] * B[0] + A[0][1] * B[1]) / check[i].A1;
	t = (A[1][0] * B[0] + A[1][1] * B[1]) / check[i].A1;

	/*printf("s:%d t:%d\n",s,t);*/


	/*printf("%f %f %f\n",check,s,t);*/

	if(0<s && s<1)
	  {
	    if(0<t&&t<1)
	      {
		x = check[i].P1[0] + (check[i].Q1[0] - check[i].P1[0]) * s;
		y = check[i].P1[1] + (check[i].Q1[1] - check[i].P1[1]) * s;
		cross[l][0] = x;
		cross[l][1] = y;
		/*printf("cross: %f %f\n",cross[l][0],cross[l][1]);*/
		l++;
	      }
	  }
      }
      else continue;
    }

  for(i=0;i<l-1;i++)
    {
      if(cross[l][0]>cross[l+1][0])
	{
	  keyx = cross[l][0];
	  keyy = cross[l][1];
	  cross[l][0] = cross[l+1][0];
	  cross[l][1] = cross[l+1][1];
	  cross[l+1][0] = keyx;
	  cross[l+1][1] = keyy;
	}
    }
  for(i=0;i<l;i++)
    {
      printf("%f %f\n",cross[i][0],cross[i][1]);
    }
  
  return 0;
}
 
