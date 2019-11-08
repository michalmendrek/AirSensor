#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"

typedef enum {Green=0,Blue=1,Orange=2,Red=3} QualityState;

void DisplayAir(QualityState State);
void DisplayCO(QualityState State);
void GetMeasurements(void);
void Calibrate(int val);



typedef struct
{
	uint16_t Display[4];
	uint32_t CalibrationValue;
	uint16_t CurrentValue;

} SensorStruct;




#endif

