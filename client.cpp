#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include "encrypt.h"
#include"decrypt.h"
#define M_ITERATION 15
#define deb(x) cout<<#x<<' '<<x<<endl;
using namespace std;
#define limit 2000
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
int compute_exp_modulo(int base,int pow,int mod)
{
long long x=1;
base%=mod;
for(int i=1;i<=pow;i++)
{
x=(x*base)%mod;
}
return x%mod;
}
int MillerRabinTest(int value, int iteration) {
  if (value < 2)
    return 0;
  int q = value - 1, k = 0;
  while (!(q % 2)) {
    q /= 2;
    k++;
  }
  for (int i = 0; i < iteration; i++) {
    int a = rand() % (value - 1) + 1;
    int current = q;
    int flag = 1;
    int mod_result = compute_exp_modulo(a, current, value);
    for (int i = 1; i <= k; i++) {
      if (mod_result == 1 || mod_result == value - 1) {
        flag = 0;
        break;
      }
      mod_result = (int)((long long)mod_result * mod_result % value);
    }
    if (flag)
      return 0;
  }
  return 1;
}
int generatePrime()
{  
  printf("* Running Miller-Rabin test to find a large prime number...\n\n");
  srand(time(NULL));
  while(1) {
    int current_value = rand() % 1000000;
    if (!(current_value % 2))
      current_value++;
    deb(current_value);
    if (MillerRabinTest(current_value, M_ITERATION) == 1)
      return current_value;
    cout<<"not prime";
  }
}
int main(int argc,char* argv[])
{
    int sockfd,portno,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
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
   bzero(buffer,limit);
   readfromsocket(sockfd,buffer);
   int G = atoi(buffer);
   deb(G);
    bzero(buffer,limit);
    readfromsocket(sockfd,buffer);
    int P = atoi(buffer);
    deb(P);
   // int privatekeyclient = generatePrime();
   // int to_server = compute_exp_modulo(G,privatekeyclient,P);
   //   char to_server_string[to_string(to_server).length()]; 
   //    strcpy(to_server_string,to_string(to_server).c_str()); 
   //    write(newsockfd,to_server_string,sizeof(to_server_string));
    
   while(1)
   {
       bzero(buffer,limit);
       fgets(buffer,limit,stdin);
       encryptt(buffer,55);
       n = write(sockfd,buffer,sizeof(buffer));
       if(n<0)perror("error in writing");
       bzero(buffer,limit);
       n = read(sockfd,buffer,limit);
       if(n<0)error("error in reding");
       printf("server(encrypted): %s",buffer);
       decryptt(buffer,55);
       printf("server(decrypted): %s",buffer);
       if(strncmp(buffer,"Bye",3)==0)break;
   }
   close(sockfd);
   return 0;
}