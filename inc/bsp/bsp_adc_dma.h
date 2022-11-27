#ifndef BSP_ADC_DMA_H
#define BSP_ADC_DMA_H

#include <stdint.h>

#define ADC_DMA_CHANNELS    7
#define READINGS_PER_ADC    256

#define ADC_DMA_BUFFER_SIZE             ADC_DMA_CHANNELS * READINGS_PER_ADC
#define ADC_DMA_HALF_BUFFER_SIZE        (ADC_DMA_BUFFER_SIZE / 2)

typedef void( * bsp_adc_dma_callback_t) (uint32_t * out_data);

void BSP_ADC_DMA_Init();
void BSP_ADC_DMA_Start();
void BSP_ADC_DMA_Stop();
void BSP_ADC_DMA_Register_Callback(bsp_adc_dma_callback_t callback_function);

#endif /* BSP_ADC_DMA_H */
