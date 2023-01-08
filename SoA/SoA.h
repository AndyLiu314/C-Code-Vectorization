#include "intrin.h"
#include "logger.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
using namespace std;

int SoASerial(int *values, int N)
{
  int sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum += values[i];
  }

  return sum;
}
// Assume N % VLEN == 0
// Assume VLEN is a power of 2
int SoAVector(int *values, int N) {
  // TODO
  int* sum = (int*)malloc(8*sizeof(int));

  __cs295_vec_int sum_v = _cs295_vset_int(0); 
  __cs295_vec_int x_v; // x 
  __cs295_mask maskOnes = _cs295_init_ones(VLEN);

  for (int i = 0; i < N; i = i + VLEN){
    
    _cs295_vload_int(x_v, values + i, maskOnes);

    for (int n = 0; n < 3; n++) {
      _cs295_hadd_int(x_v, x_v);
      __cs295_vec_int xtemp_v = _cs295_vset_int(0); // x 
      _cs295_interleave_int(xtemp_v, x_v);

      _cs295_vmove_int(x_v, xtemp_v, maskOnes); 
    }

    _cs295_vadd_int(sum_v, x_v, sum_v, maskOnes);  
  }

  _cs295_vstore_int(sum+0, sum_v, maskOnes); 

  int ans = *sum;
  free(sum);
  return ans;
 
}
