#
#Makefile for EMIFA 128x64 LCD display driver
#
obj-$(CONFIG_OMAPL138_EMIFA)+= omap_emif.o
obj-$(CONFIG_LCD_INTERFACE)+= lcd-dev.o
obj-$(CONFIG_EMIFA_LCD)+= emif_lcd.o
