#
# module.mk
#
# Copyright (C) 2010 Creytiv.com
#

MOD		:= speex_resamp
$(MOD)_SRCS	+= resampler.c
ifneq ($(HAVE_SPEEXDSP),)
$(MOD)_LFLAGS	+= "-lspeexdsp"
else
$(MOD)_LFLAGS	+= "-lspeex"
endif

include mk/mod.mk
