#include <xinu.h>

#ifdef GPIO

void heartbeat() {
  while (1) {
    gpioLEDOn(GPIO_LED_USR0);
    sleepms(100);
    gpioLEDOff(GPIO_LED_USR0);
    sleepms(200);
    gpioLEDOn(GPIO_LED_USR0);
    sleepms(100);
    gpioLEDOff(GPIO_LED_USR0);
    sleep(2);
  }
}

#endif

