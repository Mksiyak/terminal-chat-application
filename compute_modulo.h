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