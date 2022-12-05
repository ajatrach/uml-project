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

#include <netinet/in.h> //needed for ipv6

#define INPUT_SIZE 50

FILE *fp;
char *input_str;
int len, t, s_o, re; //specifies the return of the socket function calls

int receive_socket(int client_S){
    int n=0;
    fp = fopen("output-client.csv", "a+");
    input_str = (malloc(sizeof(char)*INPUT_SIZE));
    perror("Connection");

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
    free(input_str);
    return 0; 
}

int remote_socket_init(){
    static struct sockaddr_in6 server, remote; //sockaddr_in6 specifies the type of socket
    int client_s;
    int name;

    if ((s_o = socket(AF_INET6, SOCK_STREAM, 0)) == -1){ //uml may use PF_INET6
	perror("Socket");
	return errno;
    } 

    perror("Opened socket");
                                                                                  
    //init members
    server.sin6_family = AF_INET6;
    server.sin6_flowinfo = 0;
    server.sin6_port = htons(1084); //converts port number to big endian format format/network byte order
    /* to check =  less /etc/services & ss -tulvn */
    server.sin6_addr = in6addr_any;
 
    len = sizeof(server); //size of struct sockaddr_in6                                                                                                      
    if ((bind(s_o, (struct sockaddr*) &server, len)) == -1){ //connects sd s_o to address (&server)
	perror("Bind");
	return errno;
    }
                                                                                                         
    if ((listen(s_o, 1)) == -1){ //opens the socket for incoming connections for 1 device
	perror("Listen");
	return errno;
    }  

    if ((name=getsockname(s_o, (struct sockaddr*) &server, &len)) == -1) {
	perror("Getsockname");
	return errno;
    }
    printf("sock name = %d\n", name);

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
