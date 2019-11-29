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