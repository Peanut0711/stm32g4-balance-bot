#include "imu/icm42670.h"


#ifdef _USE_HW_ICM42670
#include "include/i2c.h"
#include "include/log.h"
#include "cli.h"
#include "imu/icm42670_regs.h"
#include <stdint.h>

#ifndef UINT32_MAX
typedef unsigned int uint32_t;
#endif


#define WHO_AM_I      ICM42670_WHO_AM_I
#define REG_WHO_AM_I  ICM42670_REG_WHO_AM_I


#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *args);
#endif
bool regReadByte(uint16_t addr, uint8_t *p_data);
bool regWriteByte(uint16_t addr, uint8_t data);
bool regRead(uint16_t addr, uint8_t *p_data, uint16_t length);
bool regWrite(uint16_t addr, uint8_t *p_data, uint16_t length);
static bool icm42670InitRegs(void);

static bool    is_init     = false;
static uint8_t i2c_ch      = HW_I2C_CH_IMU;
static uint8_t i2c_addr    = ICM42670_I2C_ADDR_GND;
static bool    is_found    = false;

// 캘리브레이션 기본값
#define ICM42670_CALIB_SAMPLES    500
#define ICM42670_CALIB_DELAY_MS   2

// MCLK_RDY bit (MCLK_RDY<3>)
#define ICM42670_MCLK_RDY_BITS    0x08

static uint8_t icm42670AccelRangeToReg(uint16_t fsr_g)
{
  switch (fsr_g)
  {
    case 2:  return ICM42670_ACCEL_RANGE_2G;
    case 4:  return ICM42670_ACCEL_RANGE_4G;
    case 8:  return ICM42670_ACCEL_RANGE_8G;
    case 16: return ICM42670_ACCEL_RANGE_16G;
    default: return ICM42670_ACCEL_RANGE_8G;
  }
}

static uint8_t icm42670GyroRangeToReg(uint16_t fsr_dps)
{
  switch (fsr_dps)
  {
    case 250:  return ICM42670_GYRO_RANGE_250DPS;
    case 500:  return ICM42670_GYRO_RANGE_500DPS;
    case 1000: return ICM42670_GYRO_RANGE_1000DPS;
    case 2000: return ICM42670_GYRO_RANGE_2000DPS;
    default:   return ICM42670_GYRO_RANGE_1000DPS;
  }
}

static uint8_t icm42670GyroOdrToReg(uint16_t odr_hz)
{
  switch (odr_hz)
  {
    case 12:   return ICM42670_GYRO_ODR_12_5HZ;
    case 25:   return ICM42670_GYRO_ODR_25HZ;
    case 50:   return ICM42670_GYRO_ODR_50HZ;
    case 100:  return ICM42670_GYRO_ODR_100HZ;
    case 200:  return ICM42670_GYRO_ODR_200HZ;
    case 400:  return ICM42670_GYRO_ODR_400HZ;
    case 800:  return ICM42670_GYRO_ODR_800HZ;
    case 1600: return ICM42670_GYRO_ODR_1_6KHZ;
    default:   return ICM42670_GYRO_ODR_200HZ;
  }
}

static uint8_t icm42670AccelOdrToReg(uint16_t odr_hz)
{
  switch (odr_hz)
  {
    case 12:   return ICM42670_ACCEL_ODR_12_5HZ;
    case 25:   return ICM42670_ACCEL_ODR_25HZ;
    case 50:   return ICM42670_ACCEL_ODR_50HZ;
    case 100:  return ICM42670_ACCEL_ODR_100HZ;
    case 200:  return ICM42670_ACCEL_ODR_200HZ;
    case 400:  return ICM42670_ACCEL_ODR_400HZ;
    case 800:  return ICM42670_ACCEL_ODR_800HZ;
    case 1600: return ICM42670_ACCEL_ODR_1_6KHZ;
    default:   return ICM42670_ACCEL_ODR_200HZ;
  }
}





bool icm42670Init(void)
{
  bool ret = true;


  if (!i2cIsBegin(i2c_ch))
  {
    ret = i2cBegin(i2c_ch, ICM42670_I2C_FREQ_KHZ);
  }

  if (ret)
  {
    uint8_t data = 0;
   
    regReadByte(ICM42670_REG_WHO_AM_I, &data);
    if (data == WHO_AM_I)
    {
      icm42670InitRegs();
      is_found = true;
    }    
  }
  if (is_found)
    logPrintf("[OK] ICM42670 Found\n");
  else
    logPrintf("[NG] ICM42670 Not Found\n");

#ifdef _USE_HW_CLI
  cliAdd("icm42670", cliCmd);
#endif

  is_init = is_found;
  ret = is_init;
  
  return ret;
}

bool icm42670InitRegs(void)
{
  
  uint8_t data;


  data = 1 << ICM42670_FIFO_FLUSH_SHIFT;
  regWriteByte(ICM42670_REG_SIGNAL_PATH_RESET, data);
  delay(1);

  data = 1 << ICM42670_SOFT_RESET_DEVICE_CONFIG_SHIFT;
  regWriteByte(ICM42670_REG_SIGNAL_PATH_RESET, data);
  delay(10);


  data  = ICM42670_GYRO_ENABLE_LN_MODE  << ICM42670_GYRO_MODE_SHIFT;
  data |= ICM42670_ACCEL_ENABLE_LN_MODE << ICM42670_ACCEL_MODE_SHIFT;
  regWriteByte(ICM42670_REG_PWR_MGMT0, data);
  delay(50);


  data  = (ICM42670_INT_MODE_PULSED          << ICM42670_INT1_MODE_SHIFT);
  data |= (ICM42670_INT_DRIVE_PUSH_PULL      << ICM42670_INT1_DRIVE_CIRCUIT_SHIFT);
  data |= (ICM42670_INT_POLARITY_ACTIVE_HIGH << ICM42670_INT1_POLARITY_SHIFT);
  data |= (ICM42670_INT_MODE_PULSED          << ICM42670_INT2_MODE_SHIFT);
  data |= (ICM42670_INT_DRIVE_PUSH_PULL      << ICM42670_INT2_DRIVE_CIRCUIT_SHIFT);
  data |= (ICM42670_INT_POLARITY_ACTIVE_HIGH << ICM42670_INT2_POLARITY_SHIFT);
  regWriteByte(ICM42670_REG_INT_CONFIG, data);


  data = (1 << ICM42670_DRDY_INT1_EN_SHIFT);
  regWriteByte(ICM42670_REG_INT_SOURCE0, data);


  // GYRO
  //
  data  = icm42670GyroRangeToReg(ICM42670_GYRO_FSR_DPS) << ICM42670_GYRO_UI_FS_SEL_SHIFT;
  data |= icm42670GyroOdrToReg(ICM42670_ODR_HZ) << ICM42670_GYRO_ODR_SHIFT;
  regWriteByte(ICM42670_REG_GYRO_CONFIG0, data);

  data = ICM42670_GYRO_LPF << ICM42670_GYRO_UI_FILT_BW_SHIFT;
  regWriteByte(ICM42670_REG_GYRO_CONFIG1, data);

  // ACCEL
  //
  data  = icm42670AccelRangeToReg(ICM42670_ACCEL_FSR_G)  << ICM42670_ACCEL_UI_FS_SEL_SHIFT;
  data |= icm42670AccelOdrToReg(ICM42670_ODR_HZ) << ICM42670_ACCEL_ODR_SHIFT;
  regWriteByte(ICM42670_REG_ACCEL_CONFIG0, data);

  data  = ICM42670_ACCEL_AVG_2X << ICM42670_ACCEL_UI_AVG_SHIFT;
  data |= ICM42670_ACCEL_LPF << ICM42670_ACCEL_UI_FILT_BW_SHIFT;
  regWriteByte(ICM42670_REG_ACCEL_CONFIG1, data);

  return true;
}

bool icm42670GetInfo(icm42670_info_t *p_info)
{
  bool ret = true;
  uint8_t buf[14];
  
  if (is_found == false)
    return false;
  
  p_info->timestamp = micros();
  ret = regRead(ICM42670_REG_TEMP_DATA1, buf, sizeof(buf));

  p_info->acc_x = (buf[2]<<8) | (buf[3]<<0);
  p_info->acc_y = (buf[4]<<8) | (buf[5]<<0);
  p_info->acc_z = (buf[6]<<8) | (buf[7]<<0);

  p_info->gyro_x = (buf[8]<<8) | (buf[9]<<0);
  p_info->gyro_y = (buf[10]<<8) | (buf[11]<<0);
  p_info->gyro_z = (buf[12]<<8) | (buf[13]<<0);

  p_info->temp = (buf[0]<<8) | (buf[1]<<0);

  p_info->acc_scale = 32768 / ICM42670_ACCEL_FSR_G;
  p_info->gyro_scale = 32768 / ICM42670_GYRO_FSR_DPS;
  
  return ret;
}

bool regReadByte(uint16_t addr, uint8_t *p_data)
{
  bool ret;  

  ret = regRead(addr, p_data, 1);
  return ret;
}

bool regWriteByte(uint16_t addr, uint8_t data)
{
  bool ret;  

  ret = regWrite(addr, &data, 1);
  return ret;
}

bool regRead(uint16_t addr, uint8_t *p_data, uint16_t length)
{
  bool ret = true;  

  ret = i2cIsDeviceReady(i2c_ch, i2c_addr);
  if (ret)
  {
    ret = i2cReadBytes(i2c_ch, i2c_addr, addr, p_data, length, length * 5);
  }

  return ret;
}

bool regWrite(uint16_t addr, uint8_t *p_data, uint16_t length)
{
  bool ret = true;  

  ret = i2cIsDeviceReady(i2c_ch, i2c_addr);
  if (ret)
  {
    ret = i2cWriteBytes(i2c_ch, i2c_addr, addr, p_data, length, length * 5);
  }

  return ret;
}

static bool icm42670MclkOn(uint8_t *pwr_mgmt0_prev)
{
  uint8_t data;
  uint8_t timeout_ms = 50;

  if (!regReadByte(ICM42670_REG_PWR_MGMT0, &data))
    return false;

  if (pwr_mgmt0_prev != NULL)
    *pwr_mgmt0_prev = data;

  if ((data & ICM42670_IDLE_BITS) == 0)
  {
    data |= ICM42670_IDLE_BITS;
    if (!regWriteByte(ICM42670_REG_PWR_MGMT0, data))
      return false;
  }

  while (1)
  {
    if (!regReadByte(ICM42670_REG_MCLK_RDY, &data))
      return false;

    if (data & ICM42670_MCLK_RDY_BITS)
      break;

    if (timeout_ms == 0)
      return false;

    delay(1);
    timeout_ms--;
  }

  return true;
}

static bool icm42670MclkOff(uint8_t pwr_mgmt0_prev)
{
  return regWriteByte(ICM42670_REG_PWR_MGMT0, pwr_mgmt0_prev);
}

/**
 * @brief MREG1 레지스터에 쓰기 (I2C 간접 접근)
 * @param mreg1_addr MREG1 내부 주소 (0x00 ~ 0xFF)
 * @param data 쓸 데이터
 * @return 성공 여부
 * 
 * I2C에서 MREG1 레지스터에 접근하는 방법:
 * 1. BLK_SEL_W (0x79)에 0x01 쓰기 (MREG1 선택)
 * 2. MADDR_W (0x7A)에 MREG1 내부 주소 쓰기
 * 3. M_W (0x7B)에 데이터 쓰기
 * 4. BLK_SEL_W에 0x00 쓰기 (원래대로 복귀)
 */
static bool regWriteMreg1(uint8_t mreg1_addr, uint8_t data)
{
  bool ret = true;
  uint8_t blk_sel = 0x00;  // MREG1 선택 (datasheet: MREG1 = 0x00)
  uint8_t zero = 0x00;
  uint8_t pwr_mgmt0_prev = 0;

  if (!icm42670MclkOn(&pwr_mgmt0_prev))
    return false;

  // MREG1 선택
  ret = regWriteByte(ICM42670_REG_BLK_SEL_W, blk_sel);
  if (!ret) return false;
  
  delayUs(10);
  
  // MREG1 내부 주소 설정
  ret = regWriteByte(ICM42670_REG_MADDR_W, mreg1_addr);
  if (!ret) return false;
  
  delayUs(10);
  
  // 데이터 쓰기
  ret = regWriteByte(ICM42670_REG_M_W, data);
  if (!ret) return false;
  
  delayUs(10);
  
  // BLK_SEL을 0으로 복귀
  ret = regWriteByte(ICM42670_REG_BLK_SEL_W, zero);

  if (!icm42670MclkOff(pwr_mgmt0_prev))
    return false;

  return ret;
}

/**
 * @brief MREG1 레지스터에서 읽기 (I2C 간접 접근)
 * @param mreg1_addr MREG1 내부 주소 (0x00 ~ 0xFF)
 * @param p_data 읽은 데이터를 저장할 포인터
 * @return 성공 여부
 */
static bool regReadMreg1(uint8_t mreg1_addr, uint8_t *p_data)
{
  bool ret = true;
  uint8_t blk_sel = 0x00;  // MREG1 선택 (datasheet: MREG1 = 0x00)
  uint8_t zero = 0x00;
  uint8_t pwr_mgmt0_prev = 0;

  if (!icm42670MclkOn(&pwr_mgmt0_prev))
    return false;

  // MREG1 선택
  ret = regWriteByte(ICM42670_REG_BLK_SEL_R, blk_sel);
  if (!ret) return false;
  
  delayUs(10);
  
  // MREG1 내부 주소 설정
  ret = regWriteByte(ICM42670_REG_MADDR_R, mreg1_addr);
  if (!ret) return false;
  
  delayUs(10);
  
  // 데이터 읽기
  ret = regReadByte(ICM42670_REG_M_R, p_data);
  if (!ret) return false;
  
  delayUs(10);
  
  // BLK_SEL을 0으로 복귀
  ret = regWriteByte(ICM42670_REG_BLK_SEL_R, zero);

  if (!icm42670MclkOff(pwr_mgmt0_prev))
    return false;

  return ret;
}

static int16_t icm42670SignExtend12(uint16_t value)
{
  value &= 0x0FFF;
  if (value & 0x0800)
  {
    return (int16_t)(value | 0xF000);
  }
  return (int16_t)value;
}

static bool icm42670ReadOffsetRegs(uint8_t regs[9])
{
  uint8_t addr = ICM42670_REG_OFFSET_USER0;
  for (int i = 0; i < 9; i++)
  {
    if (!regReadMreg1(addr + i, &regs[i]))
    {
      return false;
    }
  }
  return true;
}

static bool icm42670PrintOffsets(void)
{
  uint8_t r[9] = {0};

  if (!icm42670ReadOffsetRegs(r))
  {
    cliPrintf("offset read fail\n");
    return false;
  }

  int16_t gx = icm42670SignExtend12((uint16_t)r[0] | ((uint16_t)(r[1] & 0x0F) << 8));
  int16_t gy = icm42670SignExtend12((uint16_t)r[2] | ((uint16_t)((r[1] >> 4) & 0x0F) << 8));
  int16_t gz = icm42670SignExtend12((uint16_t)r[3] | ((uint16_t)(r[4] & 0x0F) << 8));

  int16_t ax = icm42670SignExtend12((uint16_t)r[5] | ((uint16_t)(r[4] >> 4) << 8));
  int16_t ay = icm42670SignExtend12((uint16_t)r[6] | ((uint16_t)(r[7] & 0x0F) << 8));
  int16_t az = icm42670SignExtend12((uint16_t)r[8] | ((uint16_t)(r[7] >> 4) << 8));

  cliPrintf("offset regs: ");
  for (int i = 0; i < 9; i++)
  {
    cliPrintf("0x%02X ", r[i]);
  }
  cliPrintf("\n");
  cliPrintf("gyro off (1/32 dps): %d %d %d\n", gx, gy, gz);
  cliPrintf("accel off (0.5 mgee): %d %d %d\n", ax, ay, az);

  return true;
}

/**
 * @brief 수평 상태에서 오프셋 캘리브레이션
 * @param sample_count 샘플 개수
 * @param sample_delay_ms 샘플 간 지연(ms)
 * @return 성공 여부
 *
 * - 자이로: 평균값(LSB)을 그대로 오프셋으로 사용
 * - 가속도: X/Y는 0g, Z는 +/-1g 기대값 기준으로 오프셋 계산
 * - 기존 오프셋은 먼저 0으로 리셋 후 측정
 */
bool icm42670Calibrate(uint16_t sample_count, uint16_t sample_delay_ms)
{
  int64_t sum_acc[3] = {0};
  int64_t sum_gyro[3] = {0};
  uint16_t valid = 0;
  icm42670_info_t info;

  if (sample_count == 0)
  {
    return false;
  }

#ifdef _USE_HW_CLI
  cliPrintf("offset before:\n");
  icm42670PrintOffsets();
#endif

  // 기존 오프셋 제거 후 측정
  icm42670SetGyroOffset(0, 0, 0);
  icm42670SetAccelOffset(0, 0, 0);
  delay(10);

  for (uint16_t i = 0; i < sample_count; i++)
  {
    if (icm42670GetInfo(&info))
    {
      sum_acc[0] += info.acc_x;
      sum_acc[1] += info.acc_y;
      sum_acc[2] += info.acc_z;
      sum_gyro[0] += info.gyro_x;
      sum_gyro[1] += info.gyro_y;
      sum_gyro[2] += info.gyro_z;
      valid++;
    }
    delay(sample_delay_ms);
  }

  if (valid == 0)
  {
    return false;
  }

  int32_t mean_acc_x = (int32_t)(sum_acc[0] / (int32_t)valid);
  int32_t mean_acc_y = (int32_t)(sum_acc[1] / (int32_t)valid);
  int32_t mean_acc_z = (int32_t)(sum_acc[2] / (int32_t)valid);
  int32_t mean_gyro_x = (int32_t)(sum_gyro[0] / (int32_t)valid);
  int32_t mean_gyro_y = (int32_t)(sum_gyro[1] / (int32_t)valid);
  int32_t mean_gyro_z = (int32_t)(sum_gyro[2] / (int32_t)valid);

  // 수평 기준: Z축은 +/-1g로 자동 판단
  int16_t expected_z = (mean_acc_z >= 0) ? (int16_t)info.acc_scale : (int16_t)-info.acc_scale;

  // 오프셋은 기대값 - 평균값으로 계산 (보정 후 평균이 기대값에 수렴하도록)
  int16_t acc_offset_x = (int16_t)(0 - mean_acc_x);
  int16_t acc_offset_y = (int16_t)(0 - mean_acc_y);
  int16_t acc_offset_z = (int16_t)(expected_z - mean_acc_z);

  int16_t gyro_offset_x = (int16_t)(0 - mean_gyro_x);
  int16_t gyro_offset_y = (int16_t)(0 - mean_gyro_y);
  int16_t gyro_offset_z = (int16_t)(0 - mean_gyro_z);

  if (!icm42670SetAccelOffset(acc_offset_x, acc_offset_y, acc_offset_z))
  {
    return false;
  }

  if (!icm42670SetGyroOffset(gyro_offset_x, gyro_offset_y, gyro_offset_z))
  {
    return false;
  }

#ifdef _USE_HW_CLI
  cliPrintf("calib samples: %d, delay: %dms\n", valid, sample_delay_ms);
  cliPrintf("mean acc: %d %d %d (expected z: %d)\n", mean_acc_x, mean_acc_y, mean_acc_z, expected_z);
  cliPrintf("mean gyro: %d %d %d\n", mean_gyro_x, mean_gyro_y, mean_gyro_z);
  cliPrintf("set acc offset: %d %d %d\n", acc_offset_x, acc_offset_y, acc_offset_z);
  cliPrintf("set gyro offset: %d %d %d\n", gyro_offset_x, gyro_offset_y, gyro_offset_z);
  cliPrintf("offset after:\n");
  icm42670PrintOffsets();
#endif

  return true;
}

static int16_t icm42670ClampOffset12(int32_t value)
{
  if (value > 2047) return 2047;
  if (value < -2048) return -2048;
  return (int16_t)value;
}

/**
 * @brief 자이로 오프셋 설정
 * @param gyro_x_offset X축 자이로 오프셋 (raw LSB 기준)
 * @param gyro_y_offset Y축 자이로 오프셋 (raw LSB 기준)
 * @param gyro_z_offset Z축 자이로 오프셋 (raw LSB 기준)
 * @return 성공 여부
 *
 * ICM42670 오프셋 레지스터 구조 (12bit):
 * - Gyro X: OFFSET_USER0[7:0] (LSB) + OFFSET_USER1[3:0] (MSB)
 * - Gyro Y: OFFSET_USER1[7:4] (LSB) + OFFSET_USER2[7:0] (MSB)
 * - Gyro Z: OFFSET_USER3[7:0] (LSB) + OFFSET_USER4[3:0] (MSB)
 *
 * 레지스터 해상도: 1/32 dps (±64 dps)
 * 입력(raw LSB)은 현재 설정된 FSR 기준으로 1/32 dps 단위로 변환해서 기록
 */
bool icm42670SetGyroOffset(int16_t gyro_x_offset, int16_t gyro_y_offset, int16_t gyro_z_offset)
{
  bool ret = true;
  int32_t offset_x, offset_y, offset_z;
  uint8_t data;
  int32_t scale = 32768 / ICM42670_GYRO_FSR_DPS;

  // raw LSB -> (1/32 dps) 단위로 변환
  int32_t num_x = (int32_t)gyro_x_offset * 32;
  int32_t num_y = (int32_t)gyro_y_offset * 32;
  int32_t num_z = (int32_t)gyro_z_offset * 32;

  offset_x = (num_x >= 0) ? ((num_x + (scale / 2)) / scale) : ((num_x - (scale / 2)) / scale);
  offset_y = (num_y >= 0) ? ((num_y + (scale / 2)) / scale) : ((num_y - (scale / 2)) / scale);
  offset_z = (num_z >= 0) ? ((num_z + (scale / 2)) / scale) : ((num_z - (scale / 2)) / scale);

  int16_t reg_x = icm42670ClampOffset12(offset_x);
  int16_t reg_y = icm42670ClampOffset12(offset_y);
  int16_t reg_z = icm42670ClampOffset12(offset_z);

  uint16_t ux = (uint16_t)reg_x & 0x0FFF;
  uint16_t uy = (uint16_t)reg_y & 0x0FFF;
  uint16_t uz = (uint16_t)reg_z & 0x0FFF;
  
  // Gyro X 오프셋 설정
  // OFFSET_USER0: X LSB (8bit)
  data = (uint8_t)(ux & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER0, data);
  if (!ret) return false;
  
  // OFFSET_USER1: X MSB (4bit) + Y MSB (4bit)
  data = (uint8_t)((ux >> 8) & 0x0F);  // X MSB 4bit
  data |= (uint8_t)(((uy >> 8) & 0x0F) << 4); // Y MSB 4bit
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER1, data);
  if (!ret) return false;
  
  // OFFSET_USER2: Y LSB (8bit)
  data = (uint8_t)(uy & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER2, data);
  if (!ret) return false;
  
  // Gyro Z 오프셋 설정
  // OFFSET_USER3: Z LSB (8bit)
  data = (uint8_t)(uz & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER3, data);
  if (!ret) return false;
  
  // OFFSET_USER4: Z MSB (4bit) + Accel X LSB (4bit) - 기존 Accel X 값 보존
  uint8_t current_user4 = 0;
  ret = regReadMreg1(ICM42670_REG_OFFSET_USER4, &current_user4);
  if (!ret) return false;
  
  // Z MSB 4bit 설정, Accel X LSB 4bit는 기존 값 유지
  data = (uint8_t)((uz >> 8) & 0x0F);  // Z MSB 4bit
  data |= (current_user4 & 0xF0);  // Accel X LSB 4bit 유지
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER4, data);
  if (!ret) return false;
  
  return ret;
}

/**
 * @brief 가속도 오프셋 설정
 * @param accel_x_offset X축 가속도 오프셋 (raw LSB 기준)
 * @param accel_y_offset Y축 가속도 오프셋 (raw LSB 기준)
 * @param accel_z_offset Z축 가속도 오프셋 (raw LSB 기준)
 * @return 성공 여부
 *
 * ICM42670 오프셋 레지스터 구조 (12bit):
 * - Accel X: OFFSET_USER4[7:4] (MSB) + OFFSET_USER5[7:0] (LSB)
 * - Accel Y: OFFSET_USER7[3:0] (MSB) + OFFSET_USER6[7:0] (LSB)
 * - Accel Z: OFFSET_USER7[7:4] (MSB) + OFFSET_USER8[7:0] (LSB)
 *
 * 레지스터 해상도: 0.5 mgee/LSB (±1 g)
 * 입력(raw LSB)은 현재 설정된 FSR 기준으로 0.5 mgee 단위로 변환해서 기록
 */
bool icm42670SetAccelOffset(int16_t accel_x_offset, int16_t accel_y_offset, int16_t accel_z_offset)
{
  bool ret = true;
  int32_t offset_x, offset_y, offset_z;
  uint8_t data;
  int32_t scale = 32768 / ICM42670_ACCEL_FSR_G;

  // raw LSB -> 0.5 mgee 단위로 변환 (1 g = 1000 mgee)
  int32_t num_x = (int32_t)accel_x_offset * 2000;
  int32_t num_y = (int32_t)accel_y_offset * 2000;
  int32_t num_z = (int32_t)accel_z_offset * 2000;

  offset_x = (num_x >= 0) ? ((num_x + (scale / 2)) / scale) : ((num_x - (scale / 2)) / scale);
  offset_y = (num_y >= 0) ? ((num_y + (scale / 2)) / scale) : ((num_y - (scale / 2)) / scale);
  offset_z = (num_z >= 0) ? ((num_z + (scale / 2)) / scale) : ((num_z - (scale / 2)) / scale);

  int16_t reg_x = icm42670ClampOffset12(offset_x);
  int16_t reg_y = icm42670ClampOffset12(offset_y);
  int16_t reg_z = icm42670ClampOffset12(offset_z);

  uint16_t ux = (uint16_t)reg_x & 0x0FFF;
  uint16_t uy = (uint16_t)reg_y & 0x0FFF;
  uint16_t uz = (uint16_t)reg_z & 0x0FFF;
  
  // OFFSET_USER4: Gyro Z MSB(4bit) + Accel X MSB(4bit) - 기존 Gyro Z 값 보존
  uint8_t current_user4 = 0;
  ret = regReadMreg1(ICM42670_REG_OFFSET_USER4, &current_user4);
  if (!ret) return false;
  
  data = (uint8_t)(((ux >> 8) & 0x0F) << 4);  // Accel X MSB 4bit
  data |= (current_user4 & 0x0F);                  // Gyro Z MSB 4bit 유지
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER4, data);
  if (!ret) return false;
  
  // OFFSET_USER5: Accel X LSB (8bit)
  data = (uint8_t)(ux & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER5, data);
  if (!ret) return false;
  
  // OFFSET_USER6: Accel Y LSB (8bit)
  data = (uint8_t)(uy & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER6, data);
  if (!ret) return false;
  
  // OFFSET_USER7: Accel Y MSB (4bit) + Accel Z MSB (4bit)
  data = (uint8_t)((uy >> 8) & 0x0F);        // Accel Y MSB 4bit
  data |= (uint8_t)(((uz >> 8) & 0x0F) << 4); // Accel Z MSB 4bit
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER7, data);
  if (!ret) return false;
  
  // OFFSET_USER8: Accel Z LSB (8bit)
  data = (uint8_t)(uz & 0xFF);
  ret = regWriteMreg1(ICM42670_REG_OFFSET_USER8, data);
  if (!ret) return false;
  
  return ret;
}

#ifdef _USE_HW_CLI
void cliCmd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    uint8_t data = 0;

    regReadByte(REG_WHO_AM_I, &data);
    
    cliPrintf("is init  : %d\n", is_init);
    cliPrintf("WHO_AM_I : 0x%02X\n", data);
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "reg") == true)
  {
    uint16_t addr;
    uint8_t data;
    uint8_t len;

    addr = args->getData(1);
    len  = args->getData(2);


    for (int i=0; i<len; i++)
    {
      regReadByte(addr + i, &data);
      cliPrintf("0x%02X : 0x%02X\n", addr + i, data);
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "get") && args->isStr(1, "raw"))
  {
    icm42670_info_t info;


    while(cliKeepLoop())
    {
      if (icm42670GetInfo(&info))
      {
        cliPrintf("ax=%-5d ay=%-5d az=%-5d, ", info.acc_x, info.acc_y, info.acc_z);
        cliPrintf("gx=%-5d gy=%-5d gz=%-5d\n", info.gyro_x, info.gyro_y, info.gyro_z);
      }
      else
      {
        cliPrintf("icm42670GetInfo() Err\n");
        break;
      }
      delay(10);
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "get") && args->isStr(1, "info"))
  {
    icm42670_info_t info;


    while(cliKeepLoop())
    {
      if (icm42670GetInfo(&info))
      {
        float ax = (float)info.acc_x / (float)info.acc_scale;
        float ay = (float)info.acc_y / (float)info.acc_scale;
        float az = (float)info.acc_z / (float)info.acc_scale;

        float gx = (float)info.gyro_x / (float)info.gyro_scale;
        float gy = (float)info.gyro_y / (float)info.gyro_scale;
        float gz = (float)info.gyro_z / (float)info.gyro_scale;

        cliPrintf("ax=%0.4fg ay=%0.4fg az=%0.4fg, ", ax, ay, az);
        cliPrintf("gx=%0.3fdps gy=%0.3fdps gz=%0.3fdps\n", gx, gy, gz);
      }
      else
      {
        cliPrintf("icm42670GetInfo() Err\n");
        break;
      }
      delay(10);
    }
    ret = true;
  }

  if (args->argc == 4 && args->isStr(0, "g_offset"))
  {
    int16_t gx = (int16_t)args->getData(1);
    int16_t gy = (int16_t)args->getData(2);
    int16_t gz = (int16_t)args->getData(3);

    if (icm42670SetGyroOffset(gx, gy, gz))
    {
      cliPrintf("gyro offset set: %d %d %d\n", gx, gy, gz);
      ret = true;
    }
  }

  if (args->argc == 4 && args->isStr(0, "a_offset"))
  {
    int16_t ax = (int16_t)args->getData(1);
    int16_t ay = (int16_t)args->getData(2);
    int16_t az = (int16_t)args->getData(3);

    if (icm42670SetAccelOffset(ax, ay, az))
    {
      cliPrintf("accel offset set: %d %d %d\n", ax, ay, az);
      ret = true;
    }
  }

  if (args->argc == 1 && args->isStr(0, "offset"))
  {
    icm42670PrintOffsets();
    ret = true;
  }

  if ((args->argc >= 1) && args->isStr(0, "calib"))
  {
    uint16_t sample_count = ICM42670_CALIB_SAMPLES;
    uint16_t sample_delay_ms = ICM42670_CALIB_DELAY_MS;

    if (args->argc >= 2)
    {
      sample_count = (uint16_t)args->getData(1);
    }
    if (args->argc >= 3)
    {
      sample_delay_ms = (uint16_t)args->getData(2);
    }

    if (icm42670Calibrate(sample_count, sample_delay_ms))
    {
      cliPrintf("icm42670 calib ok\n");
      ret = true;
    }
    else
    {
      cliPrintf("icm42670 calib fail\n");
      ret = true;
    }
  }

  if (ret != true)
  {
    cliPrintf("icm42670 info\n");
    cliPrintf("icm42670 reg addr len\n");
    cliPrintf("icm42670 get info\n");
    cliPrintf("icm42670 get raw\n");
    cliPrintf("icm42670 g_offset gx gy gz\n");
    cliPrintf("icm42670 a_offset ax ay az\n");
    cliPrintf("icm42670 offset\n");
    cliPrintf("icm42670 calib [samples] [delay_ms]\n");
  }
}


#endif


#endif
