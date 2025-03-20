/*
 * @Project: Infantry Code
 *
 * @Author: GDDG08
 * @Date: 2021-12-31 17:37:14
 * @LastEditors: Hatrix
 * @LastEditTime: 2024-01-05 17:37:20
 */

#include "util_fdcan.h"

/**
 * @brief      Initialize fdcan transmitter
 * @param      pheader: Pointer to the initialized header
 * @param      id: FDCAN Equipment number
 * @param      dlc: FDCAN Datalength
 * @param      baudrateswitch: Choose if use bandrateswitch function
 * @param      can_type: Choose use classis can or fdcan
 * @retval     NULL
 */
void FDCAN_InitTxHeader(FDCAN_TxHeaderTypeDef *pheader, uint32_t id, uint32_t dlc, uint32_t baudrateswitch, uint32_t can_type)
{
    pheader->Identifier = id;
    if (id >= 0x800) 
    {
        pheader->IdType = FDCAN_EXTENDED_ID;
    }
    else 
    {
        pheader->IdType = FDCAN_STANDARD_ID;
    }
    pheader->TxFrameType = FDCAN_DATA_FRAME;
    pheader->DataLength = dlc;
    pheader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    pheader->BitRateSwitch = baudrateswitch;
    pheader->FDFormat = can_type;
    pheader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    pheader->MessageMarker = 0;
}

/**
 * @brief      Initialize fdcan filter and enable FDCAN Bus Transceiver
 * @param      phfdcan: Pointer to the CAN header
 * @retval     NULL
 */
void FDCAN_IntFilterAndStart(FDCAN_HandleTypeDef *phfdcan)
{
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x0400;
    sFilterConfig.FilterID2 = 0x0000;

    uint32_t ret;
    if (HAL_FDCAN_ConfigFilter(phfdcan, &sFilterConfig) != HAL_OK) 
    {
        FDCAN_ErrorHandler(ret);
    }
    if (HAL_FDCAN_ConfigGlobalFilter(phfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
    {
        FDCAN_ErrorHandler(ret);
    } 
    if (HAL_FDCAN_Start(phfdcan) != HAL_OK) 
    {
        FDCAN_ErrorHandler(ret);
    }
    if (HAL_FDCAN_ActivateNotification(phfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) 
    {
        FDCAN_ErrorHandler(ret);
    }
    if (HAL_FDCAN_ActivateNotification(phfdcan, FDCAN_IT_BUS_OFF, 0) != HAL_OK) 
    {
        FDCAN_ErrorHandler(ret);
    }
}

/**
 * @brief      Sending information to can bus
 * @param      phfdcan: Pointer to the CAN header
 * @param      pheader: Pointer to the CAN tx header
 * @param      txdata: Message to send
 * @retval     NULL
 */
void FDCAN_Send(FDCAN_HandleTypeDef *phfdcan, FDCAN_TxHeaderTypeDef *ptxhead, uint8_t *pdata)
{
    uint32_t ret;
    if (HAL_FDCAN_AddMessageToTxFifoQ(phfdcan, ptxhead, pdata) != HAL_OK) 
    {
        FDCAN_ErrorHandler(ret);
    }
}

/**
 * @brief      FDCAN Error handle handling
 * @retval     NULL
 */
void FDCAN_ErrorHandler(uint32_t ret)
{
    while (1)
    {;}
}
