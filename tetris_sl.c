#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>


#define SPACE 32	//////////////////////////////////
#define LEFT  75	//////////////////////////////////
#define RIGHT 77	//////////��ⰴ��ֵ
#define DOWN 80	    //////////
#define ESC 27	    //////////////////////////////////
#define ENTER 13	//////////////////////////////////

#define Wall 2	    //����߽�ֵ
#define Box 1	    //����߽�ֵ
#define Kong 0	    //�ÿ�
#define FACE_Y 30	//����ߴ紹ֱ�ߴ�
#define FACE_X 20   //����ߴ�ˮƽ�ߴ�


void gotoxy(int x,int y);  	//�ƶ����
int color(int c);           //��ɫ
void hidden_cursor();  	    //���ع��
void init_interface();  	//��ʼ������
void init_dia();   		    //��ʼ��������Ϣ
void draw_dia(int base,int space_c,int x,int y);   	//������
void draw_kong(int base,int space_c,int x,int y); 	//���Ƿ���
int hit_bottom(int n,int space_c,int x,int y);  	//�ж��Ƿ񵽵�
void start_game();   		//��ʼ��Ϸ
//void vdrop();
int gameover();				//����Ƿ���Ϸ���� 
int eliminate();		    //����
void read_file();   		//��д��߼�¼
void write_file();  		//д��߼�¼

int grade=0;		        //��ǰ����
int grade_max=0;		    //��߼�¼
int rand_n=0;


struct Interface
{
    int data[FACE_X+10][FACE_Y];    //��ֵ��Ϊ1�Ƿ��飬Ϊ0�ǿո�
    int color[FACE_X+10][FACE_Y];   //��Ӧ�������ɫ
}face;

typedef struct Diamonds
{   int space[4][4];        	    //4*4����Ϊ1Ϊ���飬Ϊ0 Ϊ��
}Dia;
Dia dia[7][4];  		            //һά����7�����飬��ά��ʾ��ת����


int main(void)
{   
    system("cls");
    system("title ����˹����");
    color(7);
    system("mode con cols=60 lines=30");  //���ڿ�ȸ߶�,����FACE_X FACE_Y��ֵ�趨�� ��FACE_X+10��*2   FACE_Y 
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
    
    
    //��ʼ������
	// 
    return 0;
}
/////////////////////////////////////////////////



void start_game()
{  
	int n,ch,t=0,y=0,x=FACE_X/2-2,i,j;
    int space_c=0;//��ת����
    draw_kong(rand_n,space_c,FACE_X+2,4);
    n=rand_n;   
    rand_n=rand()%7;    //���������һ��
    color(rand_n);
    draw_dia(rand_n,space_c,FACE_X+2,4);
    while(1)
    {   
        color(n);
        draw_dia(n,space_c,x,y);//����ͼ��
        if(t==0)
        {
        	t=15000;
		}
        while(--t)
        {   if(kbhit()!=0)//�����������
                break;  
        }
        if(t==0)
        {
            if(hit_bottom(n,space_c,x,y+1)==1)
            {   draw_kong(n,space_c,x,y);
                y++;            //���½���
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
            switch(ch)     //�ƶ�
            {
            case LEFT: if(hit_bottom(n,space_c,x-1,y)==1)   //�ж��Ƿ�����ƶ�
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
                        printf("---��Ϸ����!---\n\n");
                        gotoxy(FACE_X/2+10,FACE_Y/2-2);
                        printf("---��������˳�!---\n");
                        getch();
                        exit(0);
                        break;
            case  'R':
            case  'r':  main();
                        exit(0);
            case  'S':
            case  's':  while(1)
                        {   if(kbhit()!=0)//�����������
                            break;  
                        }
                        break;
            }
        }
    }
}


void init_interface()//����
{   int i,j;
    for(i=0;i<FACE_Y;i++)
    {   for(j=0;j<FACE_X+10;j++)
        {   if(j==0 || j==FACE_X-1 || j==FACE_X+9)
            {   face.data[j][i]=Wall;
                gotoxy(2*j,i);
                printf("��");
            }
            else if(i==FACE_Y-1)
            {   face.data[j][i]=Box;
                gotoxy(2*j,i);
                printf("��");
            }
            else
            face.data[j][i]=Kong;
        }
    }
    gotoxy(2*FACE_X+2,FACE_Y-18);
    printf("���ƣ���");

    gotoxy(2*FACE_X+2,FACE_Y-16);
    printf("���ƣ���");

    gotoxy(2*FACE_X+2,FACE_Y-14);
    printf("��ת��space");

    gotoxy(2*FACE_X+2,FACE_Y-12);
    printf("��ͣ: S");

    gotoxy(2*FACE_X+2,FACE_Y-10);
    printf("�˳�: ESC");

    gotoxy(2*FACE_X+2,FACE_Y-8);
    printf("���¿�ʼ:R");

    gotoxy(2*FACE_X+2,FACE_Y-6);
    printf("��߼�¼:%d",grade_max);

    gotoxy(2*FACE_X+2,FACE_Y-4);
    printf("������%d",grade);

}





void gotoxy(int x,int y) //�ƶ�����
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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //����������ɫ
    return 0;
}


void hidden_cursor()//���ع��
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(hOut,&cci);
  cci.bVisible=0;//��1Ϊ��ʾ����0Ϊ����
  SetConsoleCursorInfo(hOut,&cci);
}


void init_dia()
{
    int i,j,k,z;
    int tmp[4][4];
    for(i=0;i<3;i++)
        dia[0][0].space[i][1]=1;
    dia[0][0].space[1][2]=1;     //����

    for(i=1;i<4;i++)
        dia[1][0].space[1][i]=1;
    dia[1][0].space[2][1]=1;    //L��--1

    for(i=1;i<4;i++)
        dia[2][0].space[2][i]=1;
    dia[2][0].space[1][1]=1;    //L��--2

    for(i=0;i<2;i++)
    {   dia[3][0].space[i][1]=1;
        dia[3][0].space[i+1][2]=1; //Z��--1

        dia[4][0].space[i+1][1]=1;
        dia[4][0].space[i][2]=1;//Z��--2

        dia[5][0].space[1][i+1]=1;
        dia[5][0].space[2][i+1]=1;//������
    }
    for(i=0;i<4;i++)
        dia[6][0].space[2][i]=1;//1��
    //////////////////////////////////����7����״
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
    ///////////////////////////////////��ת���21����״
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
            printf("��");
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



int hit_bottom(int n,int space_c,int x,int y)   //�ж��Ƿ񵽵�
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


void read_file()    //��ȡ��߼�¼
{
    FILE *fp;
    fp=fopen("����˹�����¼.txt","r+");
    if(fp==NULL)
    {
        fp=fopen("����˹�����¼.txt","w+");
        fwrite(&grade_max,sizeof(int),1,fp);
    }
    fseek(fp,0,0);
    fread(&grade_max,sizeof(int),1,fp);
    fclose(fp);
}
void write_file()   //������߼�¼
{
    FILE *fp;
    fp=fopen("����˹�����¼.txt","r+");
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
        if(sum==FACE_X-2)   //��һ�У�����
        {   
            grade+=100;
            color(7);
            gotoxy(2*FACE_X+2,FACE_Y-4);
            printf("������%d",grade);
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
                        printf("��");
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
            Sleep(2000); //��ʱ
            system("cls");
            color(7);
            gotoxy(2*(FACE_X/2-2),FACE_Y/3);
            if(grade>grade_max)
            {
                printf("��ϲ�����Ƽ�¼��Ŀǰ��߼�¼Ϊ��%d",grade);
                write_file();
            }
            else if(grade==grade_max)
                printf("���¼��ƽ����ͻ����ļ��ޣ�");
            else
                printf("�����Ŭ����������߼�¼ֻ�%d",grade_max-grade);
            gotoxy(2*(FACE_X/2-2),FACE_Y/3-2);
            printf("GAME OVER!\n");
            do
            {
                gotoxy(2*(FACE_X/2-2),FACE_Y/3+2);
                printf("�Ƿ����¿�ʼ��Ϸ(y/n): ");
                scanf("%c",&n);
                gotoxy(2*(FACE_X/2-2),FACE_Y/3+4);
                if(n!='n' && n!='N' && n!='y' && n!='Y')
                    printf("�����������������!");
                else
                    break;
            }while(1);
            if(n=='n' || n=='N')
            {
                gotoxy(FACE_X/2+4,2*(FACE_Y/3));
                printf("��������˳���Ϸ��");
                exit(0);
            }
            else if(n=='y' || n=='Y')
                main();
        }      
    }
    return 0;
}
///////////////////////////////////

