#include<iostream>
using namespace std;
char caesar_encrypt(char c, int key) {
	char dict[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N',
								'O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.',
								'?','0','1','2','3','4','5','6','7','8','9','!'};
	for (int i = 0; i < 41; i++) {
		if (dict[i] == c) 
			return dict[(i + key) % 41];
	}
	return c;
}
void encryptt(char* input,int key)
{
	int i=0;
	while(input[i]!='\0' and input[i]!='\n' )
	{   if(input[i]>='a' and input[i]<='z')input[i]-=('a'-'A');
        //cout<<input[i];
		input[i]=caesar_encrypt(input[i],key);
		i++;
	}cout<<endl;
	//return input;
}