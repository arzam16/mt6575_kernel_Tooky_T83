

#ifndef _CLOCK_H
#define _CLOCK_H

#include <asm/hardware/icst.h>

struct clk {
	unsigned long		rate;
	const struct icst_params *params;
	u32			oscoff;
	void			*data;
	void			(*setvco)(struct clk *, struct icst_vco vco);
};

#endif
