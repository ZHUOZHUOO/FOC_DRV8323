#include "util_fdcan.h"

void FDCAN_InitTxHander(FDCAN_TxHeaderTypeDef* pheader, uint32_t id, uint32_t dlc,uint32_t baudrateswitch,uint32_t can_type){
    
    pheader->Identifier = id;
		if(id>=0x800) {
		pheader->IdType = FDCAN_EXTENDED_ID;
		}
		else {
		pheader->IdType =   FDCAN_STANDARD_ID;
		}
    pheader->TxFrameType =  FDCAN_DATA_FRAME;
    pheader->DataLength = dlc;
    pheader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    pheader->BitRateSwitch =  baudrateswitch;
    pheader->FDFormat = can_type;
    pheader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    pheader->MessageMarker = 0;
}

FDCAN_TxHeaderTypeDef ptxhead;

void FDCAN_SendMessageWithBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id) 
{
	FDCAN_TxHeaderTypeDef txhead;
	FDCAN_InitTxHander(&txhead,id,dlc,FDCAN_BRS_ON,FDCAN_FD_CAN);
	ptxhead = txhead;
	HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txhead, pdata);
}
	

FDCAN_TxHeaderTypeDef txhead;

void FDCAN_SendMessageWithOutBaudSwitch(FDCAN_HandleTypeDef *hfdcan,uint8_t* pdata,uint32_t dlc,uint32_t id) 
{
	FDCAN_InitTxHander(&txhead,id,dlc,FDCAN_BRS_OFF,FDCAN_CLASSIC_CAN );
	ptxhead = txhead;
	if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txhead, pdata)!=HAL_OK) {
		FDCAN_ErrorHandler();
	}
}


void FDCAN_ErrorHandler(void)
{
	while(1) {
		return;
	}
}
