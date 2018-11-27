#include<string.h>
#include<mysql/mysql.h>
#include<errno.h>
#include<pthread.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>


#define REGISTER 1
#define LOGINER 2
#define GROUP_CHATER 3
#define PRIVATE_CHATER 4
#define CHANGE_SECRETER 5
#define EXITER 6
#define GROUP_CHATING 7
#define KICKER 8
#define PRINT_NUMBERER 9
#define PRINT_PERSONER 10
#define NO_TALKER 11
#define RELEASE 12
#define SEND_FILER 13
#define FILE_NAME 14
#define MSG_DONE 15
#define MSG_EXCEPT 16
#define MSG_CONTEXT 17
#define RECV_FILER 18
#define RECV_FILE 19
#define RECV_FILE_NAME 20

struct msg
{
  int type;
  char account_number[50];
  char send_account[50];
  char recv_account[50];
  char secret[30];
  char insurance_question[50];
  char insurance_answer[50];
  char context[100];
  int flag;
  char file_name[50];
  int len;
};


typedef struct logining
{
   int accept_fd;
   char account_number[50];
}LOGINING;


typedef struct group
{
    char account_number[50];
    int is_online;
    int accept_fd_flag;
    int is_qunzhu;
    int is_no_talk;
}GROUP;

#define PORT 9999
#define SADDR "127.0.0.1"
int socket_fd;

struct sockaddr_in servaddr;
struct sockaddr_in clieaddr;
MYSQL *g_conn;                  //mysql 连接
MYSQL_RES *g_res;               //mysql记录集
MYSQL_ROW g_row;                //字符串数组，mysql记录行

#define MAX_BUF_SIZE 1024           //缓冲区最大字节数
#define SER_ADD "127.0.0.1"

const char *g_host_name = "localhost";
const char *g_user_name = "root";
const char *g_passwd = "980801T";
const char *g_db_name = "chat";
const unsigned int g_db_port = 3306;
