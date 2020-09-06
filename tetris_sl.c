#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>


#define SPACE 32	//////////////////////////////////
#define LEFT  75	//////////////////////////////////
#define RIGHT 77	//////////检测按键值
#define DOWN 80	    //////////
#define ESC 27	    //////////////////////////////////
#define ENTER 13	//////////////////////////////////

#define Wall 2	    //纵向边界值
#define Box 1	    //横向边界值
#define Kong 0	    //置空
#define FACE_Y 30	//界面尺寸垂直尺寸
#define FACE_X 20   //界面尺寸水平尺寸


void gotoxy(int x,int y);  	//移动光标
int color(int c);           //颜色
void hidden_cursor();  	    //隐藏光标
void init_interface();  	//初始化界面
void init_dia();   		    //初始化方块信息
void draw_dia(int base,int space_c,int x,int y);   	//画方块
void draw_kong(int base,int space_c,int x,int y); 	//覆盖方块
int hit_bottom(int n,int space_c,int x,int y);  	//判断是否到底
void start_game();   		//开始游戏
//void vdrop();
int gameover();				//检测是否游戏结束 
int eliminate();		    //消除
void read_file();   		//读写最高记录
void write_file();  		//写最高纪录

int grade=0;		        //当前分数
int grade_max=0;		    //最高记录
int rand_n=0;


struct Interface
{
    int data[FACE_X+10][FACE_Y];    //数值，为1是方块，为0是空格
    int color[FACE_X+10][FACE_Y];   //对应方块的颜色
}face;

typedef struct Diamonds
{   int space[4][4];        	    //4*4矩阵，为1为方块，为0 为空
}Dia;
Dia dia[7][4];  		            //一维基础7个方块，二维表示旋转次数


int main(void)
{   
    system("cls");
    system("title 俄罗斯方块");
    color(7);
    system("mode con cols=60 lines=30");  //窗口宽度高度,根据FACE_X FACE_Y的值设定， （FACE_X+10）*2   FACE_Y 
    hidden_cursor();
    srand(time(NULL));
    read_file();
    grade=0;
    init_interface();
    init_dia();
    rand_n=rand()%7;
    while(1)
    {
    	
    	start_game();
    	
	}
    
    
    //初始化界面
	// 
    return 0;
}
/////////////////////////////////////////////////



void start_game()
{  
	int n,ch,t=0,y=0,x=FACE_X/2-2,i,j;
    int space_c=0;//旋转次数
    draw_kong(rand_n,space_c,FACE_X+2,4);
    n=rand_n;   
    rand_n=rand()%7;    //随机生成下一块
    color(rand_n);
    draw_dia(rand_n,space_c,FACE_X+2,4);
    while(1)
    {   
        color(n);
        draw_dia(n,space_c,x,y);//画出图形
        if(t==0)
        {
        	t=15000;
		}
        while(--t)
        {   if(kbhit()!=0)//有输入就跳出
                break;  
        }
        if(t==0)
        {
            if(hit_bottom(n,space_c,x,y+1)==1)
            {   draw_kong(n,space_c,x,y);
                y++;            //向下降落
            }
            else
            {
                for(j=0;j<4;j++)
                {
                    for(i=0;i<4;i++)
                    {
                        if(dia[n][space_c].space[i][j]==1)
                        {   
                            face.data[x+i][y+j]=Box;
                            face.color[x+i][y+j]=n;
                            while(eliminate())
							{
								;
							}
							gameover();
                        }
                    }
                }
                return;
            }
        }
        else
        {
            ch=getch();
            switch(ch)     //移动
            {
            case LEFT: if(hit_bottom(n,space_c,x-1,y)==1)   //判断是否可以移动
                       {   draw_kong(n,space_c,x,y);
                           x--;
                       }
                       break;
            case RIGHT: if(hit_bottom(n,space_c,x+1,y)==1)
                        {   draw_kong(n,space_c,x,y);
                            x++;
                        }
                        break;
            case DOWN: if(hit_bottom(n,space_c,x,y+1)==1)
                        {   draw_kong(n,space_c,x,y);
                            y++;
                        }
                        break;
            case SPACE: if(hit_bottom(n,(space_c+1)%4,x,y)==1)
                        {   draw_kong(n,space_c,x,y);
                            space_c=(space_c+1)%4;
                        }
                        break;
            case ESC  : system("cls");
                        gotoxy(FACE_X/2+12,FACE_Y/2-4);
                        printf("---游戏结束!---\n\n");
                        gotoxy(FACE_X/2+10,FACE_Y/2-2);
                        printf("---按任意键退出!---\n");
                        getch();
                        exit(0);
                        break;
            case  'R':
            case  'r':  main();
                        exit(0);
            case  'S':
            case  's':  while(1)
                        {   if(kbhit()!=0)//有输入就跳出
                            break;  
                        }
                        break;
            }
        }
    }
}


void init_interface()//界面
{   int i,j;
    for(i=0;i<FACE_Y;i++)
    {   for(j=0;j<FACE_X+10;j++)
        {   if(j==0 || j==FACE_X-1 || j==FACE_X+9)
            {   face.data[j][i]=Wall;
                gotoxy(2*j,i);
                printf("■");
            }
            else if(i==FACE_Y-1)
            {   face.data[j][i]=Box;
                gotoxy(2*j,i);
                printf("■");
            }
            else
            face.data[j][i]=Kong;
        }
    }
    gotoxy(2*FACE_X+2,FACE_Y-18);
    printf("左移：←");

    gotoxy(2*FACE_X+2,FACE_Y-16);
    printf("右移：→");

    gotoxy(2*FACE_X+2,FACE_Y-14);
    printf("旋转：space");

    gotoxy(2*FACE_X+2,FACE_Y-12);
    printf("暂停: S");

    gotoxy(2*FACE_X+2,FACE_Y-10);
    printf("退出: ESC");

    gotoxy(2*FACE_X+2,FACE_Y-8);
    printf("重新开始:R");

    gotoxy(2*FACE_X+2,FACE_Y-6);
    printf("最高纪录:%d",grade_max);

    gotoxy(2*FACE_X+2,FACE_Y-4);
    printf("分数：%d",grade);

}





void gotoxy(int x,int y) //移动坐标
{
    COORD coord;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
}


int color(int c)
{
    switch(c)
    {
    case 0: c=9;break;
    case 1:
    case 2: c=12;break;
    case 3:
    case 4: c=14;break;
    case 5: c=10;break;
    case 6: c=13;break;
    default: c=7;break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
    return 0;
}


void hidden_cursor()//隐藏光标
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(hOut,&cci);
  cci.bVisible=0;//赋1为显示，赋0为隐藏
  SetConsoleCursorInfo(hOut,&cci);
}


void init_dia()
{
    int i,j,k,z;
    int tmp[4][4];
    for(i=0;i<3;i++)
        dia[0][0].space[i][1]=1;
    dia[0][0].space[1][2]=1;     //土形

    for(i=1;i<4;i++)
        dia[1][0].space[1][i]=1;
    dia[1][0].space[2][1]=1;    //L形--1

    for(i=1;i<4;i++)
        dia[2][0].space[2][i]=1;
    dia[2][0].space[1][1]=1;    //L形--2

    for(i=0;i<2;i++)
    {   dia[3][0].space[i][1]=1;
        dia[3][0].space[i+1][2]=1; //Z形--1

        dia[4][0].space[i+1][1]=1;
        dia[4][0].space[i][2]=1;//Z形--2

        dia[5][0].space[1][i+1]=1;
        dia[5][0].space[2][i+1]=1;//田字形
    }
    for(i=0;i<4;i++)
        dia[6][0].space[2][i]=1;//1形
    //////////////////////////////////基础7个形状
    for(i=0;i<7;i++)
    {
        for(z=0;z<3;z++)
        {
            for(j=0;j<4;j++)
            {
                for(k=0;k<4;k++)
                {
                  tmp[j][k]=dia[i][z].space[j][k];
                }   
            }
            for(j=0;j<4;j++)
            {
                for(k=0;k<4;k++)
                {
                  dia[i][z+1].space[j][k]=tmp[4-k-1][j];
                }   
            }
        }
    }
    ///////////////////////////////////旋转后的21个形状
}
//////////////////////////////////////



void draw_dia(int base,int space_c,int x,int y)
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            gotoxy(2*(x+i),y+j);
            if(dia[base][space_c].space[i][j]==1)
            printf("■");
        }
    }
}


void draw_kong(int base,int space_c,int x,int y)
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            gotoxy(2*(x+i),y+j);
            if(dia[base][space_c].space[i][j]==1)
            printf("  ");
        }
    }

}



int hit_bottom(int n,int space_c,int x,int y)   //判断是否到底
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
           if(dia[n][space_c].space[i][j]==0)
               continue;
           else if(face.data[x+i][y+j]==Wall || face.data[x+i][y+j]==Box)
               return 0;
        }
    }
    return 1;
}


void read_file()    //读取最高记录
{
    FILE *fp;
    fp=fopen("俄罗斯方块记录.txt","r+");
    if(fp==NULL)
    {
        fp=fopen("俄罗斯方块记录.txt","w+");
        fwrite(&grade_max,sizeof(int),1,fp);
    }
    fseek(fp,0,0);
    fread(&grade_max,sizeof(int),1,fp);
    fclose(fp);
}
void write_file()   //保存最高记录
{
    FILE *fp;
    fp=fopen("俄罗斯方块记录.txt","r+");
    fwrite(&grade,sizeof(int),1,fp);
    fclose(fp);
}


int eliminate()
{
    int i,j,k,sum;
    for(j=FACE_Y-2;j>4;j--)
    {
        sum=0;
        for(i=1;i<FACE_X-1;i++)
        {
            sum+=face.data[i][j];   
        }
        if(sum==0)
        	 break;
        if(sum==FACE_X-2)   //满一行，减掉
        {   
            grade+=100;
            color(7);
            gotoxy(2*FACE_X+2,FACE_Y-4);
            printf("分数：%d",grade);
            for(i=1;i<FACE_X-1;i++)
            {
                face.data[i][j]=Kong;
                gotoxy(i*2,j);
                printf("  ");
            }
            for(i=j;i>1;i--)
            {
            	sum=0;
                for(k=1;k<FACE_X-1;k++)
                {
                	sum+=face.data[k][i-1]+face.data[k][i];
                    face.data[k][i]=face.data[k][i-1];
                    if(face.data[k][i]==Kong)
                    {
                        gotoxy(2*k,i);
                        printf("  ");
                    }
                    else
                    {
                        gotoxy(2*k,i);
                        color(face.color[k][i-1]);
                        printf("■");
                    }
                }
                
                if(sum==0)
                { 
                    return 1;
               	} 
            }

        }
    }

    return 0;
}
///////////////////////////////////


int gameover()
{
	int i;
	
    for(i=1;i<FACE_Y-1;i++)
    {
        if(face.data[i][1]==Box)
        {
            char n;
            Sleep(2000); //延时
            system("cls");
            color(7);
            gotoxy(2*(FACE_X/2-2),FACE_Y/3);
            if(grade>grade_max)
            {
                printf("恭喜您打破记录，目前最高纪录为：%d",grade);
                write_file();
            }
            else if(grade==grade_max)
                printf("与纪录持平，请突破你的极限！");
            else
                printf("请继续努力，你与最高记录只差：%d",grade_max-grade);
            gotoxy(2*(FACE_X/2-2),FACE_Y/3-2);
            printf("GAME OVER!\n");
            do
            {
                gotoxy(2*(FACE_X/2-2),FACE_Y/3+2);
                printf("是否重新开始游戏(y/n): ");
                scanf("%c",&n);
                gotoxy(2*(FACE_X/2-2),FACE_Y/3+4);
                if(n!='n' && n!='N' && n!='y' && n!='Y')
                    printf("输入错误，请重新输入!");
                else
                    break;
            }while(1);
            if(n=='n' || n=='N')
            {
                gotoxy(FACE_X/2+4,2*(FACE_Y/3));
                printf("按任意键退出游戏！");
                exit(0);
            }
            else if(n=='y' || n=='Y')
                main();
        }      
    }
    return 0;
}
///////////////////////////////////

