#ifndef __SPI_UTIL_H
#define __SPI_UTIL_H

#include "stm32g4xx_hal.h"
#include "spi.h"
#include "stdbool.h"

typedef struct
{
    uint16_t sample_data;
    uint16_t angle;
    bool pc_flag;
    bool no_mag_flag;
} MT6816_SPI_Struct;

extern MT6816_SPI_Struct mt6816_spi;

void SPI_Init(void);
void MT6816_SPI_Get_AngleData(void);

#endif
