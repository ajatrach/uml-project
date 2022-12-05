//THIS IS THE CLIENT SIDE
#include <stdio.h> //driver recognizes
#include <unistd.h> //driver recognizes
#include <errno.h> //driver recognizes
#include <stdlib.h> //driver recognizes
#include <strings.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>

#include <linux/in6.h> //new socket for linux
#include <netinet/in.h>

#define INPUT_SIZE 30
#define DATA_SIZE 512

int len;
char input_str[INPUT_SIZE];
long sock_send;
struct stat stat_buf;
//init global vars
int s;
static struct sockaddr_in6 remote;
int connectcheck=0;

void check_data(int fd, char *input_str){
    __uid_t uid = stat_buf.st_uid;
    int i=0;
    if(fd > 0) {
	fstat(fd, &stat_buf);
	perror("fstat"); 
	printf("fstat=%d\n",uid);
    }
}

int socket_send(int sd){
    char *c, *b;
    b=input_str;
    b=(malloc(sizeof(char)*INPUT_SIZE));
 /*                                                                                                                                  
 * Connect is only for client side. The following call connects the socket descriptor (s)                                           
 * to the address of the remote socket, which is stored at &remote in the server code.                                              
 * If successful, client side socket descriptor: s and server side socket_descriptor: client_s                                      
 * will be the same.                                                                                                                
*/                                                                                                                                  
//   if(connectcheck=0){
//    if (connect(sd, (struct sockaddr*) &remote, len) == -1){
//	perror("Connect");                                                                                                          
///        return errno; 
//    }
//	connectcheck++;    
 //  }
    
    // fgets only stops when no input is entered
//    while(fgets(input_str, INPUT_SIZE, stdin),!feof(stdin)) { 
//	if(input_str) {
//	    check_data(sd, b);
//	}
	input_str[0]='a';
	sock_send=send(sd, input_str, 1, 0); //0=no flags
//	if((c=strstr(input_str, "exit")) != NULL) { //closes when user types exit
//	    perror("Close");
//	    exit(1);
//	}
  //  }
    close(sd);
    free(b);
    return 0;
}

int socket_init(){ //param = fd
    char *c, *b;
    int ff;


    if ((s = socket(AF_INET6, SOCK_STREAM, 0)) == -1){ //create socket stream for client-side
	perror("Socket");
	return errno;
    }

    if((ff=fcntl(s, F_GETFL)) == -1) {//get file access mode 
	perror("Fnctl");
	exit(1);
    }

    printf("Connecting to socket...\n");

    //initialize members and set len
    remote.sin6_family = AF_INET6;
    remote.sin6_flowinfo = 0;
    remote.sin6_port = htons(1084);

    len = sizeof(struct sockaddr_in6);
  if (connect(s, (struct sockaddr*) &remote, len) == -1){
        perror("Connect");
        return errno;
    }
  
    return 0;
}

int main (void) {
    socket_init();
    socket_send(s);
    return 0;
} 
