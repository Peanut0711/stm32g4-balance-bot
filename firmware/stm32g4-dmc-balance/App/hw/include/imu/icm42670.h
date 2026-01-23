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
#define ICM42670_ACCEL_FSR_G  4 // 16
#define ICM42670_GYRO_FSR_DPS 500 // 2000
#define ICM42670_ODR_HZ       400
#define ICM42670_GYRO_LPF     ICM42670_GYRO_LFP_53HZ
#define ICM42670_ACCEL_LPF    ICM42670_ACCEL_LFP_34HZ



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
bool icm42670SetGyroOffset(int16_t gyro_x_offset, int16_t gyro_y_offset, int16_t gyro_z_offset);
bool icm42670SetAccelOffset(int16_t accel_x_offset, int16_t accel_y_offset, int16_t accel_z_offset);
bool icm42670Calibrate(uint16_t sample_count, uint16_t sample_delay_ms);


#endif

#ifdef __cplusplus
 }
#endif

#endif 
