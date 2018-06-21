include $(srctree)/drivers/misc/mediatek/Makefile.custom

# In case the platform does NOT support this type of sensors
obj-y   +=  mag.o
obj-y   +=  mag_factory.o

ifeq ($(CONFIG_MTK_QMC5983),y)
obj-y	+=  qmc5983/
endif

ifeq ($(CONFIG_MTK_QMCX983),y)
obj-y	+=  qmcX983/
endif