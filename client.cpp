#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define MAXSIZE 1000000
#define M_ITERATION 15
#include "compute_modulo.h"
#include "encrypt.h"
#include "decrypt.h"
#include "millerrabin.h"
#include "generate_prime.h"
#define deb(x) cout<<#x<<' '<<x<<endl;
using namespace std;
#define limit 1024
void error(const char* msg)
{
perror(msg);
exit(1);
}
void readfromsocket(int sockfd,char buffer[])
{
   int n =read(sockfd,buffer,limit);
   if(n<0)error("error in reding");
}
char* get_string(char* buffer,int n)
{
    string strn = to_string(n);
    for(int i=0;i<strn.length();i++)
    {
        buffer[i] = strn[i];
    }
}
int main(int argc,char* argv[])
{
    int sockfd,portno,n,activity,max_sd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
        fd_set readfds;   
    if(argc<2)
    {
        fprintf(stderr,"you need to provide port number program terminated\n");
        exit(1);
    }
    char buffer[limit];
    portno=atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
   error("error in opening socket");
   server = gethostbyname(argv[1]);
   if(server==NULL)
   fprintf(stderr,"No such host");
   bzero((char *)&serv_addr,sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char* )server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
   serv_addr.sin_port = htons(portno);
   if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
   error("connection failed ");
  read(sockfd,buffer,limit);
  cout<<buffer;
   cout<<"\nenter id : ";
   cin>>buffer;
   write(sockfd,buffer,sizeof(buffer));
   max_sd = sockfd;
   memset(buffer,'\0',sizeof(buffer));
   read(sockfd,buffer,sizeof(buffer));
   int P = atoi(buffer);deb(P);memset(buffer,'\0',sizeof(buffer));
   read(sockfd,buffer,sizeof(buffer));
   read(sockfd,buffer,sizeof(buffer));
   deb(buffer);
   int Q = atoi(buffer);deb(Q);memset(buffer,'\0',sizeof(buffer));
   int B = rand()%MAXSIZE;
   write(sockfd,get_string(buffer,compute_exp_modulo(Q,B,P)),sizeof(buffer));
   memset(buffer,'\0',sizeof(buffer));
   read(sockfd,buffer,sizeof(buffer));
   read(sockfd,buffer,sizeof(buffer));
   int GOLD = atoi(buffer);deb(GOLD);
   memset(buffer,'\0',sizeof(buffer));
   GOLD = compute_exp_modulo(GOLD,B,P);
   deb(GOLD);
   fgets(buffer,limit,stdin);
   while(1)
   {   
       FD_ZERO(&readfds);
       FD_SET(0, &readfds);
       FD_SET(sockfd, &readfds);
       activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
      if(FD_ISSET(0,&readfds))
      {
      bzero(buffer,limit);
      fgets(buffer,limit,stdin);
      deb(buffer);encryptt(buffer,GOLD);
      deb(buffer);
      n = write(sockfd,buffer,sizeof(buffer));
      if(n<0)perror("error in writing");
      }
      if(FD_ISSET(sockfd, &readfds))
      {
       bzero(buffer,limit);
       n = read(sockfd,buffer,limit);
       cout<<"Encryted : "<<buffer;
       decryptt(buffer,GOLD);
       if(n<0)error("error in reding");
       printf("server : %s",buffer);
      }
   }
   close(sockfd);
   return 0;
}