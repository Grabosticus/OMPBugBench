void operate()
{
  static int num; 
  num += 1;
}

int main()
{ 
  #pragma omp parallel 
  {
     operate();
  }
  return 0;   
}
