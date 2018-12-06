#include <stdio.h>
#define N 5
struct Point							//ban point struct
{
	int num;
	int x;
	int y;
};
int p[N+1] = {0};						//temporary solution from back tracking method
int solve[1000][N+1] = {0};				//solve[0][0]number of independent solutions. solve[1]the first independent solution
int add[3][2*N+1] = {0};	//optimization space: add[0][i]ensure queens not in the same column	add[1][i] for forwardslash	add[2][i] for backslash
int pos[9][N+1] = {0};					//the 8 transformations of a solution, 4(rotation)*2(fold) 
int ans[10000][9][N] = {0};				//ans[i][][]shows the repeated solutions of the i independent solutions
struct Point solve_banpoints[10000][10000] = {0};		//ban point plan
struct Point ban_temp[1000];							//ban point plan for temporary 
int num_pos[10000][9] = {0};						//existing independent solutions's transformations considering the banpoints plan

void BackTrace(int);			//back tracking method to find solutions of the queen problem including repeated solutions
void Save_Solve();				//save solutions from p[i] to solve[solve[0][0]][i] as the solve[0][0](number of solutions) solution
void Out_Solve();				//output the solutions in a text file
void Rotate_Fold();				//rotate and fold transformations from p[] to pos[][]
void Rotate90(int);				//rotate 90 degree ->pos[][]
int Norepeat_Solve();			//compare the new solution's transformation pos[][] and existing independent solution solve[][], if norepeat return 1
void Load_Ans();				//load the transformations of independent solutions respectively to ans[i][][] 
int Norepeat_Ans(int,int);		//compare pos[j][] and ans[i][][], if norepeat return 1
void Out_Ans();									//output ans[][][] in a text file
void Out_Banpoints();						//output banpoints plan in a text file
void Search(int);							//back tracking method to find the ban points,which could exclude all the repeated solutions
void Load_Numpos();								//save num_pos[][], 
int Complete();									//if 1 ,means that all repeated solutions are excluded by ban points
int Norepeat_Banpoints();						//ensure ban_temp[] is not repeated in solve_banpoints[][], if ban_temp[] contain solve_banpoints[][] return 0 
int Contain_str(struct Point *,struct Point *);		//str2(Point type) contains str1 return 1, else return 0
void Save_Solve_Banpoints(int);			//save the ban point plan from ban_temp[] to solve_banpoints[][], ban_temp[0].num stands for the points number in the plan
int Test_Ban(int,int,int);				//ensure that new ban point would not ban all the repeated solutions in a independent solution
void Load_Ban(int,int,int);				//load the ban point to ban_temp[]
void Delete_Numpos(int m,int n);		//delete num_pos[m][n], move later ones forward one position, num_pos[m][n] to num_pos[m][8]
void Back();							//recover ban_temp,find ban_temp[ban_temp[0].num].x,ban_temp[ban_temp[0].num].y
int No_Other_Ban(int);					//ensure that the deleted solution is not banned by other ban points
void Add_Numpos(int);					//the averse process of delete, num_pos[i][8]->num_pos[i][1],others move backward one position
void Delete_Solve_Ban(int);				//delete this ban point plan and move the later ones forward one position 
void Out_Opt();							//sort the ban point plans with respect to the number of banpoints in the plans, then output them

main()
{
	BackTrace(1);
	Out_Solve();
	Load_Ans();
	Load_Numpos();
	Out_Ans();
	Search(1);
	Out_Banpoints();
	Out_Opt();
}

void Search(int point)									//back tracking method to find the ban points,which could exclude all the repeated solutions
{
	int i,j,k;
	if(Complete()&&Norepeat_Banpoints())				//complete a banpoint plan which is not repeated in solve_banpoints[][]
	{
		solve_banpoints[0][0].num++;
		Save_Solve_Banpoints(solve_banpoints[0][0].num);
		Out_Banpoints();
		Out_Opt();
	}
	else
	{
		for(k=1;k<=N;k++)							//i,j,k loop the queen points in the repeated solutions
		{
			for(i=1;i<=solve[0][0];i++)
			{
				if(num_pos[i][0]==1)
				{
					continue;
				}
				for(j=1;j<=num_pos[i][0];j++)		
				{
					if(Test_Ban(i,j,k))				//test ans[i][j][k]
					{
						Load_Ban(i,j,k);
						Search(point+1);
						Back();}}}}}}					//return back,delete the point and recover num_pos[][]

void BackTrace(int step){				//back tracking method to find solutions of the queen problem including repeated solutions
	int i;
	if(step>N){
		Rotate_Fold();
		if(Norepeat_Solve()){
			Save_Solve();}} 
	for(i=1;i<=N;i++){
		if(add[0][i]==0&&add[1][i+step]==0&&add[2][N+step-i]==0){
			add[0][i] = 1;
			add[1][i+step] = 1;
			add[2][N+step-i] = 1;
			p[step] = i;
			BackTrace(step+1);
			add[0][i] = 0;
			add[1][i+step] = 0;
			add[2][N+step-i] = 0;}}}

int Norepeat_Ans(int i,int j)			//compare pos[j][] and ans[i][][], if norepeat return 1
{
	int m,n,flag;
	for(m=1;m<=ans[i][0][0];m++)
	{
		flag = 1;
		for(n=1;n<=N;n++)
		{
			if(pos[j][n]!=ans[i][m][n])
			{
				flag = 0;
				break;
			}
		}
		if(flag==1)
		{
			return 0;
		}
	}
	return 1;
}

int Norepeat_Solve()					//compare the new solution's transformation pos[][] and existing independent solution solve[][], if norepeat return 1
{
	int i,j,k,flag;
	for(i=1;i<=solve[0][0];i++)
	{
		for(j=1;j<=8;j++)
		{
			flag = 1;
			for(k=1;k<=N;k++)
			{
				if(solve[i][k]!=pos[j][k])
				{
					flag = 0;
					break;
				}
			}
			if(flag==1)
			{
				return 0;
			}
		}
	}
	return 1;
} 

void Rotate90(int i)					//rotate 90 degree ->pos[][]
{
	int j;
	for(j=1;j<=N;j++)
	{
		pos[i+1][pos[i][j]] = N + 1 - j;
	}
}

void Rotate_Fold()						//rotate and fold transformations from p[] to pos[][]
{
	int i,j;
	for(i=1;i<=N;i++)
	{
		pos[1][i] = p[i];
	}
	for(j=1;j<4;j++)
	{
		Rotate90(j);
	}
	for(i=1;i<=N;i++)
	{
		pos[5][i] = N + 1 - p[i];
	}
	for(j=5;j<8;j++)
	{
		Rotate90(j);
	}
}

void Out_Solve()						//output the solutions in a text file
{
	int i,j,k;
	FILE *fp;
	fp = fopen("queen.txt","w");
	fprintf(fp,"%d queen independent solutions��\n",N);
	for(i=1;i<=solve[0][0];i++)
	{
		fprintf(fp,"The %d independent solution: \n",i);
		for(j=1;j<=N;j++)
		{
			for(k=1;k<=N;k++)
			{
				if(k==solve[i][j])
				{
					fprintf(fp,"X");
				}
				else
				{
					fprintf(fp,"*");
				}
			}
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
}



void Save_Solve()						//save solutions from p[i] to solve[solve[0][0]][i] as the solve[0][0](number of solutions) solution
{
	int i;
	solve[0][0]++;
	for(i=1;i<=N;i++)
	{
		solve[solve[0][0]][i] = p[i];
	}
}
void Load_Numpos()						//save num_pos[][], 
{
	int i,j;
	num_pos[0][0] = solve[0][0];
	for(i=1;i<=solve[0][0];i++)
	{
		num_pos[i][0] = ans[i][0][0];
		for(j=1;j<=ans[i][0][0];j++)
		{
			num_pos[i][j] = j;
		}
	}
}

void Out_Ans()							//output ans[][][] in a text file
{
	int i,j,k;
	FILE *fp;
	fp = fopen("queen.txt","a");
	for(i=1;i<=solve[0][0];i++)
	{
		fprintf(fp,"===================\n");
		for(j=1;j<=ans[i][0][0];j++)
		{
			for(k=1;k<=N;k++)
			{
				fprintf(fp,"%4d",ans[i][j][k]);
			}
			fprintf(fp,"\n");
		}
	}
	fclose(fp);	
}

void Load_Ans()							//load the transformations of independent solutions respectively to ans[i][][] 
{
	int i,j,k,m;
	for(i=1;i<=solve[0][0];i++)
	{
		for(m=1;m<=N;m++)
		{
			p[m] = solve[i][m];
		}
		Rotate_Fold();
		ans[i][0][0] = 0;
		for(j=1;j<=8;j++)
		{
			if(pos[j][1]>0&&Norepeat_Ans(i,j))
//			if(pos[j][1]>0&&pos[j][1]<=(N+1)/2&&Norepeat_Ans(i,j))		//exclude some fold repeated solutions by limit queen position from 1-N to 1-N+1/2
			{
				ans[i][0][0]++;
				for(k=1;k<=N;k++)
				{
					ans[i][ans[i][0][0]][k] = pos[j][k];
				}
			}
		}
	}
}

int Complete()												//if 1 ,means that all repeated solutions are excluded by ban points
{
	int i;
	for(i=1;i<=solve[0][0];i++)
	{
		if(num_pos[i][0]>1)
		{
			return 0;
		}
	}
	return 1;
}

int Contain_str(struct Point *str1,struct Point *str2)		//str2(Point type) contains str1 return 1, else return 0
{
	int i,m,flag;
	for(i=1;i<=str1[0].num;i++)
	{
		flag = 0;
		for(m=1;m<=str2[0].num;m++)
		{
			if(str1[i].x==str2[m].x&&str1[i].y==str2[m].y)
			{
				flag = 1;
				break;
			}
		}
		if(flag==0)
		{
			return 0;
		}
	}
	return 1;
}

int Norepeat_Banpoints()									//ensure ban_temp[] is not repeated in solve_banpoints[][], if ban_temp[] contain solve_banpoints[][] return 0 
{
	int i,j;
	int flag = 1;											//flag stands for no solve_banpoints[][] is swaped or no solve_banpoints[][] is contained in ban_temp[]
	for(i=1;i<=solve_banpoints[0][0].num;i++)
	{
		if(Contain_str(ban_temp,solve_banpoints[i])&&ban_temp[0].num<solve_banpoints[i][0].num)		//check if ban_temp[] contained by solve_banpoints[i][]
		{
			if(flag==0)
			{
				Save_Solve_Banpoints(i);															//swap ban_temp[] with solve_banpoints[i][]
				flag = 0;
			}
			else
			{
				Delete_Solve_Ban(i);
				i--;
				solve_banpoints[0][0].num--;
			}
		}
		else if(Contain_str(solve_banpoints[i],ban_temp))
		{
			flag = 0;
		}
	}
	return flag;
}

void Save_Solve_Banpoints(int i)			//save the ban point plan from ban_temp[] to solve_banpoints[][], ban_temp[0].num stands for the points number in the plan 
{
	int j;
	solve_banpoints[i][0].num = ban_temp[0].num;
	for(j=1;j<=ban_temp[0].num;j++)
	{
		solve_banpoints[i][j].x = ban_temp[j].x;
		solve_banpoints[i][j].y = ban_temp[j].y;
	}
}



void Out_Banpoints()							//output banpoints plan in a text file
{
	int i,j,k;
	FILE *fp;
	fp = fopen("queen_banpoints.txt","w");
	for(i=1;i<=solve_banpoints[0][0].num;i++)
	{
		fprintf(fp,"The %d ban point plan: \n",i);
		for(j=1;j<=solve_banpoints[i][0].num;j++)
		{
			fprintf(fp,"%d,%d\n",solve_banpoints[i][j].x,solve_banpoints[i][j].y);
		}
	}
	fclose(fp);
}

int Test_Ban(int i,int j,int k)								//test ans[i][j][k],ban points(x,y)= (k, ans[i][num_pos[i][j]][k])
{															//ensure that new ban point would not ban all the repeated solutions in a independent solution
	int m,n,t,flag;											//flag stands for the number of solutions that banned by the new ban point in each independent solution
	for(m=1;m<=solve[0][0];m++)
	{
		flag = 0;
		for(n=1;n<=num_pos[m][0];n++)
		{
			if(ans[m][num_pos[m][n]][k]==ans[i][num_pos[i][j]][k])
			{
				flag++;
			}
		}
		if((num_pos[m][0]-flag)==0)
		{
			return 0;
		}
	}
	return 1;
}

void Load_Ban(int i,int j,int k)							//load the ban point to ban_temp[]
{
	int m,n;
	ban_temp[0].num++;
	ban_temp[ban_temp[0].num].x = k;
	ban_temp[ban_temp[0].num].y = ans[i][num_pos[i][j]][k];
	for(m=1;m<=solve[0][0];m++)	
	{
		for(n=1;n<=num_pos[m][0];n++)
		{
			if(ban_temp[ban_temp[0].num].y==ans[m][num_pos[m][n]][k])		//delete the repeated solutions banned by the ban point
			{
				Delete_Numpos(m,n);
				n--;
				num_pos[m][0]--;
			}
		}
	}
}

void Delete_Numpos(int m,int n)								//delete num_pos[m][n], move later ones forward one position, num_pos[m][n] to num_pos[m][8]
{															//to recover
	int i,temp;
	temp = num_pos[m][n];
	for(i=n;i<8;i++)
	{
		num_pos[m][i] = num_pos[m][i+1];
	}
	num_pos[m][8] = temp;
}

void Back()												//recover ban_temp,find ban_temp[ban_temp[0].num].x,ban_temp[ban_temp[0].num].y
{
	int i,j;
	for(i=1;i<=solve[0][0];i++)							//see ans[i][num_pos[i][8]][k]
	{
		if(ans[i][num_pos[i][8]][ban_temp[ban_temp[0].num].x]==ban_temp[ban_temp[0].num].y)
		{
			if(No_Other_Ban(i))				//not banned by other ban_temp[]
			{
				Add_Numpos(i);				
				i--;
			}
		}
	}
	ban_temp[0].num--;
}

void Add_Numpos(int i)						//the averse process of delete, num_pos[i][8]->num_pos[i][1],others move backward one position
{
	int j,temp;
	temp = num_pos[i][8];
	for(j=8;j>1;j--)
	{
		num_pos[i][j] = num_pos[i][j-1];
	}
	num_pos[i][1] = temp;
	num_pos[i][0]++;
}

int No_Other_Ban(int i)					//ensure that the deleted solution is not banned by other ban points
{
	int j;
	for(j=1;j<ban_temp[0].num;j++)			//ban_temp[j].x ban_temp[j].y
	{
		if(ans[i][num_pos[i][8]][ban_temp[j].x]==ban_temp[j].y)
		{
			return 0;
		}
	}
	return 1;
}

void Delete_Solve_Ban(int j)					//delete this ban point plan and move the later ones forward one position 
{
	int m,n;
	for(m=j;m<solve_banpoints[0][0].num;m++)
	{
		solve_banpoints[m][0].num = solve_banpoints[m+1][0].num;
		for(n=1;n<=solve_banpoints[m][0].num;n++)
		{
			solve_banpoints[m][n].x = solve_banpoints[m+1][n].x;
			solve_banpoints[m][n].y = solve_banpoints[m+1][n].y;
		}
	}
}

void Out_Opt()									//sort the ban point plans with respect to the number of banpoints in the plans, then output them
{
	int i,j,num = 0,k;
	FILE *fp;
	fp = fopen("queen_opt_ban.txt","w");
	int a1,a2;
	a1 = solve_banpoints[1][0].num;
	a2 = solve_banpoints[1][0].num;
	for(i=1;i<=solve_banpoints[0][0].num;i++)
	{
		if(solve_banpoints[i][0].num<a1)
		{
			a1 = solve_banpoints[i][0].num;
		}
		if(solve_banpoints[i][0].num>a2)
		{
			a2 = solve_banpoints[i][0].num;
		}
	}
	for(k=a1;k<=a2;k++)
	{
		for(i=1;i<=solve_banpoints[0][0].num;i++)
		{
			if(solve_banpoints[i][0].num==k)
			{
				num++;
				fprintf(fp,"The %d ban point plan: \n",num);
				for(j=1;j<=k;j++)
				{
					fprintf(fp,"%d,%d\n",solve_banpoints[i][j].x,solve_banpoints[i][j].y);
				}
			}
		}	
	}
	fclose(fp);
}





