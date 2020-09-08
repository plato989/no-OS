/***************************************************************************//**
 *   @file   ad469x_fmcz.c
 *   @brief  Implementation of Main Function.
 *   @author Cristian Pop (cristian.pop@analog.com)
 ********************************************************************************
 * Copyright 2020(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdio.h>
#include <inttypes.h>
#include <xil_cache.h>
#include <xparameters.h>
#include "spi_engine.h"
#include "pwm.h"
#include "axi_pwm_extra.h"
#include "ad469x.h"
#include "error.h"
#include "delay.h"
#include "clk_axi_clkgen.h"
#include "gpio.h"
#include "gpio_extra.h"

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/
#define AD469x_EVB_SAMPLE_NO		1000
#define AD469x_DMA_BASEADDR             XPAR_AXI_AD4696_DMA_BASEADDR
#define AD469x_SPI_ENGINE_BASEADDR      XPAR_SPI_AD4696_AXI_REGMAP_BASEADDR
#define RX_CLKGEN_BASEADDR		XPAR_SPI_CLKGEN_BASEADDR
#define AXI_PWMGEN_BASEADDR		XPAR_SPI_AD4696_TRIGGER_GEN_BASEADDR
#define AD469x_SPI_ENG_REF_CLK_FREQ_HZ	XPAR_PS7_SPI_0_SPI_CLK_FREQ_HZ
#define AD469x_SPI_CS                   0
#define GPIO_OFFSET			54
#define GPIO_RESETN_1			GPIO_OFFSET + 32
#define GPIO_DEVICE_ID			XPAR_PS7_GPIO_0_DEVICE_ID

int main()
{
	uint32_t buf[AD469x_EVB_SAMPLE_NO];
	struct ad469x_dev *dev;
	uint32_t ch, i, j = 0;
	int32_t ret;

	struct spi_engine_offload_init_param spi_engine_offload_init_param = {
		.offload_config = OFFLOAD_RX_EN,
		.rx_dma_baseaddr = AD469x_DMA_BASEADDR,
	};

	struct spi_engine_init_param spi_eng_init_param  = {
		.ref_clk_hz = AD469x_SPI_ENG_REF_CLK_FREQ_HZ,
		.type = SPI_ENGINE,
		.spi_engine_baseaddr = AD469x_SPI_ENGINE_BASEADDR,
		.cs_delay = 0,
		.data_width = 16,
	};

	struct axi_clkgen_init clkgen_init = {
		.name = "rx_clkgen",
		.base = RX_CLKGEN_BASEADDR,
		.parent_rate = 100000000,
	};

	struct axi_pwm_init_param axi_pwm_init = {
		.base_addr = AXI_PWMGEN_BASEADDR,
		.ref_clock_Hz = 160000000,
	};

	struct pwm_init_param trigger_pwm_init = {
		.period_ns = 1000,	/* 1Mhz */
		.duty_cycle_ns = 10,
		.polarity = PWM_POLARITY_HIGH,
		.extra = &axi_pwm_init,
	};

	struct xil_gpio_init_param gpio_extra_param = {
		.device_id = GPIO_DEVICE_ID,
		.type = GPIO_PS,
	};

	struct gpio_init_param ad469x_resetn = {
		.number = GPIO_RESETN_1,
		.extra = &gpio_extra_param
	};

	struct spi_init_param spi_init = {
		.chip_select = AD469x_SPI_CS,
		.max_speed_hz = 80000000,
		.mode = SPI_MODE_3,
		.platform_ops = &spi_eng_platform_ops,
		.extra = (void*)&spi_eng_init_param,
	};

	struct ad469x_init_param ad469x_init_param = {
		.spi_init = &spi_init,
		.offload_init_param = &spi_engine_offload_init_param,
		.trigger_pwm_init = &trigger_pwm_init,
		.gpio_resetn = &ad469x_resetn,
		.clkgen_init = &clkgen_init,
		.axi_clkgen_rate = 160000000,
		.reg_access_speed = 20000000,
		.reg_data_width = 8,
		.capture_data_width = 16,
		.dev_id = ID_AD4696, /* dev_id */
	};

	ret = ad469x_init(&dev, &ad469x_init_param);
	if (ret < 0)
		return ret;

	while (1) {
		ch = j % 2;
		j++;
		ad469x_read_data(dev, ch, buf, AD469x_EVB_SAMPLE_NO);
		for (i = 0; i < AD469x_EVB_SAMPLE_NO; i++) {
			printf("ADC %"PRIu32": %"PRIu32" \n", i, buf[i]);
		}
	}

	printf("Success\n\r");
}
