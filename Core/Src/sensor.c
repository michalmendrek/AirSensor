#include "sensor.h"

static uint16_t AIR[4]={LD6_Pin,LD4_Pin,LD8_Pin,LD3_Pin};
static uint16_t CO[4]={LD7_Pin,LD9_Pin,LD5_Pin,LD10_Pin};
static uint32_t ADC_3_INITIAL=0;
static uint32_t ADC_2_INITIAL=0;

void DisplayAir(QualityState State)
{
	int i=0;
		for(i=0;i<4;i++)
		{
			HAL_GPIO_WritePin(GPIOE,AIR[i],GPIO_PIN_RESET);
		}
	HAL_GPIO_WritePin(GPIOE,AIR[State],GPIO_PIN_SET);
}

void DisplayCO(QualityState State)
{
	int i=0;
	for(i=0;i<4;i++)
	{
		HAL_GPIO_WritePin(GPIOE,CO[i],GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(GPIOE,CO[State],GPIO_PIN_SET);
}


void GetMeasurements(void)
{
	extern ADC_HandleTypeDef hadc1;
	extern ADC_HandleTypeDef hadc2;

	static uint16_t adc_2=0, adc_3=0;
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
	{
		adc_2=HAL_ADC_GetValue(&hadc1);
		if(adc_2>ADC_2_INITIAL+600)DisplayAir(Red);
		if(adc_2<ADC_2_INITIAL+400 && adc_2 >ADC_2_INITIAL+300)DisplayAir(Orange);
		if(adc_2<ADC_2_INITIAL+300 && adc_2 >ADC_2_INITIAL+100) DisplayAir(Blue);
		if(adc_2<=ADC_2_INITIAL) DisplayAir(Green);

	}

	HAL_ADC_Start(&hadc2);

	if (HAL_ADC_PollForConversion(&hadc2, 100) == HAL_OK)
		{
	    adc_3=HAL_ADC_GetValue(&hadc2);
	    if(adc_3>ADC_3_INITIAL+500)DisplayCO(Red);
		if(adc_3<ADC_3_INITIAL+500 && adc_3 > ADC_3_INITIAL+400)DisplayCO(Orange);
	    if(adc_3<ADC_3_INITIAL+400 && adc_3 > ADC_3_INITIAL+300) DisplayCO(Blue);
	    if(adc_3<=ADC_3_INITIAL) DisplayCO(Green);
		}
	HAL_Delay (200);
	HAL_ADC_Stop (&hadc1);
	HAL_ADC_Stop (&hadc2);

}

void Calibrate(int val)
{
	static int i=0;
	static int j=0;
	static int state=0;
	extern ADC_HandleTypeDef hadc1;
	extern ADC_HandleTypeDef hadc2;
	uint16_t Leds[8]={LD4_Pin,LD3_Pin,LD5_Pin,LD7_Pin,LD9_Pin,LD10_Pin,LD8_Pin,LD6_Pin};
	if(val==1)
	{
if(i<8)
{
	HAL_GPIO_WritePin(GPIOE,Leds[i++],GPIO_PIN_SET);
	for(j=0;j<10;j++)
	{
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
			{
				ADC_2_INITIAL+=HAL_ADC_GetValue(&hadc1);
			}
		HAL_ADC_Start(&hadc2);

		if (HAL_ADC_PollForConversion(&hadc2, 100) == HAL_OK)
			{
			    ADC_3_INITIAL+=HAL_ADC_GetValue(&hadc2);
			}
			HAL_Delay (200);
			HAL_ADC_Stop (&hadc1);
			HAL_ADC_Stop (&hadc2);
	}
}
else
{
	if(state==0)
	{
	ADC_2_INITIAL/=80;
	ADC_3_INITIAL/=80;
	state=1;
	for(j=0;j<8;j++){HAL_GPIO_WritePin(GPIOE,Leds[j],GPIO_PIN_RESET);
}
	}
	}
	} else {state=0; i=0; j=0;}

}
