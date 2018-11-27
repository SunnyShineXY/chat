#include"./head.h"

struct msg rm,sm;
//sm = (struct msg*)malloc(2048);

//rm=(struct msg*)malloc(2048);



void sigfun()
{

    close(socket_fd);
    exit(0);
}

void update()
{
    sm.type = FILE_NAME;
    printf("please input filename:\n");
    scanf("%s",sm.context);
    send(socket_fd,&sm,sizeof(sm),0);

    FILE *fp;
    if((fp = fopen(sm.context,"r")) == NULL)
    {
        printf("open file fail!\n");
        exit(0);
    }

    fseek(fp,0,SEEK_SET);


    int op = 1;
    while(op)
    {memset(sm.context,0,1024);
        sm.type = SEND_FILER;
        fread(sm.context,sizeof(char),1023,fp);
        send(socket_fd,&sm,sizeof(sm),0);
        sleep(3);
        if(feof(fp))
        {
            sm.type = MSG_DONE;
            op = 0;
            send(socket_fd,&sm,sizeof(sm),0);
        }
    }
    fclose(fp);
    printf("close file ok!\n");
}

void download()
{
    sm.type = RECV_FILER;
    send(socket_fd,&sm,sizeof(sm),0);
}

void send_file()
{
    sleep(2);
    system("clear");
    printf("-------------------1.上传-------------------\n");
    printf("-------------------2.下载-------------------\n");
    printf("-------------------3.退出-------------------\n");

    int perf;
    printf("Do what?\n");
    scanf("%d",&perf);

    switch(perf)
    {
        case 1:update();break;
        case 2:download();break;
        case 3:return ;
    }
}










void regist()
{
    sm.type = REGISTER;

    int op = 1;

    system("clear");
    while(op)
    {
        printf("welcome to chat , please register!\n");
        printf("please input your account_number:\n");
        scanf("%s",sm.account_number);
        printf("please input your secret:\n");
        scanf("%s",sm.secret);
        printf("please input your insurance_question:\n");
        scanf("%s",sm.insurance_question);
        printf("please input your insurance_answer:\n");
        scanf("%s",sm.insurance_answer);

        send(socket_fd,&sm,sizeof(sm),0);

        int recv_len;
        int flag;
        recv_len = read(socket_fd,&flag,sizeof(flag));
        if(recv_len > 0 && flag == 1)
        {

            printf("welcome to join chat!\n");
            op = 0;
        }
        else if(recv_len > 0 && flag == 0)
        {

            printf("Register fail!Try again!\n");
        }
        else if(recv_len <= 0)
        {

            printf("recv fail!\n");
            op = 0;
        }
    }
}

void print_person()
{
    sm.type = PRINT_PERSONER;
    if(send(socket_fd,&sm,sizeof(sm),0) < 0)
    {
        printf("send fail!\n");
        exit(0);
    }
}

void print_number()
{
    sm.type = PRINT_NUMBERER;
    if(send(socket_fd,&sm,sizeof(sm),0) < 0)
    {
        printf("send fail!\n");
        exit(0);
    }

}

void group_chat()
{
    sm.type = GROUP_CHATER;
    sm.context[0] = '\0';
    printf("please input your name:");
    scanf("%s",sm.send_account);
    getchar();
    system("clear");
    while(strcmp(sm.context,"end") != 0)
    {
        printf("input :\n");
        fgets(sm.context,50,stdin);
        sm.context[strlen(sm.context)-1] = '\0';
        if(send(socket_fd,&sm,sizeof(sm),0) < 0)
        {
            printf("sede fail!\n");
            exit(0);
        }
    }

}	
void group_chating()
{
    while(1)
    {
        system("clear");


        printf("-------------1.群聊---------------\n");
        printf("-------------2.踢人---------------\n");
        printf("-------------3.禁言---------------\n");
        printf("-------------4.解除禁言-----------\n");
        printf("-------------5.打印在线人数-------\n");
        printf("-------------6.显示在线-----------\n");
        printf("-------------7.退出---------------\n");

        int perf;
        printf("Do what?");
        scanf("%d",&perf);
        switch(perf)
        {
            case 1:group_chat();break;
            case 2:
                   {
                       char account[50];
                       sm.type = KICKER;
                       printf("please input who do you want to kick:\n");
                       scanf("%s",sm.account_number);
                       if(send(socket_fd,&sm,sizeof(sm),0) < 0)
                       {
                           printf("send fail!\n");
                       }
                       break;
                   }
            case 3:
                   {
                       char account[50];
                       sm.type = NO_TALKER;
                       printf("please input who do you want to jinyan:\n");
                       scanf("%s",sm.account_number);
                       if(send(socket_fd,&sm,sizeof(sm),0) < 0)
                       {
                           printf("send fail!\n");
                       }
                       break;
                   }
            case 4:
                   {
                       char account[50];
                       sm.type = RELEASE;
                       printf("please input who do you want to jiechujinyan:\n");
                       scanf("%s",sm.account_number);
                       if(send(socket_fd,&sm,sizeof(sm),0) < 0)
                       {
                           printf("send fail!\n");
                       }
                       break;
                   }

            case 5:print_number();break;
            case 6:print_person();break;
            case 7:return;break;
        }
        sleep(1);
    }
}

void *deal_data()
{

    char account[50];

    while(1)
    {

        if(recv(socket_fd,&rm,sizeof(rm),0) < 0)
        {

            printf("recv fail!\n");
            exit(0);
        }
        printf("%d- type\nflag-%d\n",rm.type,rm.flag);
        if(rm.type == KICKER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == PRINT_NUMBERER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == PRINT_PERSONER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == NO_TALKER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == RELEASE)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == PRIVATE_CHATER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == GROUP_CHATER)
        {
            printf("%s\n",rm.context);
        }
        if( rm.type == SEND_FILER)
        {
            printf("%s\n",rm.context);
        }

        FILE *fp;
        if( rm.type == RECV_FILER)
        {
            printf("%s\n",rm.context);
            printf("please input filename:\n");
            sm.type = RECV_FILE_NAME;
            scanf("%s",sm.context);
            fp = fopen(sm.context,"w");
            send(socket_fd,&sm,sizeof(sm),0);
        }

        if( rm.type == RECV_FILE)
        {
            printf("%s",rm.context);
            printf("hello     ....\n");
            if((fprintf(fp,"%s",rm.context)) <= 0)
            {
                printf("zhuijiashibai\n");
            }
        }
        if(rm.type == MSG_DONE)
        {
            fclose(fp);
            printf("close ok!\n");
            printf("download ok!\n");
        }

        //      fclose(fp);
    }
}

void private_chat()
{
    sm.type = PRIVATE_CHATER;
    sm.context[0] = '\0';
    printf("please input your_name:\n");
    scanf("%s",sm.send_account);
    printf("please input send_name:\n");
    scanf("%s",sm.recv_account);
    getchar();
    system("clear");
    while(strcmp(sm.context,"end") != 0)
    {
        printf("input :\n");
        fgets(sm.context,50,stdin);
        sm.context[strlen(sm.context)-1] = '\0';
        if(send(socket_fd,&sm,sizeof(sm),0) < 0)
        {
            printf("sede fail!\n");
            exit(0);
        }
    }
}

int login()
{
    sm.type = LOGINER;
    pthread_t tid;
    int flag;
    int op = 1;

    while(op)
    {

        printf("welcome to chat , please login!\n");
        printf("please input your account_number:\n");
        scanf("%s",sm.account_number);
        printf("please input your secret:\n");
        scanf("%s",sm.secret);

        int recv_len;
        recv_len = send(socket_fd,&sm,sizeof(sm),0);
        recv_len = recv(socket_fd,&flag,sizeof(int),0);
        if(recv_len > 0 && flag == 1)
        {

            printf("Let's chat!\n");
            pthread_create(&tid,NULL,deal_data,NULL);
            op = 0;
        }
        else if(recv_len > 0 && flag == 0)
        {

            printf("Login fail!Try again!\n");
        }
        else if(recv_len <= 0)
        {

            printf("recv fail!\n");
            op = 0;
        }
    }
    while(1){

        system("clear");
        int perf;
        printf("----------1.聊天室----------\n");
        printf("----------2.私聊------------\n");
        printf("----------3.发送文件--------\n");
        printf("----------4.退出------------\n");
        scanf("%d",&perf);
        switch(perf)
        {

            case 1:
                {
                    sm.type = GROUP_CHATING;
                    printf("please input your account_number:\n");
                    scanf("%s",sm.account_number);
                    if(send(socket_fd,&sm,sizeof(sm),0) < 0)
                    {
                        printf("send fail!\n");
                        exit(0);
                    }
                    group_chating();break;
                }
            case 2:private_chat();break;
            case 3:send_file();break;
            case 4:return ;
        }
    }
}

void exit_module()
{

    sm.type = EXITER;
    if(send(socket_fd,&sm,sizeof(sm),0) < 0)
    {
        printf("send fail!\n");
        exit(0);
    }
    system("clear");
    printf("\n\n\n\n\n");
    printf("Have a good day!\n");
    printf("\n\n\n\n\n");
    exit(0);
}

void change_secret()
{
    sm.type = CHANGE_SECRETER;
    int op = 1;
    system("clear");
    printf("welcome to change your secret!\n");
    int len;

    while(op)
    {
        printf("please input your account_number:\n");
        scanf("%s",sm.account_number);
        printf("please input your new secret!\n");
        scanf("%s",sm.secret);
        printf("please input your answer!\n");
        scanf("%s",sm.insurance_answer);
        send(socket_fd,&sm,sizeof(sm),0);
        int flag;
        len = recv(socket_fd,&flag,sizeof(flag),0);
        if(len > 0 && flag == 1)
        {
            printf("change ok!\n");
            op = 0;
        }
        else if(len > 0 && flag == 0)
        {

            printf("error answer!Try again!\n");
        }
        else if(len < 0)
        {

            printf("recv fail!\n");
            exit(0);
        }

    }
}


int main()
{
    int len,recv_len;
    int perf;
    signal(SIGINT,sigfun);
    if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {

        printf("socket fail!\n");
        exit(0);
    }
    memset(&servaddr,0,sizeof(clieaddr));
    clieaddr.sin_family = AF_INET;
    clieaddr.sin_port = htons(PORT);
    clieaddr.sin_addr.s_addr = inet_addr(SADDR);

    if((connect(socket_fd,(struct sockaddr*)&clieaddr,sizeof(clieaddr)) == -1))
    {

        printf("connect fail!\n");
        exit(0);
    }

    printf("---------welcome come to perfect_class_chat！！！---------\n");
    while(1)
    {
        system("clear");

        printf("----------1.注册----------------\n");
        printf("----------2.登录----------------\n");
        printf("----------3.修改密码------------\n");
        printf("----------4.退出----------------\n");
        scanf("%d",&perf);
        switch(perf)
        {

            case 1:regist();break;
            case 2:login();break;
            case 4:exit_module();break;
            case 3:change_secret();break;
        }
    }
}



