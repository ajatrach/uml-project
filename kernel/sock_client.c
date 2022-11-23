//THIS IS THE CLIENT SIDE
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>
#include <linux/export.h>


#define INPUT_SIZE 30
#define SOCK_PATH "uml_socket.sock"

int len;
char input_str[INPUT_SIZE];
ssize_t sock_send;
int sd;

int socket_send(char* buf, int size){
    char *c;
    perror("Connection");
    printf("Input: ");

    // fgets only stops when no input is entered
    while(fgets(input_str, INPUT_SIZE, stdin),!feof(stdin)) { 
	sock_send=send(sd, input_str, strlen(input_str), 0); //0=no flags
	if((c=strstr(input_str, "exit")) != NULL) { //closes when user types exit
	    perror("Close");
	    exit(1);
	}
	printf("Input: ");
    }

    close(sd);
    return 0;
}
//void __init is a header before methods in kernel
int __init socket_init2(void){ //param = fd
    int s,ff;
    static struct sockaddr_un remote;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){ //create socket stream for client-side
	perror("Socket");
	return errno;
    }

    if((ff=fcntl(s, F_GETFL)) == -1) {//get file access mode 
	perror("Fnctl");
	exit(1);
    }

    printf("Connecting to socket...\n");

    //initialize members and set len
    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);

/*
 * Connect is only for client side. The following call connects the socket descriptor (s)
 * to the address of the remote socket, which is stored at &remote in the server code.
 * If successful, client side socket descriptor: s and server side socket_descriptor: client_s
 * will be the same.
*/

    if (connect(s, (struct sockaddr*) &remote, len) == -1){
	perror("Connect");
	return errno;
    }

    return 0;

    //call send outside of init fuction
}

EXPORT_SYMBOL_GPL(socket_init2);


int main (void) {
    //return an int that returns from read, then open the socket, then take input, since the buffer will only be 256 bits
   // socket_init2();
    return 0;
} 
