#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define NMAX 200
#define QMAX 100
#define CMAX 1000/*交差点の最大数*/
#define PMAX 1000/*道の最大数*/

typedef struct
{
  int edges_to[PMAX];
  double edges_cost[PMAX],cost;
  int n_edges, done, from;
} node; //重み付きグラフのノード

typedef struct
{
  double A1;/*行列式|A|を表す*/
  int p1;
  int q1;
  int p2;
  int q2;
  int P1[2];
  int P2[2];
  int Q1[2];
  int Q2[2];
  int flag;
} Check;

typedef struct
{
  double cross[2];
  int p1;
  int q1;
  int p2;
  int q2;
} Cross;

typedef struct
{
  int start;
  int goal;
  double len;
} Path;

void struct_swap(Cross *i,Cross *j)//交点を並び変える関数
{
  Cross temp;

  temp = *j;
  *j = *i;
  *i = temp;

  return ;
}

void myf(char *s1, char *s2) //Qの入力にCが含まれる場合Cを除く関数
{
  char *p=s1;

  p=strstr(s1,s2);
  if( p!=NULL ) {
    strcpy(p,p+strlen(s2));
    myf( p+1, s2 );
    return ;
  }
}

void get_path(node *nodes,int size,int dep); //最短経路を探索する関数
 
int main()
{  
  Check  check[CMAX];
  Path path[1000];
  int N,M,P,Q;
  double z[NMAX][2]; /*座標を格納する。それに続いて交差点の座標をx座標が小さい順に格納する*/
  int size,size_node,i,j,r[CMAX][2],num_con[200],num_node,num_cross,k=0,l=0,m=0,flag[100],dep,dest; /*rは道をつくる座標番号を格納する。 lは交差した分だけインクリメントされる*/
  int f;
  int q[QMAX],point[200]; /*Qの三つ目の引数ki*/
  double x,y,s,t,keyx,keyy,len;  
  double A[2][2]; /*s,tを求める際の左行列*/
  double B[2]; /*s,tを求める際の右行列*/
  double EPS1 = 0.0000001,EPS2 = 0.00000001;
  double Z[NMAX][2];
  Cross cross[CMAX];
  char cstart[NMAX+2];
  char cgoal[NMAX+2];
  char C[]="C";
  char cpoint[200]="C";
  char temp[200];
  char goal[200];
  node nodes[200];
   
  scanf("%d %d %d %d",&N,&M,&P,&Q);
  for(i=0;i<N;i++)//座標の値を入力する
    {
      scanf("%lf %lf",&z[i][0],&z[i][1]);
      num_con[i] = 0;
    }
  
  for(i=0;i<M;i++)
    {
      for(j=0;j<2;j++)
	{
	  scanf("%d",&r[i][j]);//道を作る座標の値を入力する
	  r[i][j]-=1;
	}
    }
  

  for(i=0;i<M-1;i++)//P1,Q1の値を固定してまわす
    {
      for(j=i+1;j<M;j++)//P2,Q2の値を変えてまわす
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
	  check[k].p1=r[i][0]; //交差点をつくる道の座標番号を格納
	  check[k].q1=r[i][1];
	  check[k].p2=r[j][0];
	  check[k].q2=r[j][1];
	  k++;
	  
	}
    }

  
  //scanf("%d",&r[M*2-1]);
  // printf("%d",r[M*2-1]);
 
  /*for(i=0;i<N;i++)
    {
      printf("%f %f\n",z[r[i]][0],z[r[i]][1]);
      }*/
  
  for(i=0;i<k;i++)
    {
      if(check[i].A1 < 0) check[i].A1 *= -1;
      if(check[i].A1 == 0)
	{
	  check[i].flag = 0;//交わらない flag=0
	  continue;
	}
      if(EPS2<=check[i].A1 && check[i].A1<=EPS1)
	{
	  check[i].flag = 0;
	  continue;
	}
      check[i].flag=1;//交わる flag=1
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

	if(0<s && s<1)
	  {
	    if(0<t&&t<1)
	      {
		x = check[i].P1[0] + (check[i].Q1[0] - check[i].P1[0]) * s;
		y = check[i].P1[1] + (check[i].Q1[1] - check[i].P1[1]) * s;
		cross[l].cross[0] = x;
		cross[l].cross[1] = y;
		cross[l].p1 = check[i].p1;
		cross[l].q1 = check[i].q1;
		cross[l].p2 = check[i].p2;
		cross[l].q2 = check[i].q2;
		//printf("cross: %f %f\n",cross[l][0],cross[l][1]);
		l++;
	      }
	  }
      }
    }
  num_cross=l; //交差点の数を格納
  
  for(i=0;i<num_cross-1;i++) //交差点を小さい順にソート
    {
      if(cross[i].cross[0]>cross[i+1].cross[0])
	{
	  struct_swap(&cross[i],&cross[i+1]);
	}
    }
   
    k=0;
    for(i=N;i<N+l;i++)/*交差点を小さい順にｚ配列に格納*/
       {
	z[i][0] = cross[k].cross[0];
	z[i][1] = cross[k].cross[1];
	k++;
      }
    size = i; //交差点を含めた座標の数を格納
    for(j=0;j<num_cross;j++)
      {
	//	printf("%d %d %d %d\n",cross[j].p1,cross[j].q1,cross[j].p2,cross[j].q2);
      }

    k=0;
     for(i=0;i<M;i++) //入力で得た座標で交点を作らない道の始点と終点、重みを求める
     {
       f=0;
	for(j=0;j<num_cross;j++)
	  {
	    if(r[i][0] == cross[j].p1)
	      {
		if(r[i][1] == cross[j].q1){
		  f=1;
		  break;
		}
	      }
	    if(r[i][0] == cross[j].q1)
	      {
		if(r[i][1] == cross[j].p1)
		  {
		    f=1;
		    break;
		  }
	      }
	    if(r[i][0] == cross[j].p2)
	      {
		if(r[i][1] == cross[j].q2)
		  {
		    f=1;
		    break;
		  }
	      }
	    if(r[i][0] == cross[j].q2)
	      {
		if(r[i][0] == cross[j].p2){
		  f=1;
		  break;
		}
	      }
	    //	printf("%d\n",f);
	    if(f!=1){
	      path[k].start = r[i][0];
	      path[k].goal = r[i][1];
	      num_con[r[i][0]]++;
	      num_con[r[i][1]]++;
	      len = (z[r[i][1]][0]-z[r[i][0]][0])*(z[r[i][1]][0]-z[r[i][0]][0])+(z[r[i][1]][1]-z[r[i][0]][1])*(z[r[i][1]][1]-z[r[i][0]][1]);
	      path[k].len = sqrt(len);
	      k++;
	      path[k].start = r[i][1];
	      path[k].goal = r[i][0];
	      path[k].len = sqrt(len);
	      k++;	
	    }
	  }
     }
	
	  
	    
    for(i=0;i<num_cross-1;i++) //同じ直線上に交差点が二つある場合の処理
      {
	for(j=i+1;j<num_cross;j++)
	  {
	    if(cross[i].p1 == cross[j].p1 && cross[i].q1 == cross[j].q1)
	      {
		if(z[cross[i].p1][0]>z[cross[i].q1][0])
		  {
		    cross[i].p1 = N+j;
		    cross[j].q1 = N+i;
		  }
		else if(z[cross[i].p1][0]<z[cross[i].q1][0])
		  {
		    cross[i].q1 = N+j;
		    cross[j].p1 = N+i;
		  }
	      }

	    if(cross[i].p2 == cross[j].p2 && cross[i].q2 == cross[i].q2)
	      {
		if(z[cross[i].p2][0]>z[cross[i].q2][0])
		  {
		    cross[i].p2 = N+j;
		    cross[j].q2 = N+i;
		  }
		else if(z[cross[i].p2][0]<z[cross[i].q2][0])
		  {
		    cross[i].q2 = N+j;
		    cross[i].p2 = N+i;
		  }
	      }
	    if(cross[i].p1 == cross[j].q1 && cross[i].q1 == cross[j].p1)
	      {
		if(z[cross[i].p1][0]>z[cross[i].q1][0])
		  {
		    cross[i].p1 = N+j;
		    cross[j].p1 = N+i;
		  }
		else
		  {
		    cross[i].q1 = N+j;
		    cross[j].q1 = N+i;
		  }
	      }
	    if(cross[i].p2 == cross[j].q2 && cross[i].q2 == cross[j].p2)
	      {
		if(z[cross[i].p2][0]<z[cross[i].q2][0])
		  {
		    cross[i].q2 = N+j;
		    cross[j].q2 = N+i;
		  }
		else
		  {
		    cross[i].p2 = N+j;
		    cross[j].p2 = N+i;
		  }
	      }
	    if(cross[i].p1 == cross[j].p2 && cross[i].q1 == cross[j].p2)
	      {
		if(z[cross[i].p1][0]<z[cross[i].q1][0])
		  {
		    cross[i].q1 = N+j;
		    cross[j].p2 = N+i;
		  }
		else if(z[cross[i].p1][0]>z[cross[i].q1][0])
		  {
		    cross[i].p1 = N+j;
		    cross[j].q2 = N+i;
		  }
	      }
	    if(cross[i].p2 == cross[j].p1 && cross[i].q2 == cross[j].q1)
	      {
		if(z[cross[i].p2][0]<z[cross[i].q2][0])
		  {
		    cross[i].q2 = N+j;
		    cross[j].p1 = N+i;
		  }
		else if(z[cross[i].p2][0]>z[cross[i].q2][0])
		  {
		    cross[i].p2 = N+j;
		    cross[j].q1 = N+i;
		  }
	      }
	    if(cross[i].p1 == cross[j].q2 && cross[i].q1 == cross[j].p2)
	      {
		if(z[cross[i].p1][0]<z[cross[i].q1][0])
		  {
		    cross[i].q1 = N+j;
		    cross[j].q2 = N+i;
		  }
		else if(z[cross[i].p1][0]>z[cross[i].q1][0])
		  {
		    cross[i].p1 = N+j;
		    cross[j].p2 = N+i;
		  }
	      }
	    if(cross[i].p2 == cross[j].p1 && cross[i].q2 == cross[j].q1)
	      {
		if(z[cross[i].p2][0]<z[cross[i].q2][0])
		  {
		    cross[i].q2 = N+j;
		    cross[j].p1 = N+i;
		  }
		else if(z[cross[i].p2][0]>z[cross[i].q2][0])
		  {
		    cross[i].p2 = N+j;
		    cross[j].p1 = N+i;
		  }
	      }
	  }
      }
	  
 
    m = k;
    for(j=0;j<num_cross;j++) //交差点を含む道のの始点と終点、重みを求める
      { 
	path[m].start = N+j;
	path[m].goal =  cross[j].p1;
	num_con[N+j]++;
	num_con[cross[j].p1]++;
	len = (z[cross[j].p1][0]-z[N+j][0])*(z[cross[j].p1][0]-z[N+j][0])+(z[cross[j].p1][1]-z[N+j][1])*(z[cross[j].p1][1]-z[N+j][1]);
	path[m].len = sqrt(len);
	m++;
	path[m].start = cross[j].p1;
	path[m].goal = N+j;
	path[m].len = sqrt(len);
	m++;
	
	path[m].start = N+j;
	path[m].goal =  cross[j].q1;
	num_con[N+j]++;
	num_con[cross[j].q1]++;
	len = (z[cross[j].q1][0]-z[N+j][0])*(z[cross[j].q1][0]-z[N+j][0])+(z[cross[j].q1][1]-z[N+j][1])*(z[cross[j].q1][1]-z[N+j][1]);
	path[m].len = sqrt(len);
	m++;
	path[m].start = cross[j].q1;
	path[m].goal = N+j;
	path[m].len = sqrt(len);
	m++;

	path[m].start = N+j;
	path[m].goal =  cross[j].p2;
	num_con[N+j]++;
	num_con[cross[j].p2]++;
	len = (z[cross[j].p2][0]-z[N+j][0])*(z[cross[j].p2][0]-z[N+j][0])+(z[cross[j].p2][1]-z[N+j][1])*(z[cross[j].p2][1]-z[N+j][1]);
	path[m].len = sqrt(len);
	m++;
	path[m].start = cross[j].p2;
	path[m].goal = N+j;
	path[m].len = sqrt(len);
	m++;

	path[m].start = N+j;
	path[m].goal =  cross[j].q2;
	num_con[N+j]++;
	num_con[cross[j].q2]++;
	len = (z[cross[j].q2][0]-z[N+j][0])*(z[cross[j].q2][0]-z[N+j][0])+(z[cross[j].q2][1]-z[N+j][1])*(z[cross[j].q2][1]-z[N+j][1]);
	path[m].len = sqrt(len);
	m++;
	path[m].start = cross[j].q2;
	path[m].goal = N+j;
	path[m].len = sqrt(len);
	m++;
      }
    
    l=0;
    for(i=0;i<size;i++)
      {
	if(num_con[i] == 0) continue; //参照座標の接続先がないならcontinue
	nodes[l].n_edges = num_con[i]; //参照座標の接続数を代入
	for(j=0;j<nodes[l].n_edges;j++)
	  {
	    for(k=0;k<m;k++)
	      {
		if(path[k].start == i) //接続先の座標番号と重みを代入
		  {
		    nodes[l].edges_to[j] = path[k].goal;
		    nodes[l].edges_cost[j] = path[k].len;
		    j++;
		  }
	      }
	  }
	l++;
      }
    size_node = l; 

    for(i=0;i<Q;i++)
    {
      scanf("%s %s ",cstart,cgoal); //始点と終点を入力
      scanf("%d",&q[i]); //ｋの値を入力
      strcpy(goal,cgoal);
      if(cstart[0] == 'C') //入力が交点だった場合その交点に割り当てられた座標番号に変える 
	{
	  myf(cstart,C);
	  dep = atoi(cstart);
	  dep = N+dep;
	}
      else dep = atoi(cstart);

      if(cgoal[0] == 'C')
	{
	  myf(cgoal,C);
	  dest = atoi(cgoal);
	  dest = N+dest;
	}
      else dest = atoi(cgoal);
      dep--;
      dest--;
      if(dep>size_node || dest>size_node)
	{
	  printf("NA\n");
	  continue;
	}
      // printf("%d %d %d\n",dep,dest,q[i]);
      get_path(nodes,size_node,dep);
      if(nodes[dest].cost == -1)
	{
	  printf("NA\n");
	}
      else{
      printf("最短経路:%.5f\n",nodes[dest].cost);
     
      for(l=0;dest!=dep;l++)
	{
	  point[l]=dest=nodes[dest].from;
	  point[l]++;
	}
      
      printf("経路:");
      for(j=l-1;j>=0;j--)
	{
	  if(point[j]>N)
	    {
	      point[j]-=N;
	      snprintf(temp,200,"%d",point[j]); //tempにpoint[j]の値を文字列として格納
	      strcat(cpoint,temp);
	    }
	  else snprintf(cpoint,200,"%d",point[j]); 
	  printf("%s-> ",cpoint);
	  strcpy(cpoint,C);
	}
      printf("%s\n",goal);
      }
    }
      return 0;
}

void get_path(node *nodes, int size, int dep)
{
  int i, to, from;
  double cost;
  node *done_node;

  // ノードを初期化
  for (i = 0; i < size; i++) {
    nodes[i].done = 0;
    nodes[i].cost = -1;
  }

  // 始点のコストはゼロ
  nodes[dep].cost = 0;

  while (1) {
    // 確定するノードを設定
    done_node = NULL;
    for (i = 0; i < size; i++)
      {
	if (nodes[i].done || nodes[i].cost < 0) {
	  continue;
	}
	else if (done_node == NULL || nodes[i].cost < done_node->cost) {
	  done_node = &nodes[i];
	  from = i;
	}
      }
    // 確定するノードがなくなったら終了
    if (done_node == NULL) break;
    
    // ノードを確定
    done_node->done = 1;

    // 接続先の情報を更新
    for (i = 0; i < done_node->n_edges; i++) {
      to = done_node->edges_to[i];
      cost = done_node->cost + done_node->edges_cost[i];
      if (nodes[to].cost < 0 || cost < nodes[to].cost) {
	nodes[to].cost = cost;
	nodes[to].from = from;
      }
    }
  }
}
