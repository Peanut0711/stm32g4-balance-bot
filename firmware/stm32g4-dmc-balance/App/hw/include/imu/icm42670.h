#ifndef ICM42670_H_
#define ICM42670_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hw_def.h"
#include <stdint.h>


#ifdef _USE_HW_ICM42670

// 기본 브링업/밸런싱용 설정값
#define ICM42670_I2C_FREQ_KHZ 400
#define ICM42670_ACCEL_FSR_G  8
#define ICM42670_GYRO_FSR_DPS 1000
#define ICM42670_ODR_HZ       200
#define ICM42670_GYRO_LPF     ICM42670_GYRO_LFP_25HZ
#define ICM42670_ACCEL_LPF    ICM42670_ACCEL_LFP_25HZ



typedef struct
{
  
  int16_t acc_x; 
  int16_t acc_y; 
  int16_t acc_z; 
  int16_t gyro_x; 
  int16_t gyro_y; 
  int16_t gyro_z; 

  int16_t temp;

  uint32_t timestamp;
  uint16_t acc_scale;
  uint16_t gyro_scale;
} icm42670_info_t;


bool icm42670Init(void);
bool icm42670GetInfo(icm42670_info_t *p_info);


#endif

#ifdef __cplusplus
 }
#endif

#endif 
