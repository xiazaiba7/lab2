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
	{
		fprintf(out,"(");
		num++;
		return 3;
	}
	else if(s==")")
	{	
		fprintf(out,")");
		num++;
		return 4;
	}	
	else if(s=="{")
	{
		fprintf(out,"{\n");
		num++;
		return 5;
	}
	else if(s=="}")
	{
		fprintf(out,"}");
		num++;
		return 9;
	}
	else if(s==";")
	{
		num++;
		return 8;
	}
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
 	{
 		fprintf(out,"define dso_local i32 ");
 		return 1;
 	}
 	else if(s=="main")
 	{
 		fprintf(out,"@main");
 		return 2;
 	}
 	else if(s=="return")
 	{
	 	fprintf(out,"    ret ");
 		return 6;
 	}
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
 	if((s>="0"&&s<="9"||s>="a"&&s<="f"||s>="A"&&s<="F")&&s!="block")
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
	if((s=="0"&&letter[j]=="x"||s=="0"&&letter[j]=="X")&&letter[j]!="block")//十六进制 
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
			while(letter[num]=="block")
			{
				num++;
			}
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

int FuncDef()
{
	while(letter[num]=="block")
	{
		num++;
	}
	string s=letter[num];
	if(judgeword(s,num)==1)
	{
		while(letter[num]=="block")
		{
			num++;
		}
		if(judgeword(letter[num],num)==2)
		{
			while(letter[num]=="block")
				num++;
			if(symbol(letter[num])==3)
			{
				while(letter[num]=="block")
					num++;
				if(symbol(letter[num])==4)
				{
					while(letter[num]=="block")
						num++;
					if(symbol(letter[num])==5)
					{
						while(letter[num]=="block")
							num++;
						if(judgeword(letter[num],num)==6)//return
						{
							if(Exp()>0)
							{
								while(top2!=-1)
								{
									operate(op[top2]);
									top2--;
								}
								fprintf(out,"i32 %d\n",shuzi[0]);
								while(letter[num]=="block")
									num++;
								if(symbol(letter[num])==8)
								{
									while(letter[num]=="block")
										num++;
									if(symbol(letter[num])==9)
									{
										return 1;
									}
								}
							}
						}
					}
				}
			}	
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
	int fd=FuncDef();
	if(fd>0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
