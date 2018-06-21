#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(720)
#define FRAME_HEIGHT 										(1280)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0x100

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)									(lcm_util.set_reset_pin((v)))

#define UDELAY(n)											(lcm_util.udelay(n))
#define MDELAY(n)											(lcm_util.mdelay(n))

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V3(para_tbl,size,force_update)			lcm_util.dsi_set_cmdq_V3(para_tbl,size,force_update)
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)   

#define dsi_lcm_set_gpio_out(pin, out)						lcm_util.set_gpio_out(pin, out)
#define dsi_lcm_set_gpio_mode(pin, mode)					lcm_util.set_gpio_mode(pin, mode)
#define dsi_lcm_set_gpio_dir(pin, dir)						lcm_util.set_gpio_dir(pin, dir)
#define dsi_lcm_set_gpio_pull_enable(pin, en)				lcm_util.set_gpio_pull_enable)(pin, en)

//#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))	// double function 		**lewonchik

static struct LCM_setting_table {									// static struct LCM_setting_table **lewonchik
  unsigned char cmd;												// unsigned char cmd;	**lewonchik
  unsigned char count;
  unsigned char para_list[108];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
    {0xB0,1,{0x04}},
    {0xE3,1,{0x05}},
    {0xB3,1,{0x00}},
    {0xB6,1,{0x32}},
    {0x36,1,{0x0A}},
    {0xC0,6,{0x20,0xB2,0x0D,0x10,0x02,0x80}},
    {0xC1,7,{0x85,0x85,0x85,0x85,0x10,0x10,0x33}},
    {0xC3,2,{0x00,0x0F}},
    {0xC4,3,{0xBC,0xD2,0x00}},
    {0xC5,3,{0x06,0x03,0x11}},
    {0xC6,1,{0x21}},
    {0xCA,30,{0x02,0x11,0x17,0x1C,0x23,0x26,0x28,0x24,0x20,0x20,0x1C,0x10,0x09,0x01,0x00,0x02,0x11,0x17,0x1C,0x23,0x26,0x28,0x24,0x20,0x20,0x1C,0x10,0x09,0x01,0x00}},
    {0xC8,28,{0x60,0x0E,0x08,0x62,0x90,0x52,0x95,0x54,0x49,0x29,0x84,0x90,0xA2,0x4A,0x29,0x45,0x15,0x42,0x48,0xA9,0xA4,0xA4,0xAA,0x85,0x0C,0x11,0xC0,0x31}},
    {0xD0,3,{0x05,0x95,0x14}},
    {0xD1,1,{0x03}},
    {0xD2,2,{0x02,0x1F}},
    {0xE5,1,{0x02}},
    {0xD4,1,{0x4D}},
    {0xD5,2,{0x24,0x24}},
    {0x11,0,{0x00}},
    {REGFLAG_DELAY, 120, {}},
    {0x29,0,{0x00}},
    {REGFLAG_DELAY, 50, {}},
    {REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    { 0x28,0,{0x00}},
    { REGFLAG_DELAY, 20, {}},
    { 0x10,0,{0x00}},
    { REGFLAG_DELAY, 120, {}},
    { 0xB0,1,{0x04}},
    { 0xB1,1,{0x01}},
    { REGFLAG_DELAY, 20, {}},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update) {
  unsigned int i;
  for(i = 0; i < count; i++) {
    unsigned cmd;
    cmd = table[i].cmd;
    switch (cmd) {
      case REGFLAG_DELAY :
        MDELAY(table[i].count);
        break;
      case REGFLAG_END_OF_TABLE :
        break;
      default:
        dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
		}
	}
}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
  memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
  memset(params, 0, sizeof(LCM_PARAMS));
  params->type = 2;
  params->dsi.mode = 2;
  params->dsi.data_format.format = 2;
  params->dsi.PS = 2;
  params->dsi.vertical_sync_active = 2;
  params->width = 720;
  params->dsi.vertical_backporch = 10;
  params->height = 1280;
  params->dsi.vertical_frontporch = 17;
  params->dsi.LANE_NUM = 4;
  params->dsi.horizontal_sync_active = 12;
  params->dsi.vertical_active_line = 1280;
  params->dsi.horizontal_backporch = 60;
  params->dsi.horizontal_active_pixel = 720;
  params->dsi.horizontal_frontporch = 90;
  params->dsi.compatibility_for_nvk = 0;
  params->dsi.PLL_CLOCK = 230;
}

static void lcm_init(void)
{

  SET_RESET_PIN(1);
  MDELAY(1);
  SET_RESET_PIN(0);
  MDELAY(50);
  SET_RESET_PIN(1);
  MDELAY(100);
  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_suspend(void)
{
  SET_RESET_PIN(1);
  MDELAY(1);
  SET_RESET_PIN(0);
  MDELAY(50);
  SET_RESET_PIN(1);
  MDELAY(100);
  push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_resume(void)
{
  lcm_init();
}

static unsigned int lcm_compare_id(void)
{
  volatile unsigned char buffer[4];
  unsigned int data_array[16];

  SET_RESET_PIN(1);
  SET_RESET_PIN(0);
  MDELAY(6);
  SET_RESET_PIN(1);
  MDELAY(50);

  data_array[0] = 0x00053700;
  dsi_set_cmdq(data_array, 1, 1);
  read_reg_v2(0xBF, buffer, 5);
  return (buffer[3] | (buffer[2] << 8)) - 4888 <= 0;
//  return 1;  // my default
}

unsigned int lcm_esd_check(void)
{
//  static int err_count = 0;
  unsigned int result = 0;
  volatile unsigned char buffer[4];
  unsigned int data_array[16];

//  if ( err_count ) // always false
//  {
//    result = 1;
//    err_count = 0;  // err_count == vC0D6D904
//  }
//  else
//  {
    data_array[0] = 0x00013700;
    dsi_set_cmdq(data_array, 1, 1);
    read_reg_v2(0xA, buffer, 1);
    result = buffer[0] - 8;
    if ( buffer[0] != 8 )
      result = 1;
//  }
  return result;
}

unsigned int lcm_esd_recover(void)
{
  lcm_init();
  return 1;
}

// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------

LCM_DRIVER r61318_dsi_pnl_lcm_drv = {
  .name = "r61318_dsi_pnl",
  .set_util_funcs = lcm_set_util_funcs,
  .get_params = lcm_get_params,
  .init = lcm_init,
  .suspend = lcm_suspend,
  .resume = lcm_resume,
  .compare_id = lcm_compare_id,
  .esd_check = lcm_esd_check,
  .esd_recover = lcm_esd_recover,
};
