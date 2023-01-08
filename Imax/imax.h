#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include "intrin.h"
#include "logger.h"

using namespace std;

void imaxSerial(int *values, int *output, int N) {
  int x = 0xffffffff;
  int index = 0;
  for (int i = 0; i < N; i++) {
    if (values[i] > x) {
      x = values[i];
      index = i;
    }
  }
  *output = index;
}

// implementation of imax using instrinsics
void imaxVector(int *values, int *output, int N) {
  __cs295_vec_int x_v;
  __cs295_vec_int max_v = _cs295_vset_int(0);
  __cs295_vec_int index_v = _cs295_vset_int(0);
  int vlen_index = 0;
  
  for (int i = 0; i < N; i = i+VLEN){
    int width = VLEN;
    
    if (N - i < VLEN){
      width = N - i; 
    }

    __cs295_mask maskOnes = _cs295_init_ones(width);
    __cs295_mask maxMask = _cs295_init_ones(0);
    _cs295_vload_int(x_v, values + i, maskOnes);

    for ( int j = 0; j < width; j++){
      
      _cs295_vgt_int(maxMask, x_v, max_v, maskOnes);
      vlen_index = _cs295_firstbit(maxMask) + i;
      
      if (_cs295_cntbits(maxMask) > 0){
        max_v = _cs295_vset_int(values[vlen_index]);
        index_v = _cs295_vset_int(vlen_index);
      }
      
      if (_cs295_cntbits(maxMask) == 1){
        break; // found the max of the VLEN of elements
      } else if (_cs295_cntbits(maxMask) == 0){
        break; // no max in the loop
      }
    }
  }
  __cs295_mask maskStore = _cs295_init_ones(1); 
  _cs295_vstore_int(output, index_v, maskStore);
}