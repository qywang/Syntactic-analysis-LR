#include<iostream>
#include<fstream>
#include<stack>
#include<string>
using namespace std;

char G[10][10]={"S->E","E->E+T","E->E-T","E->T","T->T*F","T->T/F","T->F","F->(E)","F->i","F->n"};//存储文法中的产生式 
char VN[5]={'E','T','F'};        //存储非终结符 
char VT[10]={'i', 'n' , '+' , '-' , '*' , '/' , '(' , ')','$'};    //存储终结符 

struct action
{
	char ac;
	int  num;
};
action act[17][10];
int Ana_M[6][10];
stack <char> anl_buf;
int go_to[17][3];

/*构造预测分析表*/
void buildM()
{
	int i,j;
	act[0][0].ac=act[0][1].ac=act[0][6].ac=act[1][2].ac=act[1][3].ac=act[2][4].ac=act[2][5].ac=act[4][0].ac=act[4][1].ac=act[4][6].ac='s';
	act[11][2].ac=act[11][3].ac=act[11][7].ac=act[12][4].ac=act[12][5].ac=act[13][4].ac=act[13][5].ac='s';
	act[2][2].ac=act[2][3].ac=act[2][7].ac=act[2][8].ac=act[3][2].ac=act[3][3].ac=act[3][4].ac=act[3][5].ac=act[3][7].ac=act[3][8].ac='r';
	act[5][2].ac=act[5][3].ac=act[5][4].ac=act[5][5].ac=act[5][7].ac=act[5][8].ac=act[6][2].ac=act[6][3].ac=act[6][4].ac=act[6][5].ac=act[6][7].ac=act[6][8].ac='r';
	act[12][2].ac=act[12][3].ac=act[12][7].ac=act[12][8].ac=act[13][2].ac=act[13][3].ac=act[13][7].ac=act[13][8].ac='r';
	act[12][2].num=act[12][3].num=act[12][7].num=act[12][8].num=1;
	act[13][2].num=act[13][3].num=act[13][7].num=act[13][8].num=2;
	act[2][2].num=act[2][3].num=act[2][7].num=act[2][8].num=3;
	act[0][0].num=act[4][0].num=act[3][2].num=act[3][3].num=act[3][4].num=act[3][5].num=act[3][7].num=act[3][8].num=6;
	act[0][1].num=act[4][1].num=act[15][2].num=act[15][3].num=act[15][4].num=act[15][5].num=act[15][7].num=act[15][8].num=5;
	act[0][6].num=act[4][6].num=act[14][2].num=act[14][3].num=act[14][4].num=act[14][5].num=act[14][7].num=act[14][8].num=4;
	act[1][2].num=act[11][2].num=act[16][2].num=act[16][3].num=act[16][4].num=act[16][5].num=act[16][7].num=act[16][8].num=7;
	act[1][3].num=act[11][3].num=act[6][2].num=act[6][3].num=act[6][4].num=act[6][5].num=act[6][7].num=act[6][8].num=8;
	act[2][4].num=act[12][4].num=act[13][4].num=act[5][2].num=act[5][3].num=act[5][4].num=act[5][5].num=act[5][7].num=act[5][8].num=9;
	act[2][5].num=act[12][5].num=act[13][5].num=10;
	act[1][8].ac='a';
	go_to[0][0]=1;
	go_to[0][1]=go_to[4][1]=2;
	go_to[0][2]=go_to[4][2]=go_to[7][2]=go_to[8][2]=3;
	go_to[4][0]=11;
	go_to[7][1]=12;
	go_to[8][1]=13;
	go_to[9][2]=14;
	go_to[10][2]=15;
	for(i=7;i<=10;i++)
	{
		act[i][0].ac='s';
		act[i][0].num=6;
		act[i][1].ac='s';
		act[i][1].num=5;
		act[i][6].ac='s';
		act[i][6].num=4;
	}
	for(i=14;i<=16;i++)
	{
		act[i][2].ac='r';
		act[i][3].ac='r';
		act[i][4].ac='r';
		act[i][5].ac='r';
		act[i][7].ac='r';
		act[i][8].ac='r';
	}
	
	printf("\n表达式文法的LR预测分析表如下：\n\n");
	printf("                   action                                             goto   \n ");
	cout<<"_____________________________________________________________________________"<<endl;
    for(i=0;i<10;i++)
		printf("%6c",VT[i]);
	for(i=0;i<3;i++)
		printf("%5c",VN[i]);
    printf("\n");
	cout<<"______________________________________________________________________________"<<endl;
    for(i=0;i<17;i++)
    {  
		printf("%-5d",i);
        for(j=0;j<9;j++)
		{
			if(act[i][j].ac!='\0')
		{
					printf("%c",act[i][j].ac);
					printf("%-5d",act[i][j].num);
		}
			else
				printf("      ");
		}
		printf("  ");
		for(j=0;j<=2;j++)
		{
			if(go_to[i][j]!=0)
				printf("%5d",go_to[i][j]);
			else
				printf("      ");
		}
        printf("\n");
	}
}

/*分析过程*/
void Analyse(char input[],char ch,int num)
{
	int i,j,k,n=0;
	int flag=1;
	char output[50];
	char S1_temp;
	int S2_temp;

	while(flag)
	{
		cout<<endl;
		printf("%6c       ",anl_buf.top());
		i=0;
		for(j=num;input[j]!='\0';j++)
			output[i++]=input[j];
		output[i++]='\0';
		printf("%20s",output);
		
		if(anl_buf.top()=='$')
			break;
		
		for(i=0;i<10;i++)
			if(ch==VT[i])
			{
				if(act[anl_buf.top()-48][i].ac=='s')//状态转移
				{
					//cout<<act[anl_buf.top()-48][i].num;
					S2_temp=act[anl_buf.top()-48][i].num;
					printf("                     shift %d",S2_temp);
					anl_buf.push(ch);
					anl_buf.push(S2_temp+48);
					flag=0;//IP前移
					break;
				}
				else if(flag&&act[anl_buf.top()-48][i].ac=='r')//规约
				{
					n=0;
					printf("                     reduce by %s",G[act[anl_buf.top()-48][i].num]);//打印所用的规约语句
					S1_temp=G[act[anl_buf.top()-48][i].num][0];//产生式左侧非终结符

					while(G[act[anl_buf.top()-48][i].num][n]!='\0')//统计产生式右侧长度
					{	n++;	}
					for(j=0;j<2*(n-3);j++)
						anl_buf.pop();
					for(k=0;k<=2;k++)
						if(VN[k]==S1_temp)
							S2_temp=go_to[anl_buf.top()-48][k];
					anl_buf.push(S1_temp);
					anl_buf.push(S2_temp+48);
				}
				else if(flag&&act[anl_buf.top()-48][i].ac=='a')
				{
					printf("                     accept\n");
					flag=0;
					break;
				}
				else	if(flag)
				{
					flag=0;
					printf("\n表达式错误!");
					break;
				}
				
			}
			
	}
}

int main()
{
	int i;
	char input[50];
    cout<<"该文法的产生式如下："<<endl;
    for(i=0;i<11;i++)
		cout<<("   %s\n",G[i])<<endl;
	cout<<"____________________________________________________________________________\n"<<endl;
	/*cout<<"\n该文法的First和Follow集如下："<<endl;
	cout<<"                First                        Follow               "<<endl;
	cout<<"____________________________________________________________________________\n"<<endl;
    for(i=0;i<5;i++)
	{
		cout<<("%c",VN[i]);
		cout<<":              ";
		for(j=0;j<4;j++)
			printf("%c ",First[i][j]);
		printf("                      ");
		for(j=0;j<7;j++)
			printf("%c ",Follow[i][j]);
		cout<<endl;
	}
    cout<<"____________________________________________________________________________\n"<<endl;
	
	cout<<"文法预测分析表如下："<<endl;
	cout<<"____________________________________________________________________________\n";*/
	buildM();
	/*for(i=0;i<10;i++)
		printf("%8c",VT[i]);
	printf("\n");
	for(i=0;i<5;i++)
	{
		printf("%c:",VN[i]);
		for(j=0;j<9;j++)
			printf("%8s",G[Ana_M[i][j]-1]);
		cout<<endl;
	}
	cout<<"____________________________________________________________________________\n"<<endl;*/
	 
	cout<<"请输入表达式："<<endl;
	cin>>input;
	int len=strlen(input);
	input[len]='$';
	input[len+1]='\0';
	anl_buf.push('$');
	anl_buf.push('0');
	printf("   栈顶元素               剩余输入串                   输出\n");
	cout<<"____________________________________________________________________________\n";
	for(i=0;i<=len;i++)
		Analyse(input,input[i],i);
	
	system("pause");
	return 0;

}