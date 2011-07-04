#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

int ret ;
char buf[128] = "\0";
sqlite3 *db = NULL;

int check(void * p,int argc ,char ** argv,char **argvv)
{
	int i;
	
	for(i = 0;i < argc;i++)
	{
		printf("%s %s ",argvv[i],argv[i]);
	}
	printf("\n");

	return 0;
}

int  func(void * p,int argc,char**argv,char** argvv)
{
	if(argc > 0)
	{
		if(*argv[0] == '1')
		{	printf("No right to BORROW!");
			return -1;
		}
	}
	return 0;
}

int  fund(void * p,int argc, char **argv,char **argvv)
{
	if(argc > 0)
	{
		if(*argv[0] == '0')
		{
			printf("No more books");
			return -1;
		}
	}
	return 0;
}

void borrow_book(void)
{
	char id[32],bookid[32];
	char str[128],str2[128];
	struct tm *timeinfo;
        time_t  tim;

        time(&tim);
        timeinfo = localtime(&tim);

	scanf("%s %s",id,bookid);
	sprintf(buf,"select is_debate from std_info where stu_id = '%s';",id);
	ret = sqlite3_exec(db,buf,func,NULL,NULL);
	sprintf(buf,"select rest_book from book_info where book_id = '%s';",bookid);
	ret = sqlite3_exec(db,buf,fund,NULL,NULL);
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(str2,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 2+timeinfo->tm_mon,
timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(buf,"insert into book_record values('%s','%s','%s','%s';)",id,bookid,str,str2);
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
        {
                printf("Fail to sqlite");
                return;
        }
	sprintf(buf,"update book_info set rest_book = rest_book - 1 where book_id = '%s';",bookid);
	printf("<%s>\n", buf);
        ret = sqlite3_exec(db,buf,fund,NULL,NULL);

	
}

int back(void *p ,int argc ,char** argv,char**argvv)
{
	char * pi;
	
	pi = (char*)p;
	strcpy(pi, argv[0]);
	
	return 0;
}

void lend_book(void)
{
	char id[32],bookid[32],p[32];
	char str[128];
	
	struct tm *timeinfo;
        time_t  tim;

        time(&tim);
        timeinfo = localtime(&tim);
	
	scanf("%s %s",id,bookid);
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	sprintf(buf,"select len_time from book_record where bor_id = '%s' and bor_bookid = '%s';",id,bookid);
	ret = sqlite3_exec(db,buf,back,p,NULL);
	if(strcmp(str,p) >= 0)
	{
		sprintf(buf,"update bor_record set is_back = 1 where bor_id = '%s'and bor_bookid = '%s';",id,bookid);
		ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
		sprintf(buf,"update bor_record set is_limitted = 1 where bor_id = '%s'and bor_bookid = '%s';",id,bookid);
                ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
		sprintf(buf,"update std_info set is_debate = 1 where bor_id = '%s'and bor_bookid = '%s';",id,bookid);
                ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	}
	else
	{
		sprintf(buf,"update bor_record set is_back = 1 where bor_id = '%s'and bor_bookid = '%s';",id,bookid);
                ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
		
	}
	
	 sprintf(buf,"update book_info set rest_book = rest_book + 1 where book_id = '%s';",bookid);
         ret = sqlite3_exec(db,buf,fund,NULL,NULL);
}

void check_stuinfo(void)
{
	char id[32];

        scanf("%s",id);
        sprintf(buf,"select * from std_info where bor_id = '%s';",id);
        ret = sqlite3_exec(db,buf,check,NULL,NULL);
	
}

void check_boinfo(void)
{
	char id[32];

        scanf("%s",id);
        sprintf(buf,"select * from book_info where bor_id = '%s';",id);
        ret = sqlite3_exec(db,buf,check,NULL,NULL);

}

void check_bor_lend(void)
{
	char id[32];
	
	scanf("%s",id);
	sprintf(buf,"select * from book_record where bor_id = '%s';",id);
	ret = sqlite3_exec(db,buf,check,NULL,NULL);
}
void en_password(void);
void user_mode(void)
{
	int num;
	en_password();
	
while(1)
{
	printf("1.Borrow\n2.lend\n3.check_stdinfo\n4.check_boinfo\n5.check_lend\n6.quit\n");
	scanf("%d",&num);
	
	switch(num)
	{
		case 1:{borrow_book();break;}
		case 2:{lend_book();break;}
		case 3:{check_stuinfo();break;}
		case 4:{check_boinfo();break;}
		case 5:{check_bor_lend();break;}
		case 6:{return;}
	}

}
}

void stu_insert(void)
{
        char name[32],password[16],id[32];

        printf("insert:\n");
        scanf("%s %s %s",id,name,password);
        sprintf(buf,"insert into std_info(stu_id,stu_name,stu_password) values('%s','%s','%s');",id,name,password);
        ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
        if(ret != SQLITE_OK)
        {
                printf("Fail to sqlite\n");
                return;
        }
        printf("Print Success\n");
}

void stu_delete(void)
{
        char id[32];

        scanf("%s",id);
        sprintf(buf,"delete from std_info where stu_id = '%s';",id);
        ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite");
		return;
	}

}

void stu_correct(void)
{
        char name[32];
	char id[32];

        scanf("%s%s",name,id);
        sprintf(buf,"update std_info set name ='%s' where stu_id = '%s';",name,id);
        ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite");
		return;
	}
}	

void is_debate(void)
{
}

void stu_check(void)
{
        char id[32];

        scanf("%s",id);
        sprintf(buf,"select * from std_info where stu_id = '%s';",id);
        ret = sqlite3_exec(db,buf,check,NULL,NULL);
}


void stu_man(void)
{
	int num;
	
while(1)
{
	printf("1.Insert\n2.Delete\n3.Correct\n4.Debate Record\n5.Check\n6.Quit\n");
	printf("Please input your choose:");
	scanf("%d",&num);
	
	switch(num)
	{
		case 1:{stu_insert();break;}
		case 2:{stu_delete();break;}
		case 3:{stu_correct();break;}
		case 4:{is_debate();break;}
		case 5:{stu_check();break;}
		case 6:{return;}
	}
}
}

void b_borinfo(void)
{
	int num;
while(1)
{	
	printf("1 . For Book\n2 . For Person\n3 . For Date\n");
	printf("Please choose it:");
	scanf("%d",&num);

	switch(num)
	{
		case 1:
		{	
			char id[8];

			scanf("%s",buf);
			sprintf(buf,"select bor_id,bor_time,len_time,is_back,is_limitted from bor_record where bor_bookid = '%s';",id);
			ret = sqlite3_exec(db,buf,check,NULL,NULL);
		        if(ret != SQLITE_OK)
		        {
		                printf("Fail to sqlite");
		                return;
		        }

		}
		case 2:
		{
			char name[32];
		
			scanf("%s",name);
			sprintf(buf,"select bor_bookid,bor_time,len_time,is_back,is_limitted from bor_record where bor_name = '%s';",name);
			if(ret != SQLITE_OK)
                        {
                                printf("Fail to sqlite");
                                return;
                        }

		}
		case 3:
		{
			int date = 0;
	
			sprintf(buf,"select bor_id,bor_bookid,bor_time from bor_record where time BETWEEN now() - %d*3600 AND now();",date);
			if(ret != SQLITE_OK)
                        {
                                printf("Fail to sqlite");
                                return;
                        }
		}
		case 4: return;
	}
}
}

void b_check(void)
{
	char id[32];

	scanf("%s",id);
	sprintf(buf,"select * from book_info where book_id = '%s';",id);
	ret = sqlite3_exec(db,buf,check,NULL,NULL);
	if(ret != SQLITE_OK)
        {
                printf("Fail to sqlite");
                return;
        }
	printf("Check success\n");
}

void b_correct(void)
{
	char name[32],id[32];
	
	scanf("%s %s",name,id);
	sprintf(buf,"update book_info set name ='%s' where id = '%s';",name,id);
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
        {
                printf("Fail to sqlite");
                return;
        }
        printf("Correct success\n");
}

void b_delete(void)
{
	char book_id[32];
	
	scanf("%s",book_id);
	sprintf(buf,"delete from book_info where id = '%s';",book_id);
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
        {
                printf("Fail to sqlite");
                return;
        }
	printf("Delete success\n");
}

void b_insert(void)
{
	int total_book,rest_book;
	char book_name[32],store[32],book_id[32];
	
	printf("insert:\n");	
	scanf("%s %s %s %d %d",book_id,book_name,store,&total_book,&rest_book);
	sprintf(buf,"insert into book_info values('%s','%s','%s',%d,%d);",book_id,book_name,store,total_book,rest_book);
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite");
		return;
	}
	printf("Print Success\n");
}

void book_man(void)
{
	int num;
while(1)
{
	printf("1 . Insert\n2 . Delete\n3 . Correct\n4 . Check\n,5 . Borrow_info\n6 . Quit\n");
	printf("Please choose it:");
	scanf("%d",&num);

	switch(num)
	{
		case 1:{b_insert();break;}
		case 2:{b_delete();break;}
		case 3:{b_correct();break;}
		case 4:{b_check();break;}
		case 5:{b_borinfo();break;}
		case 6:{return;}
	}
}
}

int funct(void * p,int argc ,char**argv,char** argvv)
{
	char* pi;

	if(argc == 0)
	{printf("No this student\n");}
	pi = (char*)p;
	strcpy(pi,argv[0]);
	
	return 0;
}

void en_password(void)
{
	char id[32];
	int i;
	char p[32],*q;
	
	for(i = 0;i < 3;i++)
	{
		printf("Please input the name:");
		scanf("%s",id);

		sprintf(buf,"select stu_password from std_info where stu_name = '%s';",id);
		ret = sqlite3_exec(db,buf,funct,p,NULL);
		q = getpass("Please input the password:");
		if(strcmp(q,p) == 0)
			return ;
		else 
			printf("Error!Please try it again\n");
	}	
	printf("Thanks to use the system\n");

}

void man_mode(void)
{
	int num;
		
while(1)
{
	printf("1. Book Management\n2 . Student Management\n3 . Quit\n");
	printf("Please choose it:");
	scanf("%d",&num);

	switch(num)
	{
		case 1:{book_man();break;}
		case 2:{stu_man();break;}
		case 3:{return;}
	}
}
}

void init(sqlite3 *db)
{
	strcpy(buf,"create table book_info(book_id char8 primary key,book_name varchar32,store varchar32,total_book integer,rest_book integer);");
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite\n");
		return;
	}
	strcpy(buf,"create table std_info(stu_id char8 primary key,stu_name varchar32,stu_password varchar16,is_debate integer DEFAULT 0);");
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite\n");
		return;
	}
	strcpy(buf,"create table bor_record(bor_id varchar32,bor_bookid varchar32,bor_time date,len_time date,is_back integer DEFAULT 0,is_limitted integer DEFAULT 0);");
	ret = sqlite3_exec(db,buf,NULL,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("Fail to sqlite\n");
		return;
	}
	
	printf("Success\n");
}

int main(void)
{
	int cho;
	
	ret = sqlite3_open("lib_data.db",&db);
        if(ret != SQLITE_OK)
        {
                printf("Fail to open\n");
                return -1;
        }
	init(db);
while(1)
{
	printf("Welcome to use the system\n");
	printf("1 . User Mode\n2 . Manage Mode\n3 . Quit\n");
	printf("Please choose the mode:");
	scanf("%d",&cho);
	
	switch (cho)
	{
		case 1:{user_mode();break;}
		case 2:{man_mode();break;}
		case 3:{exit(1);}
	}
}
	return 0;
}
