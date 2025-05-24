#include "Foc_Comm.h"


void FDCAN_IntFilterAndStart(void) {
	FDCAN_FilterTypeDef sFilterConfig;
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = Rx_Master_ID;
	sFilterConfig.FilterID2 = 0x7FA;
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
	{
		FDCAN_ErrorHandler();
	}
	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{
		FDCAN_ErrorHandler();
	}
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		FDCAN_ErrorHandler();
	}
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		FDCAN_ErrorHandler();
	}
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_BUS_OFF, 0) != HAL_OK) 
	{ 
		FDCAN_ErrorHandler(); 
	}
}

void FOC_Comm_TxData_Encoder(uint8_t *txdata)
{
    // Encode the data to be transmitted
	txdata[0] = Motor_Error.SAFETY_STATE |
				Motor_Error.OVER_VOLTAGE_STATE << 1 |
				Motor_Error.UNDER_VOLTAGE_STATE << 2 |
				Motor_Error.OVER_CURRENT_STATE << 3 |
				Motor_Error.OVER_SPEED_STATE << 4 |
				Motor_Error.OVER_TEMPERATURE_STATE << 5 |
				Motor_Error.OVER_LOAD_STATE << 6 |
				Motor_Error.DRV8323_ERROR_STATE << 7;
    txdata[1] = *((uint8_t *)&Motor_FOC.Theta);
    txdata[2] = *((uint8_t *)&Motor_FOC.Theta + 1);
    txdata[3] = *((uint8_t *)&Motor_FOC.Theta + 2);
    txdata[4] = *((uint8_t *)&Motor_FOC.Theta + 3);
}

void FOC_Comm_Handler(uint32_t cmd, uint8_t *rxdata)
{
	if(cmd == 0x01)
	{
		// Decode the received data
		Motor_FOC.Theta_Ref = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
	    // Transmit data
		uint8_t txdata[5];
		FOC_Comm_TxData_Encoder(txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_5, Tx_Master_ID | 0x02);
	}
}


uint8_t RxData[64];
FDCAN_RxHeaderTypeDef RxHeader;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET)
	{
		if(hfdcan->Instance == FDCAN1) 
		{
			HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);
			uint32_t cmd = RxHeader.Identifier & 0x003;
			uint32_t device_id = RxHeader.Identifier & 0x7FA;
			switch(device_id) 
			{
				case Rx_Master_ID:
					FOC_Comm_Handler(cmd, RxData);
					break;
				default: break;
			}
			if( HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0)!= HAL_OK) 
			{
				FDCAN_ErrorHandler();
			}
		}
	}
}
