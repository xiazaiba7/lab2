#include <iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sstream>
#include <queue>
using namespace std;
int length=0; 
int num;
FILE *in,*out;
string letter[1000];
int shuzi[200];//用来储存数字 
char op[200];
int top1=-1,top2=-1;
int operate(char c)
{
	if(c=='+')
	shuzi[top1-1]=shuzi[top1]+shuzi[top1-1];
	if(c=='-')
	shuzi[top1-1]=shuzi[top1-1]-shuzi[top1];
	if(c=='*')
	shuzi[top1-1]=shuzi[top1-1]*shuzi[top1];
	if(c=='/')
	shuzi[top1-1]=shuzi[top1-1]/shuzi[top1];
	if(c=='%')
	shuzi[top1-1]=shuzi[top1-1]%shuzi[top1];
	top1--;
	return shuzi[top1-1]; 
}
int q[100];
int top=0;
int result= -1;
int MulExp();
int PrimaryExp();
int AddExp();
int UnaryExp();
int inputstack[100];
int opstack[100];
int symbol(string s)
{
	if(s=="(")
		return 3;
	else if(s==")")
		return 4;
	else if(s=="{")
		return 5;
	else if(s=="}")
		return 9;
	else if(s==";")
		return 8;
	else
		return 0;
} 
 int judgeword(string s,int n)
 {
 	int j=n+1;
 	while(1)
	{
		if((letter[j]>="a"&&letter[j]<="z"||letter[j]>="0"&&letter[j]<="9"||letter[j]>="A"&&letter[j]<="Z"||letter[j]=="_")&&letter[j]!="block")
		{
   			s=(s+letter[j]).c_str();
   			j++;
   		}
   		else
   		{
   			break;
		}
  	}
  	num=j;
 	if(s=="int")
 		return 1;
 	else if(s=="main")
 		return 2;
 	else if(s=="return")
 		return 6;
 	else
 		return 0;
 }
 bool isnonezero(string s)
 {
 	if(s>="1"&&s<="9")
 		return true;
	return false; 
 }
 bool isoctaldigit(string s)
 {
 	if(s>="0"&&s<="7")
 		return true;
 	else
 		return false;
 }
 bool ishexdigit(string s)
 {
 	if(s>="0"&&s<="9"||s>="a"&&s<="f"||s>="A"&&s<="F")
 		return true;
 	else
 		return false;
 }
int Number(string s,int n)
{
	stringstream ss1;
	int j=n+1;
	int flag=1;
	int ret;
	if(s=="0"&&letter[j]=="x"||s=="0"&&letter[j]=="X")//十六进制 
	{
		s=(s+letter[j]).c_str();
 		j++;
 		while(1)
 		{
 			if(ishexdigit(letter[j]))
 			{
 				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>hex>>ret;
				return ret;
			}
		}
	}
	else if(s=="0"&&letter[j]!="x"&&letter[j]!="X")//八进制 
	{
		while(1)
		{
			if(isoctaldigit(letter[j]))
			{
				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>oct>>ret;
				return ret;
			}
		}
	}
	else if(isnonezero(s))
	{
		while(1)
		{
			if(letter[j]>="0"&&letter[j]<="9")
			{
				s=(s+letter[j]).c_str();
   				j++;
			}
			else
			{
				num=j;
				ss1.str(s);
				ss1>>ret;
				return ret;
			}
		}
	}
 num=j;
 return -1;
}
int Exp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(AddExp()>0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
 } 
int AddExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(MulExp()>0)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		while(letter[num]=="+"||letter[num]=="-")
		{
			while(op[top2]!='('&&top2!=-1)
			{
				operate(op[top2]);
				top2--;
			}
			if(letter[num]=="+") 
				op[++top2]='+';
			else if(letter[num]=="-")
				op[++top2]='-';
			num++;
			while(letter[num]=="block")
			{
				num++;
			}
			if(MulExp()==0)
				return 0;
		}
		return 1;
	}
	else
		return 0;
}
int MulExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(UnaryExp()>0)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		while(letter[num]=="*"||letter[num]=="/"||letter[num]=="%")
		{
			if(op[top2]=='+'||op[top2]=='-'||op[top2]=='('||top2==-1)
			{
				if(letter[num]=="*")
					op[++top2]='*';
				else if(letter[num]=="/")
					op[++top2]='/';
				else if(letter[num]=="%")
					op[++top2]='%';				
			}
			else if(op[top2]=='*'||op[top2]=='%'||op[top2]=='/')
			{
				while(op[top2]=='*'||op[top2]=='%'||op[top2]=='/')
				{
					operate(op[top2]);
					top2--;
				}
				if(letter[num]=="*")
					op[++top2]='*';
				else if(letter[num]=="/")
					op[++top2]='/';
				else if(letter[num]=="%")
					op[++top2]='%';
			}
			num++;
			while(letter[num]=="block")
			{
				num++;
			}
			if(UnaryExp()==0)
				return 0;
		}
		return 1;
	}
	else
		return 0;
 } 
int PrimaryExp(int opt)
{
	while(letter[num]=="block")
	{
		num++;
	}
	if(letter[num]=="(")
	{
		op[++top2]='(';
		num++;
		while(letter[num]=="block")
		{
			num++;
		}
		if(Exp()>0)
		{
			while(letter[num]=="block")
			{
				num++;
			}
			if(letter[num]==")")
			{
				while(op[top2]!='(')
				{
					operate(op[top2]);
					top2--;
				}
				shuzi[top1]=shuzi[top1]*opt;
				top2--;
				num++;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		
	}
	else if(letter[num]>="0"&&letter[num]<="9")//number
	{
		int j=num;
		string s=letter[j];
		result=Number(s,j);
		shuzi[++top1]=opt*result;
		if(result!=-1)
		{
			return 2;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
int UnaryExp()
{
	while(letter[num]=="block")
	{
		num++;
	}
	int opt=1;
	while(letter[num]=="+"||letter[num]=="-")
	{
		if(letter[num]=="-")
		{
			opt = -opt;
		}
		num++;
		while(letter[num]=="block")
		{
			num++;
		}
	}
	if(PrimaryExp(opt)>0)
		return 1;
	else
		return 0;
}
int TakeWord()
{
	for(;num<length;)
	{
  		string str;
  		string strnew;
  		str=letter[num];
  		if((str>="a"&&str<="z"||str>="A"&&str<="Z"||str=="_")&&str!="block")
		{
			int x=judgeword(str,num);	
			if(x==1)
			{
				q[++top]=1;//int 
			}
			else if(x==2)
			{
				q[++top]=2;//main
			}
			else if(x==6)
			{
				q[++top]=6;//return
				if(Exp()>0)
				{
					q[++top] = 7;
					while(top2!=-1)
					{
						operate(op[top2]);
						top2--;
					}
				}
				else
				{
					printf("这里的问题\n");
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else if(str=="("||str==")"||str=="{"||str=="}"||str==";")
		{
			int x=symbol(str);
			if(x!=0)
			{
				q[++top]=x;
				num++;
			}
			else
			{
				return -2;
			}
		}
		else if(str>="0" && str<="9")
		{
			int ret=Number(str,num);
			if(ret==-1)
			{
				return -1;
			}
			else if(result == -1)
			{
				q[++top] = 7;
				result = ret;
			}
			else
			{
				return -3;//出现多个数字 
			}
		} 
		else if(str=="block")
		{
			num++;
		}
		else
		{
			printf("这里有错\n");
			return -4;
		} 
	}
	return 0;
}
int main(int argc,char **argv){
	char w;
 	int i,j;

	in = fopen(argv[1],"r");
	out = fopen(argv[2],"w");
 	int flag=0;
	char line[105];
	while(fgets(line,100,in)!=NULL)
 	{
 		int len=strlen(line);
	 	for(int k=0;line[k]>0;k++)
		{
			if(line[k]==47&&line[k+1]==42)//是/*形注释 
			{
				flag=1;
			} 
			if(flag==1)
			{
				if(k>0)
				{
					if(line[k-1]==42&&line[k]==47)
					{
						flag=0;
					}
				}
				continue;
			}
			if(k>=len)
			{
				break;
			 } 
			if(line[k]==47&&line[k+1]==47)//是//形注释 
			{
				break;
			}
			w=line[k];
  			if(!isblank(w)&&w!='\n'&&w!='\r')
			{
   				letter[length]=w;
   				length++;
  			} 
  			else if(isblank(w)||w=='\n'&&w!='\r')
  			{
  				letter[length]="block";
  				length++;
  				if(w=='\n')
  				{
  					break;
				}
  			}
  			else
  			{
  				break;
  			}
  		}
	}
	int te=TakeWord();
	if(te!=0)
	{
		printf("%d\n",te);
  		return 5;
  	}
	if(top!=9)
	{
		printf("4\n");
		return 4;
	}
	for(i=1;i<=top;i++)
	{
		if(q[i]!=i)
		{
			printf("3\n");
			return 3;
		}
	} 
	for(i=1;i<=top;i++)
	{
		if(q[i]==1)
		{
			fprintf(out,"define dso_local i32 ");
		}
		else if(q[i]==2)
		{
			fprintf(out,"@main");
		}
		else if(q[i]==3)
		{
			fprintf(out,"(");
		}
		else if(q[i]==4)
		{
			fprintf(out,")");
		}
		else if(q[i]==5)
		{
			fprintf(out,"{\n");
		}
		else if(q[i]==6)
		{
			fprintf(out,"    ret ");
		}
		else if(q[i]==7)
		{
			fprintf(out,"i32 %d\n",shuzi[0]);
		}
		else if(q[i]==9)
		{
			fprintf(out,"}");
		}
	}
	return 0;
}
