int generatePrime()
{  
  printf("* Running Miller-Rabin test to find a large prime number...\n\n");
  srand(time(NULL));
  while(1) {
    int current_value = rand() % 1000000;
    if (!(current_value % 2))
      current_value++;
    if (MillerRabinTest(current_value, 15) == 1)
      return current_value;
  }
}