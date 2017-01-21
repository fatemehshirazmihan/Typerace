#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <dirent.h>

COORD coord = { 0,0 };

void gotoxy(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
////////////////////////////////////////////////////

int all_wrongs=0;
int total_pause_time;
int total_number_player = 0;
int i,j,size=0;
FILE * file;

////////////////////////////////////////////////////

struct node
{
    char word[30];
    struct node *next;
}*head;

struct x
{
    int level;
    int speed;
    int all_wrongs;
    double score;
    char name[30];
}gamer[1000];

/////////////////////////////////////////////////////

void scan(void);
void quit(int,int,double,int);
void add_word(struct node *);
char* delete_word(int);
int count_levels(void);
void create(int);
char* new_word(void);
void best_score(void);
int add(char[]);
int find(char[]);
void play(int,int);
void game(void);

////////////////////////////////////////////////////

int main (void)
{
    file = fopen("player_info.txt" , "r");
    srand(time(0));
    scan();
    game();
    return 0;
}

void scan()
{
    rewind(file);
    char b[200];
    char *result = fgets(b,200,file);
    int cnt_player = 0;
    while(b!="\n" && result!=NULL)
    {
        int cnt = 0,flag=1;
        double tmp_num = 1.0;
        while(b[cnt]!=' ')
        {
            gamer[cnt_player].name[cnt] = b[cnt];
            cnt++;
        }
        cnt++;
        gamer[cnt_player].level = 0;
        while(b[cnt]!=' ')
        {
            gamer[cnt_player].level*=10;
            gamer[cnt_player].level+=b[cnt++] - '0';
        }
        cnt++;
        gamer[cnt_player].score = 0;

        while(b[cnt]!='.')
        {
            gamer[cnt_player].score*=10;
            gamer[cnt_player].score+=b[cnt++] - '0';
        }
        cnt++;
        while(b[cnt]>='0' && b[cnt]<='9')
        {
            tmp_num/=10.0;
            gamer[cnt_player].score+=(b[cnt++] -'0')*tmp_num;
        }
        cnt++;
        while(b[cnt]>='0' && b[cnt]<='9')
        {
            gamer[cnt_player].speed*=10;
            gamer[cnt_player].speed+=b[cnt++] - '0';
        }
        result = fgets(b,200,file);
        cnt_player++;
    }
    total_number_player = cnt_player;
}

void quit(int help,int tmp_level,double tmp_score , int level_type_speed)
{
        system("cls");
        printf("Exit!\ndo you want to save?[yes/no]\n");
        char temp[10];
        scanf("%s",temp);
        while(strcmp(temp,"yes")!=0 && strcmp(temp,"no")!=0)
        {
            system("cls");
            scanf("%s",temp);
        }
        if(strcmp(temp,"yes")==0)
        {
            gamer[help].level = tmp_level;
            gamer[help].score += tmp_score;
            gamer[help].speed = level_type_speed;
            gamer[help].all_wrongs=all_wrongs;
        }
        fclose(file);
        file = fopen("player_info.txt","w");
        for(i=0;i<total_number_player;i++)
            fprintf(file,"%s %d %lf %d %d\n",gamer[i].name,gamer[i].level,gamer[i].score , gamer[i].speed,gamer[i].all_wrongs);
        fclose(file);
}

void add_word(struct node *new_node)
{
    struct node *current = head;
    while(current->next!=NULL)
    {
        current = current->next;
    }
    current->next = new_node;
    new_node->next = NULL;
    size++;
}

void best_score()
{
    int i,j,temp=0,all=0;
    double speed[20];
    char namess[20][20];
    char temp12[20];
    FILE * fp;
    fp=fopen("player_info.txt","r");
    while(!feof(fp))
    {
     fscanf(fp,"%s %d %lf %d %d ",&namess[all],&i,&speed[all],&i,&i);
     all++;
    }
    fclose(fp);
    for(i=0;i<all-1;i++)
    {
        for(j=0;j<all-1;j++)
        {
            if(speed[j]<speed[j+1])
            {
                strcpy(temp12,namess[j]);
                temp=speed[j];
                strcpy(namess[j],namess[j+1]);
                speed[j]=speed[j+1];
                strcpy(namess[j+1],temp12);
                speed[j+1]=temp;
            }
        }
    }
    if(all>10)
    for(i=0;i<10;i++)
    {
        printf("%d : %s %lf\n",i+1,namess[i],speed[i]);
    }
    else
    {
    for(i=0;i<all;i++)
    {
        printf("%d : %s %lf\n",i+1,namess[i],speed[i]);
    }
    }
}
/////////////////////////////////////////////////////

char* delete_word(int index)
{
    struct node *current = head;
    int cnt = 0;
    while(cnt <= index-1)
    {
        current = current->next;
        cnt++;
    }
    char* wanted_word = (char *)malloc(sizeof(char)*30);
    struct node *tmp = current->next;
    for(i=0;i<30;i++)
        wanted_word[i] = tmp->word[i];
    current->next = current->next->next;
    free(tmp);
    size--;
    return wanted_word;
}

int count_levels()
{
    DIR *dp;
	struct dirent *ep;
	int cnt_levels =0;
	dp = opendir ("\levels");
	if (dp != NULL)
	{
		while (ep = readdir (dp))
        {
			char *file_name = ep->d_name,tmp[5];
			if(strlen(file_name)>5)
			{
                strncpy(tmp,file_name,5);
                if((strncmp(file_name,"level",5) == 0) && file_name[6]>='0' && file_name[6]<='9')
                    cnt_levels++;
			}
        }
		(void) closedir (dp);
	}
	else return -1;
    return cnt_levels;
}

void create(int help)
{
    int i,j;
    char string_ind[5];
    sprintf(string_ind,"%d",help);
    char address[30] = "levels\\level-";
    strcat(address,string_ind);
    strcat(address,".txt");
    FILE *game_level = fopen(address,"r");
    char* file_line = (char *)(malloc(sizeof(char)*500));
    head = (struct node*)malloc(sizeof(struct node));
    head->next = NULL;
    while(fgets(file_line,500,game_level))
    {
        char tmp_word[30];
        int cnt_word = 0;
        for(i=0;i<strlen(file_line);i++)
        {

            if(file_line[i] == ' ')
            {
                struct node *new_node = (struct node*)malloc(sizeof(struct node));
                for(j=0;j<30;j++) new_node->word[j] = 0;
                for(j=0;j<cnt_word;j++)
                    new_node->word[j] = tmp_word[j],tmp_word[j] = 0;
                add_word(new_node);
                cnt_word = 0;
            }
            else
                tmp_word[cnt_word++] = file_line[i];
        }
    }
    fclose(game_level);
}

char*  new_word()
{
    int random_index = rand()%size;
    return delete_word(random_index);
}

int add(char player_name[])
{
    for(i=0;i<30;i++)
        gamer[total_number_player].name[i] = player_name[i];
    total_number_player++;
    return total_number_player-1;
}

int find(char player_name[])
{
    for(i=0;i<total_number_player;i++)
    {
        if(strlen(player_name)!=strlen(gamer[i].name))
            continue;
        for(j=0;j<strlen(player_name);j++)
        {
            if(gamer[i].name[j] != player_name[j])
                break;
            else if(j==strlen(player_name)-1)
            return i;
        }
    }
    return -1;
}

void play(int help,int level_number)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    create(level_number);
    struct node *current = head;
    int number_of_words =size;
    int level_type_speed = gamer[help].speed;
    clock_t start = clock();
    double level_pause_time = 0;
    double level_score = gamer[help].score;
    while(size != 0)
    {
        system("color 06");
        double word_pause_time =0;
        double word_score =0;
        char *type_word = new_word();
        printf("%s\n" , type_word);
        i=0;
        int wrong =0 ;
        COORD cursor_pos;
        cursor_pos.X = 0;
        cursor_pos.Y = 0;
        SetConsoleCursorPosition(hStdOut , cursor_pos);
        while(i < strlen(type_word))
        {
            char input = _getch();
            if(input == type_word[i])
            {
                SetConsoleTextAttribute(hStdOut , FOREGROUND_GREEN);
                printf("%c" , input-32);
                cursor_pos.X = ++i;
                SetConsoleCursorPosition(hStdOut , cursor_pos);
            }
            else if(input == 'P')
            {
                clock_t pause_start = clock();
                char check_char = _getch();
                while(check_char != 'R' && check_char != 'Q')
                    check_char = _getch();
                if(check_char == 'R')
                {
                    word_pause_time += clock() - pause_start;
                }
                else if(check_char == 'Q')
                {
                    system("color 04");
                    quit(help,level_number,level_score , gamer[help].speed);
                    return ;
                }
            }
            else if(input == 'Q')
            {
                system("color 04");
                quit(help,level_number,level_score , gamer[help].speed);
                return ;
            }
            else
            {
                SetConsoleTextAttribute(hStdOut , FOREGROUND_RED);
                printf("%c" , type_word[i]);
                SetConsoleCursorPosition(hStdOut , cursor_pos);
                wrong++;
                all_wrongs++;
            }

        }
        word_score = (3*i - wrong)/(clock() - start - word_pause_time/1000);
        level_score += word_score;
        level_pause_time += word_pause_time;
        system("cls");
    }

        level_type_speed = (level_type_speed*gamer[help].level + (number_of_words/((clock() - start - level_pause_time)/1000)*60))/(gamer[help].level +1);
        system("color 01");
        printf("score: %lf , speed : %d WPM , all wrongs: %d \ndo you want to continue?\n",level_score , level_type_speed,all_wrongs);
        char temp[10];
        scanf("%s",temp);
        if(strcmp(temp,"yes")==0)
        {
            if(level_number+1>count_levels())
            {
                printf("<< the game is finish >>");
                quit(help,level_number,level_score , level_type_speed);
            }
            else{
                gamer[help].speed = level_type_speed;
                play(help,level_number+1);
            }
        }
        else quit(help,level_number+1,level_score , level_type_speed);

}

void game()
{
    system("color 03");
    printf("please enter your name:");
    char name[30];
    scanf("%s",name);
    system("cls");
     system("color 05");
    printf("what do you want to do?\n1)new game \n2)old game \n3)view the ranks based on score \n");
    int model_of_game;
    gotoxy(24,0);
    scanf("%d",&model_of_game);
    while(model_of_game != 1 && model_of_game != 2 && model_of_game != 3)
    {
        system("cls");
        printf("what do you want to do?\n1)new game \n2)old game \n3)view the ranks based on score \n");
        gotoxy(24,0);
        scanf("%d" , &model_of_game);
    }
    if(model_of_game == 2)
    {
        while(find(name) ==-1 && gamer[find(name)].level == 0)
        {
            system("cls");
            system("color 03");
            printf("player not found!\n");
            game();
            scanf("%s" , name);
        }
        int help = find(name);
        play(help,gamer[help].level);
    }
    else if(model_of_game ==1)
    {
        int total = count_levels();
        system("cls");
        system("color 03");
        printf("Enter level for play (at most %d levels):",total);
        int number;
        scanf("%d",&number);
        while(number<1 || number>total)
        {
            system("cls");
            system("color 03");
            printf("Enter level for play (at most %d levels):",total);
            scanf("%d",&number);
        }
        int help = find(name);
        if(help==-1)
            help = add(name);
        else
        {
            gamer[help].level = 0;
            gamer[help].speed = 0;
            gamer[help].score = 0;
        }
        play(help,number);
    }
    else if(model_of_game ==3)
    {
        system("cls");
        best_score();
        system("color 03");
        printf(" press any key\n");
        getch();
        system("cls");
        game();
    }
}



