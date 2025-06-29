#ifndef __ALG_SWF_H
#define __ALG_SWF_H

#include "stdint.h"
#include "configure.h"

typedef struct {
    float *buffer;
    float sum;
    uint16_t window_size;
    uint16_t index;
    float shift;
} SlidingWindowFilter;

void SlidingWindowFilter_Init(SlidingWindowFilter *filter, float *buffer, uint16_t window_size);

float SlidingWindowFilter_Update(SlidingWindowFilter *filter, float new_value);

#endif // __ALG_SWF_H
