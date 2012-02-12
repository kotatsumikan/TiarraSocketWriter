#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

char op_sock[108]  = {0};
char op_sender[32] = {0};
int  op_notice     = 0;
char op_msg[512]   = {0};
char op_ch[64]     = {0};

int setparm(int argc, char **args){
    int i=0;
    int flag=0;

    for(i=0; i<argc; i++){
        if(strcmp("-socket", args[i]) == 0){
            i++;
            if(args[i] == NULL)
                continue;
            strncpy(op_sock, args[i], (sizeof(op_sock)-1));
        }else if(strcmp("-sender", args[i]) == 0){
            i++;
            if(args[i] == NULL)
                continue;
            strncpy(op_sender, args[i], (sizeof(op_sender)-1));
        }else if(strcmp("-notice", args[i]) == 0){
            op_notice = 1;
        }else if(strcmp("-message", args[i]) == 0){
            i++;
            if(args[i] == NULL)
                continue;
            strncpy(op_msg, args[i], (sizeof(op_msg)-1));
        }else if(strcmp("-channel", args[i]) == 0){
            i++;
            if(args[i] == NULL)
                continue;
            strncpy(op_ch, args[i], (sizeof(op_ch)-1));
        }            
    }

    return 0;
}

int main(int argc, char* args[]){
    int sd, len;
    int rtn = 0;
    char buf[1024]={0};
    struct sockaddr_un saddr;
    char is_notice[4] = {0};

    rtn = setparm(argc, args);

    sd = socket(AF_UNIX, SOCK_STREAM, 0);
    perror("socket ");
    saddr.sun_family = AF_UNIX;

    // set tiarra socket path
    strcpy(saddr.sun_path, op_sock);

    len = sizeof(saddr);
    rtn = connect(sd, (struct sockaddr*)&saddr, len);
    perror("connect ");

    // set notice
    if(op_notice == 1){
        strcpy(is_notice, "yes");
    }else{
        strcpy(is_notice, "no");
    }
    sprintf(buf, "NOTIFY System::SendMessage TIARRACONTROL/1.0\r\n\
Sender: \r\n\
Notice: %s\r\n\
Channel: %s\r\n\
Charset: utf-8\r\n\
Text: %s\r\n\r\n", is_notice, op_ch, op_msg);

    write(sd, buf, strlen(buf));
    perror("write ");

    // fprintf(stdout, "%s\n", buf);

    close(sd);
}
