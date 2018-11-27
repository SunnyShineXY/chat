#include"./head.h"


//sm=(struct msg*)malloc(2048);
//rm = (struct msg*)malloc(2048);
struct msg sm;
char file_name[100][100];
int num = 0;
int type;    
FILE *fpr,*fpw;                             //功能完成

GROUP g_p[100];
LOGINING logining[100];
int i = 0;
int count = 0;
void sigfun()
{
    close(socket_fd);
    exit(0);
}

void send_file(int accept_fd,struct msg rm)
{

    printf("hello     ....\n");
    printf("test %d %s\n\n",rm.type,rm.context);
    if((fprintf(fpw,"%s",rm.context)) <= 0)
    {
        printf("zhuijiashibai\n");
    }
}

void recv_file(int accept_fd,struct msg rm)
{
    sm.type = RECV_FILER;
    char buf[100];
    int j;
    for(j=0;j<num;j++)
    {
        strcpy(buf,file_name[j]);
        strcat(sm.context,buf);
        strcat(sm.context," ");
    }
    printf("%s",sm.context);
    send(accept_fd,&sm,sizeof(sm),0);
}

void recv_filer(int accept_fd,struct msg rm)
{
    sm.type = RECV_FILE;
    int op = 1;
    while(op)
    {
        memset(sm.context,0,1024);
        fread(sm.context,sizeof(char),1023,fpr);
        printf("%s",sm.context);
        send(accept_fd,&sm,sizeof(sm),0);
        sleep(3);
        if(feof(fpr))
        {
            sm.type = MSG_DONE;
            op = 0;
            send(accept_fd,&sm,sizeof(sm),0);
        }
    }
    fclose(fpr);
}


void group_chat(int accept_fd,struct msg rm)
{
    int j;
    int flag = 0;
    for(j=0;j<i;j++)
    {
        if(accept_fd == g_p[j].accept_fd_flag && g_p[j].is_no_talk == 0)
            flag = 1;
    }
    if(flag == 0)
    {
        sm.type = GROUP_CHATER;
        sprintf(sm.context,"You have been jinyaning");
        if(send(accept_fd,&sm,sizeof(sm),0) < 0)
        {
            printf("send fail!\n");
            exit(0);
        }
    }
    else
    {
        for(j=0;j<i;j++)
        {
            sm.type = GROUP_CHATER;
            sprintf(sm.context,"group_chating:\n%s says:%s",rm.send_account,rm.context);
            if(accept_fd != g_p[j].accept_fd_flag)
                if(send(g_p[j].accept_fd_flag,&sm,sizeof(sm),0) < 0)
                {
                    printf("send recv fail!\n");
                    exit(0);
                }

        }
    }
}

void private_chat(int accept_fd,struct msg rm)
{

    int j;
    int recv_fd;
    int flag = 0;
    for(j=0;j<count;j++)
    {
        printf("\ntesttest\n");
        printf("%s\n",rm.recv_account);
        printf("%d %s\n",logining[j].accept_fd,logining[j].account_number);

        if(strcmp(rm.recv_account,logining[j].account_number) == 0)
        {
            printf("testetsgetcdbv\n");
            recv_fd = logining[j].accept_fd;
            flag = 1;
            printf("recv_name = %s",logining[j].account_number);
            break;
        }
    }
    printf("flag = %d\n",flag);
    if(flag == 1)
    {
        sm.type = PRIVATE_CHATER;
        sprintf(sm.context,"private_chating:%s says:%s",rm.send_account,rm.context);
        if(send(recv_fd,&sm,sizeof(sm),0) < 0)
        {
            printf("send recv fail!\n");
            exit(0);
        }
    }
    else
    {
        sm.type = PRIVATE_CHATER;
        sprintf(sm.context,"%s is not online!",rm.recv_account);
        if(send(accept_fd,&sm,sizeof(sm),0) < 0)
        {
            printf("send fail!\n");
            exit(0);
        }
    }

}

void kick(int accept_fd,struct msg rm)
{

    int j;
    int flag;
    int n = i;
    for(j=0;j<i;j++)
    {
        if(accept_fd == g_p[j].accept_fd_flag && g_p[j].is_qunzhu == 1)
            flag = 1;
    }
    if(flag == 1)
    {
        for(j=0;j<n;j++)
        {
            if(strcmp(rm.account_number,g_p[j].account_number) == 0)
            {
                int k;
                for(k=j+1;k<n;k++)
                {
                    g_p[k-1]=g_p[k];
                }
                n--;
            }
        }
        i--;
        sm.type = KICKER;
        sm.flag = 1;
        strcpy(sm.context,"Kick ok!");
    }
    else
    {
        sm.type = KICKER;
        sm.flag = 0;
        strcpy(sm.context,"You are not qunzhu!");
    }
    send(accept_fd,&sm,sizeof(sm),0);
}

void no_talk(int accept_fd,struct msg rm)
{
    int j;
    int flag;
    int n = i;
    for(j=0;j<i;j++)
    {
        printf("test\n");
        if(accept_fd == g_p[j].accept_fd_flag && g_p[j].is_qunzhu == 1)
        {    flag = 1;
            printf("%s qunzhu\n",g_p[j].account_number);}
    }
    if(flag == 1)
    {
        for(j=0;j<n;j++)
        {
            if(strcmp(rm.account_number,g_p[j].account_number) == 0)
            {
                g_p[j].is_no_talk = 1;
                printf("%s %d %d %d %d\n",g_p[j].account_number,g_p[j].is_online,g_p[j].accept_fd_flag,g_p[j].is_qunzhu,g_p[j].is_no_talk);
            }

        }
        sm.type = NO_TALKER;
        sm.flag = 1;
        strcpy(sm.context,"Jinyan ok!");
    }
    else
    {
        sm.type = NO_TALKER;
        sm.flag = 0;
        strcpy(sm.context,"You are not qunzhu!");
    }
    send(accept_fd,&sm,sizeof(sm),0);
}    

void release_no_talk(int accept_fd,struct msg rm)
{
    int j;
    int flag;
    int n = i;
    for(j=0;j<i;j++)
    {
        printf("test\n");
        if(accept_fd == g_p[j].accept_fd_flag && g_p[j].is_qunzhu == 1)
        {    flag = 1;
            printf("%s qunzhu\n",g_p[j].account_number);}
    }
    if(flag == 1)
    {
        for(j=0;j<n;j++)
        {
            if(strcmp(rm.account_number,g_p[j].account_number) == 0)
            {
                g_p[j].is_no_talk = 0;
                printf("%s %d %d %d %d\n",g_p[j].account_number,g_p[j].is_online,g_p[j].accept_fd_flag,g_p[j].is_qunzhu,g_p[j].is_no_talk);
            }

        }
        sm.type = RELEASE;
        sm.flag = 1;
        strcpy(sm.context,"release ok!");
    }
    else
    {
        sm.type = RELEASE;
        sm.flag = 0;
        strcpy(sm.context,"You are not qunzhu!");
    }
    send(accept_fd,&sm,sizeof(sm),0);
}    

void print_number(int accept_fd)
{
    sm.type = PRINT_NUMBERER;
    printf("print_number...\n");
    sprintf(sm.context,"online number is %d",i);
    printf("%s",sm.context);
    if(send(accept_fd,&sm,sizeof(sm),0) < 0)
    {
        printf("send sm fail!\n");
        perror("send");
        exit(0);
    }
}

void print_person(int accept_fd)
{
    sm.type = PRINT_PERSONER;
    char name[30];
    sm.context[0] = '\0';
    int j;
    for(j=0;j<i;j++)
    {
        strcpy(name,g_p[j].account_number);
        strcat(sm.context,name);
        strcat(sm.context," ");
    }
    if(send(accept_fd,&sm,sizeof(sm),0) < 0)
    {
        printf("send fail!\n");
        exit(0);
    }
}

void group_chating(int accept_fd,struct msg rm)
{
    int j;
    char account[50];
    printf("group_chating!\n");
    if(i == 0)
    {

        strcpy(g_p[i].account_number,rm.account_number);
        g_p[i].is_online = 1;
        g_p[i].accept_fd_flag = accept_fd;
        g_p[i].is_qunzhu = 1;
        g_p[i].is_no_talk = 0;
        i++;
    }
    else
    {

        strcpy(g_p[i].account_number,rm.account_number);
        g_p[i].is_online = 1;
        g_p[i].accept_fd_flag = accept_fd;
        g_p[i].is_qunzhu = 0;
        g_p[i].is_no_talk = 0;
        i++;
    }
    printf("group_chating end\n");
    printf("%s %d %d %d %d\n",g_p[i-1].account_number,g_p[i-1].is_online,g_p[i-1].accept_fd_flag,g_p[i-1].is_qunzhu,g_p[i-1].is_no_talk);
    //  group_chat();
}
int init_mysql()
{

    g_conn = mysql_init(NULL);
    if(!mysql_real_connect(g_conn,g_host_name,g_user_name,g_passwd,g_db_name,g_db_port,NULL,0))
        return -1;
}

void regist(int accept_fd,struct msg rm)
{
    int res;
    while(1)
    {
        init_mysql();
        char sql[MAX_BUF_SIZE];
        sprintf(sql,"INSERT INTO `chat` VALUES('%s','%s','%s','%s')",rm.account_number,rm.secret,rm.insurance_question,rm.insurance_answer);

        if(mysql_real_query(g_conn,sql,strlen(sql)))
            exit(0);
        type = 1;
        res = write(accept_fd,&type,sizeof(type));
        if(res < 0)
        {

            perror("write");
            exit(0);
        }
        break;
    }
}

void change_secret(int accept_fd,struct msg rm)
{

    init_mysql();
    char message[50];
    int len;
    while(1)
    {
        char sql[MAX_BUF_SIZE];
        sprintf(sql,"select * from `chat` where account_number = '%s'",rm.account_number);
        printf("%s\n",sql);
        mysql_real_query(g_conn,sql,strlen(sql));
        g_res = mysql_store_result(g_conn);
        g_row = mysql_fetch_row(g_res);

        if(strcmp(rm.insurance_answer,g_row[3]) == 0)
        {

            type = 1;
            send(accept_fd,&type,sizeof(int),0);
            if(len < 0)
            {

                printf("recv fail!\n");
                exit(0);
            }
            sprintf(sql,"UPDATE `chat` SET secret = '%s' WHERE account_number = '%s'",rm.secret,rm.account_number);
            if(mysql_real_query(g_conn,sql,strlen(sql)))
                exit(0);
            send(accept_fd,&type,sizeof(int),0);
            if(len < 0)
            {

                printf("recv fail!\n");
                exit(0);
            }
            break;
        }
        else
        {

            type = 0;
            send(accept_fd,&type,sizeof(int),0);
        }
    }
}

void login(int accept_fd,struct msg rm)
{
    printf("login....\n");
    int recv_len,j;

    logining[count].accept_fd = accept_fd;
    strcpy(logining[count].account_number,rm.account_number);
    count ++;

    printf("sb online\n");
    for(j=0;j<count;j++)
        printf("%d %s\n",logining[j].accept_fd,logining[j].account_number);

    init_mysql();
    char sql[MAX_BUF_SIZE];
    sprintf(sql,"select * from `chat` where account_number = '%s' and secret = '%s'",rm.account_number,rm.secret); 
    int flag;
    flag = mysql_real_query(g_conn,sql,strlen(sql));
    if(flag)
        exit(0);
    g_res = mysql_store_result(g_conn);
    g_row = mysql_fetch_row(g_res);

    if(g_row!=0)
    {
        type = 1;
        send(accept_fd,&type,sizeof(int),0);
        printf("%d type\n",type);
    }
    else
    {
        type = 0;
        send(accept_fd,&type,sizeof(int),0);
    }
    printf("send ok!\n""");


}
void exit_module(int accept_fd)
{

    printf("%d is exit!\n",accept_fd);
    int j;
    int n = count;
    for(j=0;j<n;j++)
    {
        if(accept_fd == logining[j].accept_fd)
        {
            int k;
            for(k=j+1;k<n;k++)
            {
                logining[k-1]=logining[k];
            }
            n--;
        }
    }
    count--;
    pthread_exit(NULL);
}
void *deal_data(void *con)
{
    struct msg rm;
    int accept_fd = *(int *)con;
    int flag;
    int recv_len = 0;
    while(1)
    {

        recv_len = recv(accept_fd,&rm,sizeof(rm),0);
        if(recv_len > 0)
        {

            printf("recv 1  ok!\n");
        }
        printf("%d = perform",rm.type);
        switch(rm.type)
        {

            case REGISTER:regist(accept_fd,rm);break;
            case LOGINER:login(accept_fd,rm);break;
            case GROUP_CHATER:group_chat(accept_fd,rm);break;
            case PRIVATE_CHATER:private_chat(accept_fd,rm);break;
            case CHANGE_SECRETER:change_secret(accept_fd,rm);break;
            case EXITER:exit_module(accept_fd);break;
            case KICKER:kick(accept_fd,rm);break;
            case GROUP_CHATING:group_chating(accept_fd,rm);break;
            case PRINT_NUMBERER:print_number(accept_fd);break;
            case NO_TALKER:no_talk(accept_fd,rm);break;
            case PRINT_PERSONER:print_person(accept_fd);break;
            case FILE_NAME:
                                {

                                    printf("open fail!\n");
                                    fpw = fopen(rm.context,"w");
                                    strcpy(file_name[num],rm.context);
                                    num++;break;
                                }
            case MSG_DONE:
                                {
                                    fclose(fpw);
                                    sm.type = SEND_FILER;
                                    strcpy(sm.context,"Updata ok!");
                                    send(accept_fd,&sm,sizeof(sm),0);
                                    break;
                                }	
            case SEND_FILER:send_file(accept_fd,rm);break;
            case RECV_FILER:recv_file(accept_fd,rm);break;
            case RELEASE:release_no_talk(accept_fd,rm);break;
            case RECV_FILE_NAME:
                         {
                             fpr = fopen(rm.context,"r");

                         }
            case RECV_FILE:
                             recv_filer(accept_fd,rm);break;
        }
    }
}
    int main()
    {
        int accept_fd;
        pthread_t tid;
        signal(SIGINT,sigfun);
        if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        {

            printf("socket fail!\n");
            exit(0);
        }

        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(SADDR);
        servaddr.sin_port = htons(PORT);

        int opt = 1;
        setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

        if(bind(socket_fd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
        {

            printf("bind fail!\n");
            exit(0);
        }
        printf("bind ok!\n");

        if(listen(socket_fd,5) == -1)
        {

            printf("listen fail!\n");
            exit(0);
        }
        printf("listen ok!\n");

        int client_len = sizeof(clieaddr);
        while(1)
        {

            printf("on\n");
            if((accept_fd = accept(socket_fd,(struct sockaddr*)&clieaddr,&client_len)) == -1)
            {

                printf("accept fail!\n");
                continue;
            }
            if(accept_fd > 0)
            {

                printf("accept ok!\n");
                pthread_create(&tid,NULL,deal_data,(void *)&accept_fd);
                printf("accept_fd = %d\n",accept_fd);
                pthread_detach(tid);
            }
            printf("pthrad\n");
        }
        close(accept_fd);
        return 0;
    }
