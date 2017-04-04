/* gpio.h */

/* GPIO memory-mapped I/O address */
#define GPIO_BASE 0x4804C000

/* GPIO registers */
#define GPIO_OE           (GPIO_BASE + 0x134)
#define GPIO_DATAOUT      (GPIO_BASE + 0x13c)
#define GPIO_CLEARDATAOUT (GPIO_BASE + 0x190)
#define GPIO_SETDATAOUT   (GPIO_BASE + 0x194)

/* USRx LEDs */
#define GPIO_LED_USR0 (1<<21)
#define GPIO_LED_USR1 (1<<22)
#define GPIO_LED_USR2 (1<<23)
#define GPIO_LED_USR3 (1<<24)

/* LED control functions */

/**
 * Initialize GPIO pin as output
 */
#define initGPIO(n) (*((int32*)GPIO_OE) &= ~(n))

/**
 * Turns an LED on
 * @param n GPIO bit for LED (use GPIO_LED_* constants)
 */
#define gpioLEDOn(n) (*((int32*)GPIO_DATAOUT) |= (n))

/**
 * Turns an LED off
 * @param n GPIO bit for LED (use GPIO_LED_* constants)
 */
#define gpioLEDOff(n) (*((int32*)GPIO_DATAOUT) &= ~(n))
