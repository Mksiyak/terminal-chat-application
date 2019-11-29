char caesar_decrypt(char c, int key) {
	char dict[] = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N',
								'O','P','Q','R','S','T','U','V','W','X','Y','Z',',','.',
								'?','0','1','2','3','4','5','6','7','8','9','!'};
	for (int i = 0; i < 41; i++) {
		if (dict[i] == c) 
			return dict[((i - key)%41 + 41) % 41];
	}
	return c;
}
void decryptt(char* input,int key)
{
	int i=0;
	cout<<input<<endl;
	while(input[i]!='\0' and input[i]!='\n' )
	{   
		input[i]=caesar_decrypt(input[i],key);
		if(input[i]>='A' and input[i]<='Z')input[i]+=('a'-'A');
		i++;
	}
	cout<<input<<endl;
}