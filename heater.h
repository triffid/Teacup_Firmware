#ifndef	_HEATER_H
#define	_HEATER_H

#include "config_wrapper.h"
#include	<stdint.h>
#include "simulator.h"
#include "temp.h"

/** \def PID_SCALE_P
  Conversion factor between internal kP value and user values.  Since temperatures are measured in C/4 units,
  kP in counts/C would be kP*PID_SCALE/4 in internal units. 
*/
#define PID_SCALE_P ((int32_t)PID_SCALE/4L)   // convert to internal 1/4C

/** \def PID_SCALE_I
  Conversion factor between internal kI value and user values. Since temperatures are measured in C/4 and the I
  accumulation is done four times per second, kI in counts/(C*s) would be kI*PID_SCALE/16 in internal counts/(qC*qs)
*/
#define PID_SCALE_I ((int32_t)PID_SCALE/16L)   // internal 1/4C by 1/4s second integration.

/** \def PID_SCALE_D
  Conversion factor between internal kD value and user values. 
  Since temperatures are measured in C/4 and the derivative is measured over TH_COUNT 250ms cycles,
  kD in counts/(C/s) would be kI*PID_SCALE/TH_COUNT in internal counts/(qC/(TH_COUNT*qs).
*/
#define PID_SCALE_D ((int32_t)PID_SCALE/TH_COUNT) // Internal 1/4 degree per 1/4s sampling cancels, but the dt window is TH_COUNT long.

#undef DEFINE_HEATER
#define DEFINE_HEATER(name, pin, pwm) HEATER_ ## name,
typedef enum
{
	#include "config_wrapper.h"
	NUM_HEATERS,
	HEATER_noheater
} heater_t;
#undef DEFINE_HEATER

void heater_init(void);

void heater_set(heater_t index, uint8_t value);
void heater_tick(heater_t h, temp_type_t type, uint16_t current_temp, uint16_t target_temp);

uint8_t heaters_all_zero(void);
uint8_t heaters_all_off(void);

#ifdef EECONFIG
void pid_set_p(heater_t index, int32_t p);
void pid_set_i(heater_t index, int32_t i);
void pid_set_d(heater_t index, int32_t d);
void pid_set_i_limit(heater_t index, int32_t i_limit);
void heater_save_settings(void);
#endif /* EECONFIG */

void heater_print(uint16_t i);

#endif	/* _HEATER_H */
