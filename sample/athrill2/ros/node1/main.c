#include "test_serial.h"
#include "test_reg.h"
#include "section.h"
#include "interrupt.h"
#include <string.h>
#include "timer.h"
#include "v850_ins.h"
#include "roslib.h"

unsigned char stack_data[STACK_SIZE] __attribute__ ((section(".bss_noclr")));

int main(void)
{
	timer_init();
	roslib_init();

	printf("Hello World!!\n");
	timer_start(100U); /* 10ms */

	while (1) {
		do_idle();
	}
}

void bss_clear(void)
{
	unsigned char *p = &_bss_start;
	unsigned char *e = &_bss_end;
	for (;p < e; p++) {
		*p = 0;
	}
	return;
}

void data_init(void)
{
	unsigned char *p_rom = &_idata_start;
	unsigned char *e_rom = &_idata_end;
	unsigned char *p_ram = &_data_start;

	for (;p_rom < e_rom; p_ram++, p_rom++) {
		*p_ram = *p_rom;
	}
}

