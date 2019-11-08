#include "sensor.h"
#include <stdint.h>
#include <string.h>


uint16_t GetAdcValue(SensorStruct *ptr)
{
  uint16_t value=0;
  HAL_ADC_Start(ptr->hAdc);
  	if (HAL_ADC_PollForConversion(ptr->hAdc, 100) == HAL_OK)
  		{
  		  value=HAL_ADC_GetValue(ptr->hAdc);
  		}
  	HAL_Delay (10);
  	HAL_ADC_Stop (ptr->hAdc);
  	return value;
}

void SensorInit(SensorStruct *ptr, ADC_HandleTypeDef *hAdc, uint16_t *DisplayPins)
{
  ptr->hAdc=hAdc;
  memcpy(ptr->Display,DisplayPins,sizeof(DisplayPins)*4);
  ptr->CallibrationSamples=100;

}

void GetSensorMeasurements(SensorStruct *ptr)
{
		  ptr->CurrentValue=GetAdcValue(ptr);
}

void EvaluateSensor(SensorStruct *ptr)
{
	GetSensorMeasurements(ptr);
	if(ptr->CurrentValue <= ptr->CalibrationValue + ptr->AirLevels[0]) {ptr->Quality=Green;}else

		if(ptr->CurrentValue >= ptr->CalibrationValue + ptr->AirLevels[1] && ptr->CurrentValue <  ptr->CalibrationValue + ptr->AirLevels[2]) {ptr->Quality=Blue;}else

			if(ptr->CurrentValue >= ptr->CalibrationValue + ptr->AirLevels[2] && ptr->CurrentValue < ptr->CalibrationValue + ptr->AirLevels[3] ) {ptr->Quality=Orange;}else
				if(ptr->CurrentValue >= ptr->CalibrationValue + ptr->AirLevels[3]) {ptr->Quality=Red;}
}

void SetSensorAirLevels(SensorStruct *ptr, uint16_t Levels[])
{
  memcpy(ptr->AirLevels,Levels,sizeof(uint16_t)*4);
}

void SensorCalibrate(SensorStruct *ptr1, SensorStruct *ptr2)
{
	static int i=0;
	static int j=0;
	static int p=0;
	p++;
	p&=0x03;
	HAL_Delay(1000);

	if(ptr1!=NULL && ptr1->calibrated==0)
	{
		if(i< ptr1->CallibrationSamples){
		ptr1->CalibrationValue+=GetAdcValue(ptr1);
		ptr1->Quality=p;
		i++;

		} else
		{
			ptr1->calibrated=1;
			ptr1->CalibrationValue/=i;
			i=0;
		}

	}
	if(ptr2!=NULL && ptr2->calibrated==0)
	{
		if(j< ptr2->CallibrationSamples){
		ptr2->CalibrationValue+=GetAdcValue(ptr2);
		j++;
		ptr2->Quality=p;



		}else
		{
			ptr2->calibrated=1;
			ptr2->CalibrationValue/=j;
			j=0;
		}
	}

}

uint8_t CheckCalibration(SensorStruct *ptr)
{
	return ptr->calibrated;
}

void DisplaySensorState(SensorStruct *ptr)
{
	uint8_t i=0;
		 for(i=0;i<4;i++)
		 	 {
		 		 HAL_GPIO_WritePin(GPIOE,ptr->Display[i],GPIO_PIN_RESET);
		 	 }
 		 HAL_GPIO_WritePin(GPIOE,ptr->Display[ptr->Quality],GPIO_PIN_SET);

}

