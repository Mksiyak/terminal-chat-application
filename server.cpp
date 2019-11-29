#include <bits/stdc++.h>
#include<stdio.h>
#include <string.h>     
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>  
#include <arpa/inet.h> 
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h>
#define MAXSIZE 1000000
#define M_ITERATION 15
#include "compute_modulo.h"
#include "encrypt.h"
#include "decrypt.h"
#include "millerrabin.h"
#include "generate_prime.h"
#include "primitive_root.h"
#define deb(x) cout<<#x<<' '<<x<<endl;
#define PORT 8888  
using namespace std;
int get_client_no(string s)
{
	int i=0,n,ret=0;
	while(s[i]!=' ')i++;n=--i;
    while(i>=0){ret+=(int)(s[i]-'0')*pow(10,n-i);i--;}
    return ret;
}
string get_message(string s)
{
	int i=0;
    string ret="";
    while(s[i]!=' ')i++;
    while(s[i]!='\0'){ret+=s[i];i++;}
    return ret;
}
char* get_string(char* buffer,int n)
{
    string strn = to_string(n);
    for(int i=0;i<strn.length();i++)
    {
        buffer[i] = strn[i];
    }
}
int main(int argc , char *argv[])   
{   
    int opt = 1;

 queue<string>message_queue[30];

 queue<int>encryption_keys[30];

int master_socket , addrlen , new_socket , client_socket[30], keys[30] , max_clients = 30 , activity, i , valread , sd , max_sd;

int P = generatePrime();
deb(P);

int Q = generator(P);
deb(Q);

struct sockaddr_in address;   
         
    char buffer[1025],buffer1[30];   
         
    fd_set readfds;   
         
  char* message = "Welcome to our chat application";
  for(i=0;i<max_clients;i++)
  {
      client_socket[i]=0;
  }
  if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("[-]socket failed");   
        exit(EXIT_FAILURE);   
    }
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");
        exit(EXIT_FAILURE); 
    }

    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    addrlen = sizeof(address);   
    puts("Waiting for connections ...");   
         
    while(1)
    {
        FD_ZERO(&readfds);   
     
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
        
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            if(message_queue[i].size()!=0 and client_socket[i]!=0)
            	{string temp = message_queue[i].front();
            			                char temp_char[temp.length()+1];
            			                strcpy(temp_char,temp.c_str()); 
                                        encryptt(temp_char,keys[i]);
            		            		write(client_socket[i],temp_char,sizeof(temp_char));
                                   
                                       cout<<"msg sent to "<<i<<endl;

            		            		message_queue[i].pop();
            	}

        } 
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            sd = client_socket[i];   
                 
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
                  (address.sin_port));   
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )   
            {   
                perror("send");   
            }   
            puts("Welcome message sent successfully");   
            read(new_socket,buffer,sizeof(buffer));
             int no = atoi(buffer);  
            cout<<"set client on id "<<no<<endl;
            client_socket[no]=new_socket;
            memset(buffer,'\0',1024);
            write(new_socket,get_string(buffer,P),sizeof(buffer));
            deb(buffer);memset(buffer,'\0',sizeof(buffer));
            sleep(1);
            write(new_socket,get_string(buffer,Q),sizeof(buffer));
            deb(buffer);memset(buffer,'\0',sizeof(buffer));
            int A = rand()%MAXSIZE;
            read(new_socket,buffer,sizeof(buffer));
            int GOLD = atoi(buffer);
            deb(buffer);memset(buffer,'\0',sizeof(buffer));
            write(new_socket,get_string(buffer,compute_exp_modulo(Q,A,P)),sizeof(buffer));
            GOLD = compute_exp_modulo(GOLD,A,P);
            deb(GOLD);memset(buffer,'\0',sizeof(buffer));
            keys[no]=GOLD;
        }
        for (i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                if ((valread = read( sd , buffer, 1024)) == 0)   
                {  
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);   
                    printf("Host disconnected , ip %s , port %d \n" ,  
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));   
                         
                    close( sd );   
                    client_socket[i] = 0;   
                }   
                    
                else 
                { 
                	if((int)buffer[0]==0)continue;
                	deb(buffer);
                    decryptt(buffer,keys[i]);
                    deb(buffer);sleep(10);
                	cout<<"data from client "<<i<<" to "<<get_client_no(buffer)<<" : "<<get_message(buffer)<<endl;
                	message_queue[get_client_no(buffer)].push(get_message(buffer));
                    memset(buffer,'\0',1024); 
                }
            }   
        }
          
    }      
             
    }

