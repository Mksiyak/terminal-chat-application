#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "encrypt.h"
#include"decrypt.h"
using namespace std;
#define MAXSIZE 1000000
#define M_ITERATION 15
#define limit 2000
#define deb(x) cout<<#x<<' '<<x<<endl;
void error(const char* msg)
{
perror(msg);
exit(1);
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

int generatePrimitiveRoot(int p)
{
  int sieve[MAXSIZE];
  memset(sieve, 0, sizeof(sieve));
  sieve[0] = sieve[1] = 1;
  for (int i = 4; i < MAXSIZE; i += 2)
    sieve[i] = 1;
  for (int i = 3; i < MAXSIZE; i += 2) {
    if (!sieve[i]) {
      for (int j = 2 * i; j < MAXSIZE; j += i)
        sieve[j] = 1;
    }
  }
  while (1) {
    int a = rand() % (p - 2) + 2;
    int phi = p - 1, flag = 1, root = sqrt(phi);
    for (int i = 2; i <= root; i++) {
      if (!sieve[i] && !(phi % i)) {
        int mod_result = compute_exp_modulo(a, phi / i, p);
        if (mod_result == 1) {
          flag = 0;
          break;
        }
        if (MillerRabinTest(phi / i, M_ITERATION) && !(phi % (phi / i))) {
          int mod_result = compute_exp_modulo(a, phi / (phi / i), p);
          if (mod_result == 1) {
            flag = 0;
            break;
          }
        }
      }
    }
    if (flag) 
      return a;
  }
}
int str_to_int(char* input)
{
  int i=0,ret=0,len=0;
  while(input[i]!='\0' and input[i]!='\n' )
  {
     len++;
  }
  i=0;
  while(input[i]!='\0' and input[i]!='\n' )
  {   
    ret+=pow(10,len-i)*(int)(input[i]-'0');
    i++;
  }
  return ret;
}
int main(int argc,char* argv[])
{
    if(argc<2)
    {
        fprintf(stderr,"you need to provide port number . program terminated\n");
    }
    int sockfd,newsockfd,portno,n;
    char buffer[limit];
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t clilen;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        error("error in opening socket \n");
        exit(1);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd,(struct sockaddr*)&serv_addr,(socklen_t)sizeof(serv_addr))<0)
    {
        error("binding failed \n");
        exit(1);
    }
     int G,P;
     G = generatePrime();
     deb(G);
     P = generatePrimitiveRoot(G);
     deb(P);
     int privatekeyserver = generatePrime();
     int to_client = compute_exp_modulo(G,privatekeyserver,P);
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);
     string temp1 = to_string(G),temp2 = to_string(P);
     char GS[temp1.length()+1],PS[temp2.length()+1];
      strcpy(GS,temp1.c_str());
      strcpy(PS,temp2.c_str());
      write(newsockfd,GS,sizeof(GS));
      sleep(1);
      write(newsockfd,PS,sizeof(PS));
      bzero(buffer,limit);
      // read(newsockfd,buffer,sizeof(buffer));
      // int from_client = str_to_int(buffer);
      // char to_client_string[to_string(to_client).length()];
      // strcpy(to_client_string,to_string(to_client).c_str()); 
      // write(newsockfd,to_client_string,sizeof(to_client_string));
     while(1)
     {
         bzero(buffer,limit);
         n=read(newsockfd,buffer,sizeof(buffer));
          if(n<0)
          {
              error("error in reading");
          }
        printf("client (encrypted): %s",buffer);
        decryptt(buffer,55);
        printf("client (decrypted): %s",buffer);
        bzero(buffer,limit);
        fgets(buffer,limit,stdin);
        encryptt(buffer,55);
        n=write(newsockfd,buffer,sizeof(buffer));
        if(n<0)error("error in writing\n");
        if(strncmp(buffer,"bye",3)==0)break;
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}