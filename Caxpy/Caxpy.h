#include <math.h>
#include <stdio.h>
#include <algorithm>
#include "intrin.h"
#include "logger.h"

using namespace std;

void CAXPYSerial(int N, int cond[], int a, int X[], int Y[]) {
  int i;
  for (i = 0; i < N; i++) {
    if (cond[i]) Y[i] = a * X[i] + Y[i];
  }
}

// implementation of relu using instrinsics
void CAXPYVector(int N, int cond[], int a, int X[], int Y[]) {
 // TODO
  __cs295_vec_int x_v; // x 
  __cs295_vec_int y_v; // y
  __cs295_vec_int a_v = _cs295_vset_int(a);
  __cs295_vec_int cond_v;
  __cs295_vec_int ones = _cs295_vset_int(1);

  for (int i = 0; i < N; i = i+VLEN) {
    int width = VLEN; 

    if (N - i < VLEN){
      width = N - i; // used to mask remaining elements in last iteration
    } 

    __cs295_mask maskOnes = _cs295_init_ones(width);
    _cs295_vload_int(x_v, X + i, maskOnes);
    _cs295_vload_int(y_v, Y + i, maskOnes);
    _cs295_vload_int(cond_v, cond + i, maskOnes);

    
    __cs295_mask condMask; 
    _cs295_veq_int(condMask, cond_v, ones, maskOnes);

    __cs295_vec_int prod_v = _cs295_vset_int(0); 
    _cs295_vmult_int(prod_v, a_v, x_v, condMask);

    _cs295_vadd_int(y_v, prod_v, y_v, condMask); 

    __cs295_mask tailMask = _cs295_mask_and(condMask, maskOnes); 
    _cs295_vstore_int(Y + i, y_v, tailMask);

    }

  }

