include $(srctree)/drivers/misc/mediatek/Makefile.custom


# In case the platform does NOT support this type of sensors
obj-y   +=  mtk_tpd.o
obj-y   +=  tpd_button.o
obj-y   +=  tpd_calibrate.o
obj-y   +=  tpd_debug.o
obj-y   +=  tpd_default.o
obj-y   +=  tpd_init.o
obj-y   +=  tpd_misc.o
obj-y   +=  tpd_setting.o

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX),y)
obj-y	+=  GT9XX/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_AIO),y)
obj-y	+=  GT9XX_aio/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_HOTKNOT_SCP),y)
obj-y	+=  GT9XX_hotknot_SCP/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_HOTKNOT),y)
obj-y	+=  GT9XX_hotknot/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_HOTKNOT_WEAR),y)
obj-y	+=  GT9XX_hotknot_wear/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_HOTKNOT_FPGA),y)
obj-y	+=  GT9XX_hotknot_fpga/
endif

ifeq ($(CONFIG_TOUCHSCREEN_MTK_GT9XX_2),y)
obj-y	+=  GT9XX_2/
endif

ifeq ($(CONFIG_GT9XX_HOTKNOT_FLASHLESS),y)
obj-y	+=  GT9XX_hotknot_FLASHLESS/
endif
