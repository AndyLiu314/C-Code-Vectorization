#include "intrin.h"
#include "logger.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
using namespace std;

void ReluSerial(int *values, int *output, int N)
{
  for (int i = 0; i < N; i++)
  {
    int x = values[i];
    if (x < 0)
    {
      output[i] = 0;
    }
    else
    {
      output[i] = x;
    }
  }
}

// implementation of relu using instrinsics
void ReluVector(int *values, int *output, int N)
{
  // TODO
  __cs295_vec_int x_v; // x 

  __cs295_vec_int zeros = _cs295_vset_int(0); // arbitrary 0 integer variable

  for (int i = 0; i < N; i=i+VLEN){
    //loads x = values[i]
    //mask basically is a bool that is either 1 or 0 
    //if the mask is a true/1, then the operation is executed otherwise it is not
    __cs295_mask maskOnes = _cs295_init_ones(VLEN);
    _cs295_vload_int(x_v, values + i, maskOnes);

    // the if statement 
    // creates mask to find if x < 0
    __cs295_mask mask_x_zeros;
    _cs295_vlt_int(mask_x_zeros, x_v, zeros, maskOnes);

    // if x < 0 
    _cs295_vstore_int(output + i, zeros, mask_x_zeros);

    // else creates a new mask and flips it to run the else statement
     __cs295_mask mask_else = _cs295_mask_not(mask_x_zeros);
    _cs295_vstore_int(output + i, x_v, mask_else);
  }
}