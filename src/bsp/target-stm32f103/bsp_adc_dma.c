

#include <stdbool.h>

#include "bsp_adc_dma.h"
#include "adc.h"
#include "dma.h"
#include "utils.h"

uint32_t dma_buffer[ADC_DMA_BUFFER_SIZE];

bsp_adc_dma_callback_t adc_dma_callback_function;


static void default_adc_dma_callback(uint32_t * out_data) {

}


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
    adc_dma_callback_function(&dma_buffer[0]);
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    adc_dma_callback_function(&dma_buffer[ADC_DMA_HALF_BUFFER_SIZE]);
}


void BSP_ADC_DMA_Init(void) {
    MX_ADC1_Init();
    HAL_ADCEx_Calibration_Start(&hadc1);
    BSP_ADC_DMA_Register_Callback(default_adc_dma_callback);
} 

void BSP_ADC_DMA_Start(void) {

    HAL_ADC_Start_DMA(&hadc1, dma_buffer, ADC_DMA_BUFFER_SIZE);
    
}

void BSP_ADC_DMA_Stop(void) {

    HAL_ADC_Stop_DMA(&hadc1);

}

void BSP_ADC_DMA_Register_Callback(bsp_adc_dma_callback_t callback_function){

    adc_dma_callback_function = callback_function;

}

