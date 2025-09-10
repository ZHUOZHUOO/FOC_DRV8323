#include "Foc_Comm.h"


void FDCAN_IntFilterAndStart(void) {
	FDCAN_FilterTypeDef sFilterConfig;
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = Rx_Master_ID;
	sFilterConfig.FilterID2 = 0x780;
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

void FOC_Comm_TxData_Encoder(uint32_t cmd, uint8_t *txdata)
{
    // Encode the data to be transmitted
	if(cmd == CMD_THETA_CURRENT_FEEDBACK)
	{	
		txdata[0] = *((uint8_t *)&Motor_FOC.Theta);
    	txdata[1] = *((uint8_t *)&Motor_FOC.Theta + 1);
    	txdata[2] = *((uint8_t *)&Motor_FOC.Theta + 2);
    	txdata[3] = *((uint8_t *)&Motor_FOC.Theta + 3);
		txdata[4] = *((uint8_t *)&Motor_FOC.Iq);
    	txdata[5] = *((uint8_t *)&Motor_FOC.Iq + 1);
    	txdata[6] = *((uint8_t *)&Motor_FOC.Iq + 2);
    	txdata[7] = *((uint8_t *)&Motor_FOC.Iq + 3);
	}
	else if (cmd == CMD_ERROR_FEEDBACK)
	{
		txdata[0] = Motor_Error.SAFETY_STATE |
					Motor_Error.OVER_VOLTAGE_STATE << 1 |
					Motor_Error.UNDER_VOLTAGE_STATE << 2 |
					Motor_Error.OVER_CURRENT_STATE << 3 |
					Motor_Error.OVER_SPEED_STATE << 4 |
					Motor_Error.OVER_TEMPERATURE_STATE << 5 |
					Motor_Error.DRV8323_ERROR_STATE << 6;
	}
	else if(cmd == CMD_IQ_PID_FDB)
	{
		txdata[0] = *((uint8_t *)&Current_Iq_PID.kp);
    	txdata[1] = *((uint8_t *)&Current_Iq_PID.kp + 1);
    	txdata[2] = *((uint8_t *)&Current_Iq_PID.kp + 2);
    	txdata[3] = *((uint8_t *)&Current_Iq_PID.kp + 3);
		txdata[4] = *((uint8_t *)&Current_Iq_PID.ki);
    	txdata[5] = *((uint8_t *)&Current_Iq_PID.ki + 1);
    	txdata[6] = *((uint8_t *)&Current_Iq_PID.ki + 2);
    	txdata[7] = *((uint8_t *)&Current_Iq_PID.ki + 3);
		txdata[8] = *((uint8_t *)&Current_Iq_PID.kd);
    	txdata[9] = *((uint8_t *)&Current_Iq_PID.kd + 1);
    	txdata[10] = *((uint8_t *)&Current_Iq_PID.kd + 2);
    	txdata[11] = *((uint8_t *)&Current_Iq_PID.kd + 3);
		txdata[12] = *((uint8_t *)&Current_Iq_PID.output_max);
    	txdata[13] = *((uint8_t *)&Current_Iq_PID.output_max + 1);
    	txdata[14] = *((uint8_t *)&Current_Iq_PID.output_max + 2);
    	txdata[15] = *((uint8_t *)&Current_Iq_PID.output_max + 3);
	}
	else if(cmd == CMD_ID_PID_FDB)
	{
		txdata[0] = *((uint8_t *)&Current_Id_PID.kp);
    	txdata[1] = *((uint8_t *)&Current_Id_PID.kp + 1);
    	txdata[2] = *((uint8_t *)&Current_Id_PID.kp + 2);
    	txdata[3] = *((uint8_t *)&Current_Id_PID.kp + 3);
		txdata[4] = *((uint8_t *)&Current_Id_PID.ki);
    	txdata[5] = *((uint8_t *)&Current_Id_PID.ki + 1);
    	txdata[6] = *((uint8_t *)&Current_Id_PID.ki + 2);
    	txdata[7] = *((uint8_t *)&Current_Id_PID.ki + 3);
		txdata[8] = *((uint8_t *)&Current_Id_PID.kd);
    	txdata[9] = *((uint8_t *)&Current_Id_PID.kd + 1);
    	txdata[10] = *((uint8_t *)&Current_Id_PID.kd + 2);
    	txdata[11] = *((uint8_t *)&Current_Id_PID.kd + 3);
		txdata[12] = *((uint8_t *)&Current_Id_PID.output_max);
    	txdata[13] = *((uint8_t *)&Current_Id_PID.output_max + 1);
    	txdata[14] = *((uint8_t *)&Current_Id_PID.output_max + 2);
    	txdata[15] = *((uint8_t *)&Current_Id_PID.output_max + 3);
	}
	else if(cmd == CMD_POSITION_PID_FDB)
	{
		txdata[0] = *((uint8_t *)&Position_PID.kp);
    	txdata[1] = *((uint8_t *)&Position_PID.kp + 1);
    	txdata[2] = *((uint8_t *)&Position_PID.kp + 2);
    	txdata[3] = *((uint8_t *)&Position_PID.kp + 3);
		txdata[4] = *((uint8_t *)&Position_PID.ki);
    	txdata[5] = *((uint8_t *)&Position_PID.ki + 1);
    	txdata[6] = *((uint8_t *)&Position_PID.ki + 2);
    	txdata[7] = *((uint8_t *)&Position_PID.ki + 3);
		txdata[8] = *((uint8_t *)&Position_PID.kd);
    	txdata[9] = *((uint8_t *)&Position_PID.kd + 1);
    	txdata[10] = *((uint8_t *)&Position_PID.kd + 2);
    	txdata[11] = *((uint8_t *)&Position_PID.kd + 3);
		txdata[12] = *((uint8_t *)&Position_PID.output_max);
    	txdata[13] = *((uint8_t *)&Position_PID.output_max + 1);
    	txdata[14] = *((uint8_t *)&Position_PID.output_max + 2);
    	txdata[15] = *((uint8_t *)&Position_PID.output_max + 3);
	}
}

void FOC_Comm_Handler(uint32_t cmd, uint8_t *rxdata)
{
	if(cmd == CMD_THETA_SET)
	{
		uint8_t last_mode = Motor_FOC.Motor_Close_Loop_Mode;
		if(last_mode != Position_Mode)
		{
			PID_Clear(&Position_PID);
			Motor_FOC.Motor_Close_Loop_Mode = Position_Mode;
		}
		//Motor_FOC.Theta_Ref = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
		Motor_FOC.Theta_Ref =*(float*)(rxdata);
		uint8_t txdata[8];
		FOC_Comm_TxData_Encoder(CMD_THETA_CURRENT_FEEDBACK, txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_8, Tx_Master_ID | CMD_THETA_CURRENT_FEEDBACK);
	}
	else if (cmd == CMD_CURRENT_SET)
	{
		uint8_t last_mode = Motor_FOC.Motor_Close_Loop_Mode;
		if(last_mode != Force_Mode)
		{
			//don't Clear Iq PID !!
			Motor_FOC.Motor_Close_Loop_Mode = Force_Mode;
		}
		//Motor_FOC.Iq_ref = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
		Motor_FOC.Iq_ref =*(float*)(rxdata);
		uint8_t txdata[8];
		FOC_Comm_TxData_Encoder(CMD_THETA_CURRENT_FEEDBACK, txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_8, Tx_Master_ID | CMD_THETA_CURRENT_FEEDBACK);
	}
	else if(cmd == CMD_IQ_PID_SET)
	{
		uint8_t last_mode = Motor_FOC.Motor_Close_Loop_Mode;
		if(last_mode != Force_Mode)
		{
			//don't Clear Iq PID !!
			Motor_FOC.Motor_Close_Loop_Mode = Force_Mode;
		}
		// Current_Iq_PID.kp = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
		// Current_Iq_PID.ki = (float)(rxdata[4] | (rxdata[5] << 8) | (rxdata[6] << 16) | (rxdata[7] << 24));
		// Current_Iq_PID.kd = (float)(rxdata[8] | (rxdata[9] << 8) | (rxdata[10] << 16) | (rxdata[11] << 24));
		// Current_Iq_PID.sum_max = (float)(rxdata[12] | (rxdata[13] << 8) | (rxdata[14] << 16) | (rxdata[15] << 24));
		// Current_Iq_PID.output_max = (float)(rxdata[16] | (rxdata[17] << 8) | (rxdata[18] << 16) | (rxdata[19] << 24));
		Current_Iq_PID.kp = *(float*)(rxdata);
		Current_Iq_PID.ki = *(float*)(rxdata + 4);
		Current_Iq_PID.kd = *(float*)(rxdata + 8);
		Current_Iq_PID.sum_max = *(float*)(rxdata + 12);
		Current_Iq_PID.output_max = *(float*)(rxdata + 16);
		uint8_t txdata[16];
		FOC_Comm_TxData_Encoder(CMD_IQ_PID_FDB, txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_16, Tx_Master_ID | CMD_IQ_PID_FDB);
	}
	else if(cmd == CMD_ID_PID_SET)
	{
		uint8_t last_mode = Motor_FOC.Motor_Close_Loop_Mode;
		if(last_mode != Force_Mode)
		{
			//don't Clear Iq PID !!
			Motor_FOC.Motor_Close_Loop_Mode = Force_Mode;
		}
		// Current_Id_PID.kp = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
		// Current_Id_PID.ki = (float)(rxdata[4] | (rxdata[5] << 8) | (rxdata[6] << 16) | (rxdata[7] << 24));
		// Current_Id_PID.kd = (float)(rxdata[8] | (rxdata[9] << 8) | (rxdata[10] << 16) | (rxdata[11] << 24));
		// Current_Id_PID.sum_max = (float)(rxdata[12] | (rxdata[13] << 8) | (rxdata[14] << 16) | (rxdata[15] << 24));
		// Current_Id_PID.output_max = (float)(rxdata[16] | (rxdata[17] << 8) | (rxdata[18] << 16) | (rxdata[19] << 24));
		Current_Id_PID.kp = *(float*)(rxdata);
		Current_Id_PID.ki = *(float*)(rxdata + 4);
		Current_Id_PID.kd = *(float*)(rxdata + 8);
		Current_Id_PID.sum_max = *(float*)(rxdata + 12);
		Current_Id_PID.output_max = *(float*)(rxdata + 16);
		uint8_t txdata[16];
		FOC_Comm_TxData_Encoder(CMD_ID_PID_FDB, txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_16, Tx_Master_ID | CMD_ID_PID_FDB);
	}
	else if(cmd == CMD_POSITION_PID_SET)
	{
		uint8_t last_mode = Motor_FOC.Motor_Close_Loop_Mode;
		if(last_mode != Position_Mode)
		{
			PID_Clear(&Position_PID);
			Motor_FOC.Motor_Close_Loop_Mode = Position_Mode;
		}
			// Position_PID.kp = (float)(rxdata[0] | (rxdata[1] << 8) | (rxdata[2] << 16) | (rxdata[3] << 24));
			// Position_PID.ki = (float)(rxdata[4] | (rxdata[5] << 8) | (rxdata[6] << 16) | (rxdata[7] << 24));
			// Position_PID.kd = (float)(rxdata[8] | (rxdata[9] << 8) | (rxdata[10] << 16) | (rxdata[11] << 24));
			// Position_PID.sum_max = (float)(rxdata[12] | (rxdata[13] << 8) | (rxdata[14] << 16) | (rxdata[15] << 24));
			// Position_PID.output_max = (float)(rxdata[16] | (rxdata[17] << 8) | (rxdata[18] << 16) | (rxdata[19] << 24));
		Position_PID.kp = *(float*)(rxdata);
		Position_PID.ki = *(float*)(rxdata + 4);
		Position_PID.kd = *(float*)(rxdata + 8);
		Position_PID.sum_max = *(float*)(rxdata + 12);
		Position_PID.output_max = *(float*)(rxdata + 16);
		uint8_t txdata[16];
		FOC_Comm_TxData_Encoder(CMD_POSITION_PID_FDB, txdata);
		FDCAN_SendMessageWithBaudSwitch(&hfdcan1, txdata, FDCAN_DLC_BYTES_16, Tx_Master_ID | CMD_POSITION_PID_FDB);
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
			uint32_t cmd = RxHeader.Identifier & CMD_MASK;
			uint32_t device_id = RxHeader.Identifier & DEVICE_ID_MASK;
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
