#include "spi_util.h"

MT6816_SPI_Struct mt6816_spi={0, 0, false, false};

void SPI_Init(void)
{
    //初始化SPI1
    MX_SPI1_Init();
    HAL_SPI_Init(&hspi1);
}

void MT6816_SPI_CS_L(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
}

void MT6816_SPI_CS_H(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

void MT6816_SPI_Get_AngleData(void)
{
    uint16_t data_t[2];  //SPI发送数据
    uint16_t data_r[2];  //SPI接收数据，即角度数据
    uint8_t h_count;  //奇偶校验计数用

    data_t[0] = 0x8300;  //根据MT6816 Datasheet, 0x8300读取0x03寄存器数据
    data_t[1] = 0x8400;  //同上

    for(uint8_t i=0; i<3; i++)
    {
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[0], (uint8_t *)&data_r[0], 1, HAL_MAX_DELAY);
        MT6816_SPI_CS_H();
        MT6816_SPI_CS_L();
        HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&data_t[1], (uint8_t *)&data_r[1], 1, HAL_MAX_DELAY);
        MT6816_SPI_CS_H();

        mt6816_spi.sample_data = ((data_r[0] & 0xFF) << 8 | (data_r[1]) & 0xFF);  //16bits SPI接收数据

        //以下为奇偶校验
        h_count = 0;
        for(uint8_t j=0; j<16; j++)
        {
            if(mt6816_spi.sample_data & (0x0001 << j))
                h_count++;
        }
        if(h_count & 0x01)
        {
            mt6816_spi.pc_flag = false;
        }
        else{
            mt6816_spi.pc_flag = true;
            break;  //奇偶校验结束
        }

    }
    if(mt6816_spi.pc_flag)
    {
        mt6816_spi.angle = mt6816_spi.sample_data >> 2;  //前14bits 角度数据
        mt6816_spi.no_mag_flag = (bool) (mt6816_spi.sample_data & 0x0010);  //弱磁标志位，根据datasheet
    }
}