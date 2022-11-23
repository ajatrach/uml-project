//THIS IS THE SERVER SIDE
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/file.h>

#define SOCK_PATH "uml_socket.sock"
#define INPUT_SIZE 50

/*
 * Problem: the input data is sometimes split up, after arriving and printing
 * to the output file.
 * Solution: implement a queue to make uniform byte reads
*/

char input_str[INPUT_SIZE];
int len, t, s_o, re; //specifies the return of the socket function calls

int receive_socket(int client_S){
    FILE *fp;
    int n=0;
    fp=fopen("output_client.csv", "w+");
    perror("Connection");
    fprintf(fp,"Command Number\t\tCode from Client\n");

    for(;;) { //infinite loop
	n++;
	if((re=recv(client_S, input_str, INPUT_SIZE, 0)) == -1){
	    perror("Recieve");
	} else if(re == 0) { //client disconnects
	    break;
        } 

	//add pipelining code here to flush entire buffer to a file
	input_str[re] = '\0';
	fprintf(fp, "%d\t\t\t%s", n, input_str);
    }
    fclose(fp);
    return 0; 
}

int remote_socket_init(){
    static struct sockaddr_un server, remote; //sockaddr_un specifies the type of socket
    int client_s;

    if ((s_o = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
	return errno;
    } 

    perror("Opened socket");
                                                                                  
    //init members
    server.sun_family = AF_UNIX;  
    strcpy(server.sun_path, SOCK_PATH); //assigns path for socket
    unlink(server.sun_path);
    len = strlen(server.sun_path) + sizeof(server.sun_family);                                                                    
    t = sizeof(remote); //size of struct sockaddr_un                                                                                                                 
    if ((bind(s_o, (struct sockaddr*) &server, len)) == -1){ //connects sd s_o to address (&server)
	perror("Bind");
	return errno;
    }
                                                                                                         
    if ((listen(s_o, 1)) == -1){ //opens the socket for incoming connections for 1 device
	perror("Listen");
	return errno;
    }  

    if ((client_s=accept(s_o, (struct sockaddr*) &remote, &t)) == -1){ //assign sd when accept connection from client
	perror("Accept");
	return errno; 
    }
    receive_socket(client_s);
    
    printf("Closing connection\n");                                                                                           
    close(s_o); // closing main socket closes client_s too
    return 0; 
}

int main (void) {
    remote_socket_init();
    return 0;
}
