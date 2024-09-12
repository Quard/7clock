#include "system.h"
#include "peripherals/twi.h"


void system_init(void) {
  // Clock config
  // 20Mhz / 10 = 2Mhz
  CCP = 0xD8;
  CLKCTRL.MCLKCTRLB = (0x9 << 1) | 1;
  CCP = 0x00;

  twi_init();
}
