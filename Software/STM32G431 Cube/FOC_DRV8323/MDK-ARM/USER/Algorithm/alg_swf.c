#include "alg_swf.h"

void SlidingWindowFilter_Init(SlidingWindowFilter *filter, float *buffer, uint16_t window_size) {
    filter->buffer = buffer;
    filter->window_size = window_size;
    filter->index = 0;
    filter->sum = 0;
    
    filter->shift = 1.0f / (float)filter->window_size;

    for (uint16_t i = 0; i < window_size; i++) {
        filter->buffer[i] = 0;
    }
}

float SlidingWindowFilter_Update(SlidingWindowFilter *filter, float new_value) {
    filter->sum -= filter->buffer[filter->index];
    
    filter->buffer[filter->index] = new_value;
    filter->sum += new_value;

    filter->index = (filter->index + 1) & (filter->window_size - 1);
    
    return filter->sum * filter->shift;
}
