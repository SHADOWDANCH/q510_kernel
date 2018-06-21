#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>

static struct alsps_hw cust_alsps_hw = {
	.i2c_num    = 2,			// **lewonchik
	.polling_mode_ps =0,
	.polling_mode_als =1,		// **lewonchik
	.power_id   = MT65XX_POWER_NONE,    
	.power_vol  = VOL_DEFAULT,          
	//.i2c_addr   = {0x0C, 0x48, 0x78, 0x00},
    .als_level  = { 0,  1,  3,   7,  15,  60,  100, 1000, 2000,  3000,  6000, 10000, 14000, 18000, 20000},
    .als_value  = { 1,  3, 16,  32,  51,  75,  90,  180,  225,   320,   640,  1280,  2600,  5200,  10240, 10240},
    .ps_threshold_high = 29,	// **lewonchik  
    .ps_threshold_low = 16,		// **lewonchik
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

