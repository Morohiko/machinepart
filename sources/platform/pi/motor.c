#include <motor.h>
#include <log.h>

#if 0
#include <wiringPi.h>
#endif

int motor_initialize(int pwm_width_mcs)
{
    int retval = 0;
#if 0
  retval = wiringPiSetup();
  if (retval != 0) {
    hm_log_error("motor: "cannot setup wiring pi\n");
    return -1;
  }

  retval = wiringPiSetupGpio();
  if (retval) {
    hm_log_error("motor: "cannot setup wiring pi gpio\n");
    return -1;
  }

  retval = wiringPiSetupSys();
  if (retval) {
    hm_log_error("motor: "cannot setup wiring pi sys\n");
    return -1;
  }
#endif
    return retval;
}

int motor_pin_mode(int pin, int mode)
{
#if 0
  pinMode(pin, mode);
#endif
    return 0;
}

int motor_digital_write(int pin, int mode)
{
#if 0
  digitalWrite(pin, mode);
#endif
    return 0;
}

int motor_delay_microseconds(int mcs)
{
#if 0
  delayMicroseconds(mcs);
#endif
    return 0;
}

int motor_delay(int ms)
{
#if 0
  delay(ms);
#endif
    return 0;
}