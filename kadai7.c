#include<stdio.h>
#include <math.h>
#define MAX 100
#define INF 10000000

struct path{
    double A1;/*行列式|A|を表す*/
    int P1[2];
    int P2[2];
    int Q1[2];
    int Q2[2];
    int flag;
  };

typedef struct{ //小課題7 新しい交差地点
  double x;
  double y;
} Point;

Point getNearest(Point, Point, double, double);

int main(){
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
  Point p[2], nearest;
  double a, c, denom, dst, shortestDst = INF;

  scanf("%d %d %d %d",&N,&M,&P,&Q);
  for(i=0;i<N;i++)/*座標の値を入力する*/
    {
      scanf("%lf %lf",&z[i][0],&z[i][1]);
    //  printf("%f %f\n",z[i][0],z[i][1]);  
    }
  for(i=0;i<M;i++){
      for(j=0;j<2;j++){
	  scanf("%d",&r[i][j]);/*座標の値を入力する*/
	  r[i][j]-=1;
	//  printf(" %d %d\n",i,r[i][j]);
	  }
  }
  
  k=0;
  for(i=0; i<P; i++){ // 新たな座標入力
    scanf("%lf %lf", &z[2*i+N][0], &z[2*i+N][1]);
  }

  for(i=0; i<P; i++){
    a = c = denom = 0;
    for(j=0; j<M; j++){
      a = (z[r[j][0]][1]-z[r[j][1]][1])/(z[r[j][0]][0]-z[r[j][1]][0]);
      c = z[r[j][0]][1]-(a*z[r[j][0]][0]);
      denom = -a*z[2*i+N][0] + z[2*i+N][1] - c;
      if(denom < 0) denom *= -1;
      dst = denom / sqrt(a*a + 1); // dst: 直線と点の距離
//      printf("dst %d = %lf\n", j, dst);

      if(shortestDst > dst){
        if((z[r[j][0]][0] <= z[2*i+N][0] && z[2*i+N][0] <= z[r[j][1]][0]) || 
            (z[r[j][0]][0] >= z[2*i+N][0] && z[2*i+N][0] >= z[r[j][1]][0])){
          shortestDst = dst;
          p[0].x = z[r[j][0]][0];
          p[0].y = z[r[j][0]][1];
          p[1].x = z[r[j][1]][0];
          p[1].y = z[r[j][1]][1];
        }
        else if((z[r[j][0]][1] <= z[2*i+N][1] && z[2*i+N][1] <= z[r[j][1]][1]) || 
                (z[2*i+N][1] <= z[r[j][0]][1] && z[r[j][1]][1] <= z[2*i+N][1])){
          shortestDst = dst;
          p[0].x = z[r[j][0]][0];
          p[0].y = z[r[j][0]][1];
          p[1].x = z[r[j][1]][0];
          p[1].y = z[r[j][1]][1];
        }
      }
    }
//    printf("shortestDst = %f\n", shortestDst);
    nearest = getNearest(p[0], p[1], z[2*i+N][0], z[2*i+N][1]);
    printf("%.5f %.5f\n", nearest.x, nearest.y);
      //nearestをzに追加
    r[M+i][0] = N+k;
    r[M+i][1] = N+k+1;
    k += 2;
    z[r[M+i][1]][0] = nearest.x;
    z[r[M+i][1]][1] = nearest.y;
    M++;

    shortestDst = INF;
  }
  return 0;
}

// 直線と点の交点の座標を求める
Point getNearest(Point A, Point B, double nx, double ny){
  Point a, b;
  double r;

  a.x = B.x - A.x;
  a.y = B.y - A.y;
  b.x = nx - A.x;
  b.y = ny - A.y;

  r = (a.x*b.x + a.y*b.y) / (a.x*a.x + a.y*a.y);

  if(r <= 0){
    return A;
  }
  else if(r >= 1){
    return B;
  }
  else{
    Point result;
    result.x = A.x + r*a.x;
    result.y = A.y + r*a.y;
    return result;
  }
}