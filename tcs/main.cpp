#include<stdio.h>
#include<mutex>
#include<iostream>
#include<pthread.h>
#include<malloc.h>
#include<stdlib.h>
#include<unistd.h>
char f[1000][5000];
int width=180,height=45;
//int width=5,height=5;
std::mutex mut,mut2;
void ck()
{
	    // width=GetSystemMetrics(SM_CXFULLSCREEN);
		// height=GetSystemMetrics(SM_CYFULLSCREEN);
		for(int i=1;i<=height;i++){
			for(int j=1;j<width;j++){
				f[i][j]=' ';
			}
		}
		for(int i=1;i<=height;i++)
			f[i][1]=f[i][width]='|';
		for(int j=1;j<=width;j++)
			f[1][j]=f[height][j]='-';
		//printf("%d %d\n",width,height);
}
void show()
{
		system("clear");
	    for(int i=1;i<=height;i++){
			for(int j=1;j<=width;j++){
					printf("%c",f[i][j]);
			}
			printf("\n");
		}
}
int direct=-1;
void up(int x)
{
//	show();
	mut.lock();
	direct=x;
	mut.unlock();
}
char get_char()
{
	system("stty -echo");
	char c=getchar();
	system("stty echo");
	return c;
}
int dx[5]={0,0,1,-1};
int dy[5]={1,-1,0,0};
struct node
{
	int x,y;
	struct node* next;
	struct node* prev;
};
class cz
{
	public:
		cz(){
			head=crt(2,3,'.');
			tail=crt(2,2,'.');
			head->prev=tail;
			tail->next=head;
			food=crt(4,4,'*');
			len=2;
			direction_=0;
		}
		int move(){
			node* now=nullptr;
			int xx=head->x+dx[direction_];
			int yy=head->y+dy[direction_];
			if(Safe(xx,yy)){
				now=crt(xx,yy,'.');
			}else{
				return -1;
			}
			now->prev=head;
			head->next=now;
			head=now;
			if(f[food->x][food->y]=='.'){
				food->x=rand()%(height-3)+2;
				food->y=rand()%(width-3)+2;
				if(f[food->x][food->y]==' ')
					f[food->x][food->y]='*';
			}else{
				del();
			}
			return 0;
		}
		void who(node* now){
			printf("%d %d\n",now->x,now->y);
		}
		node* crt(int xx,int yy, char ch){
			node* now=(struct node*)malloc(sizeof(struct node));
			now->next=nullptr;
			now->prev=nullptr;
			now->x=xx;
			now->y=yy;
			f[xx][yy]=ch;
			return now;
		}
		void del(){
			f[tail->x][tail->y]=' ';
			if(tail->next!=head)

			tail=tail->next;
		}
		bool find()
		{
			node* z=tail;
			while(z!=head && z!=nullptr){
				z=z->next;
			}
			if(z==head)
				printf("Find head\n");
			else
				printf("Find null\n");
		}
		bool Safe(int xx,int yy){
			if(f[xx][yy]==' '||f[xx][yy]=='*')
				return true;
			else
				return false;
		}
		void UpdateDirection(){
			mut.lock();
			if(direct!=-1){
				direction_=direct;
				direct=-1;
			}
			mut.unlock();
		}

		int direction_;
		node* head;
		node* tail;
		node* food;
		int len;

};
void* Play(void* args)
{
	    ck();
		cz man=cz();
		show();
		while(true){
			sleep(1);
			man.UpdateDirection();
			if(man.move()==-1){
				printf("Game over!!\n");
				break;
			}
			show();
		}
}
int main()
{
		srand(int(time(0)));
		system("clear");
		pthread_t tid;
		pthread_create(&tid, NULL, Play, NULL);
		system("stty -echo");
		while(true){
			char ch = getchar();
			if(ch=='w')
				up(3);
			else if(ch=='s')
				up(2);
			else if(ch=='a')
				up(1);
			else if(ch=='d')
				up(0);
			else if(static_cast<int>(ch)==27)
				break;
		}
		system("stty echo");
		pthread_exit(NULL);

		return 0;
}

