/*
 * ap.c
 *
 *  Created on: Dec 4, 2024
 *      Author: user
 */

#include "def.h"
#include "ap.h"

void apInit(void)
{
  cliOpen(_DEF_UART1, 115200);
  logBoot(true);

  // balanceInit();
}

void apMain(void)
{
  uint32_t led_pre_time;
  static bool btn_prev_pressed = false;

  led_pre_time = millis();

	while(1)
	{
	  bool btn_pressed = buttonGetPressed(_DEF_BUTTON1);

	  if (btn_prev_pressed == true && btn_pressed == false)
	  {
	    if (buttonGetPressedTime(_DEF_BUTTON1) >= 300)
	    {	      
				icm42670Calibrate(500, 2);
	    }
	  }
	  btn_prev_pressed = btn_pressed;

	  if (millis() - led_pre_time >=  500)
	  {
	    led_pre_time = millis();
	    ledToggle(_DEF_LED1);
	  }

	  imuUpdate();
	  cliMain();
	  // balanceUpdate();
	  //btUpdate();
	  //rcCarUpdate();
  }
}


void ps2MotorWrite(ps2_state_t * ps2_state)
{
}

