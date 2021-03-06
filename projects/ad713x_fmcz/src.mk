################################################################################
#									       #
#     Shared variables:							       #
#	- PROJECT							       #
#	- DRIVERS							       #
#	- INCLUDE							       #
#	- PLATFORM_DRIVERS						       #
#	- NO-OS								       #
#									       #
################################################################################

SRCS := $(PROJECT)/src/ad713x_fmc.c
SRCS += $(DRIVERS)/spi/spi.c						\
	$(DRIVERS)/adc/ad713x/ad713x.c					\
	$(DRIVERS)/axi_core/axi_dmac/axi_dmac.c				\
	$(DRIVERS)/axi_core/spi_engine/spi_engine.c			\
	$(NO-OS)/util/util.c
SRCS +=	$(PLATFORM_DRIVERS)/axi_io.c					\
	$(PLATFORM_DRIVERS)/gpio.c					\
	$(PLATFORM_DRIVERS)/xilinx_spi.c				\
	$(PLATFORM_DRIVERS)/delay.c
ifeq (y,$(strip $(TINYIIOD)))
SRCS += $(PLATFORM_DRIVERS)/uart.c					\
	$(PLATFORM_DRIVERS)/irq.c					\
	$(NO-OS)/util/xml.c						\
	$(NO-OS)/util/fifo.c						\
	$(NO-OS)/iio/iio.c						\
	$(NO-OS)/iio/iio_ad713x/iio_ad713x.c				\
	$(NO-OS)/iio/iio_app/iio_app.c
endif
INCS += $(DRIVERS)/adc/ad713x/ad713x.h					\
	$(DRIVERS)/axi_core/axi_dmac/axi_dmac.h				\
	$(DRIVERS)/axi_core/spi_engine/spi_engine.h			\
	$(DRIVERS)/axi_core/spi_engine/spi_engine_private.h
INCS +=	$(PLATFORM_DRIVERS)/spi_extra.h					\
	$(PLATFORM_DRIVERS)/irq_extra.h					\
	$(PLATFORM_DRIVERS)/uart_extra.h				\
	$(PLATFORM_DRIVERS)/gpio_extra.h
INCS +=	$(INCLUDE)/axi_io.h						\
	$(INCLUDE)/spi.h						\
	$(INCLUDE)/gpio.h						\
	$(INCLUDE)/error.h						\
	$(INCLUDE)/delay.h						\
	$(INCLUDE)/irq.h						\
	$(INCLUDE)/uart.h						\
	$(INCLUDE)/util.h
ifeq (y,$(strip $(TINYIIOD)))
INCS += $(INCLUDE)/xml.h						\
	$(INCLUDE)/fifo.h						\
	$(NO-OS)/iio/iio.h						\
	$(NO-OS)/iio/iio_types.h					\
	$(NO-OS)/iio/iio_ad713x/iio_ad713x.h				\
	$(NO-OS)/iio/iio_app/iio_app.h					\
	$(NO-OS)/libraries/libtinyiiod/tinyiiod.h			\
	$(NO-OS)/libraries/libtinyiiod/compat.h
endif
