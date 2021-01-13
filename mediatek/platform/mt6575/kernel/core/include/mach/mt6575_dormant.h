
#ifndef _MT6575_DORMANT_
#define _MT6575_DORMANT_

#define MAX_CPUS 1
#define STATUS_RUN      0
#define STATUS_STANDBY  1
#define STATUS_DORMANT  2
#define STATUS_SHUTDOWN 3
#define DORMANT_MODE  STATUS_DORMANT
#define SHUTDOWN_MODE STATUS_SHUTDOWN

int cpu_power_down(int mode);

void cpu_dormant_init(void);

void cpu_dormant_abort(void);

#endif

