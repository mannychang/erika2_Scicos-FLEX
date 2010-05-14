EEOPT += __NO_MALLOC__
EEOPT += MODELNAME=PJ
EEOPT += __USE_LEDS__
EE_SRCS_SCICOS := rt_sinus.c \
  flex_led.c
APP_SRCS += sci_PJ.c
