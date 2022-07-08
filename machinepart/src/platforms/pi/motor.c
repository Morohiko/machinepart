#include "motor.h"
#include "log.h"

#include "wiringPi.h"

int motor_initialize() {
  int retval = 0;

  retval = wiringPiSetup();
  if (retval != 0) {
    print(ERROR, "cannot setup wiring pi");
    return -1;
  }

  retval = wiringPiSetupGpio();
  if (retval) {
    print(ERROR, "cannot setup wiring pi gpio");
    return -1;
  }

  retval = wiringPiSetupSys();
  if (retval) {
    print(ERROR, "cannot setup wiring pi sys");
    return -1;
  }
}

int motor_pin_mode(int pin, int mode) {
    pinMode(pin, mode);
    return 0;
}

int motor_digital_write(int pin, int mode) {
    digitalWrite(pin, mode);
    return 0;
}

int motor_delay_microseconds(int mcs) {
    delayMicroseconds(mcs);
    return 0;
}

int motor_delay(int ms) {
    delay(ms);
    return 0;
}