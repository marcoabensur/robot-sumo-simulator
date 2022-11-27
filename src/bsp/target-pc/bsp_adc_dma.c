

#include <stdbool.h>
#include <stdio.h>

#include "bsp_adc_dma.h"
#include "bsp_adc_fake.h"
#include "utils.h"

#include "qpc.h"  
#include "bsp.h"

#ifdef Q_SPY
#include "qs_defines.h"
#endif

#define FAKE_ADC_BATTERY_POSITION 0

typedef enum line_position_in_adc_fake 
{
    FAKE_LINE_POS_FR = 3, 
    FAKE_LINE_POS_FL = 2,
    FAKE_LINE_POS_BR = 5,      
    FAKE_LINE_POS_BL = 1,
} line_position_in_adc_fake;


bsp_adc_dma_callback_t adc_dma_callback_function;
static bool enabled;


static void default_adc_dma_callback(uint32_t * out_data) {
    Q_UNUSED_PAR(out_data);
}


void ADC_Fake_ConvCpltCallback(bool fl, bool fr, bool bl, bool br, bool battery_full) {

    if (!enabled){
        return;
    }

    uint32_t dma_buffer[ADC_DMA_HALF_BUFFER_SIZE];

    for (uint16_t i = 0; i < ADC_DMA_HALF_BUFFER_SIZE; i += ADC_DMA_CHANNELS) {
        for (uint16_t j = 0; j < ADC_DMA_CHANNELS; j++) {
            switch (j) {
                case FAKE_LINE_POS_FR:
                    dma_buffer[i + j] = 2000 * fr;
                    break;
                case FAKE_LINE_POS_FL:
                    dma_buffer[i + j] = 2000 * fl;
                    break;
                case FAKE_LINE_POS_BR:
                    dma_buffer[i + j] = 2000 * bl;
                    break;
                case FAKE_LINE_POS_BL:
                    dma_buffer[i + j] = 2000 * br;
                    break;
                case FAKE_ADC_BATTERY_POSITION:
                    dma_buffer[i + j] = 4095 * battery_full;
                    break;
                default:
                    dma_buffer[i + j] = 0;
                    break;
            }
        }
    }
    
    adc_dma_callback_function(&dma_buffer[0]);
}


void BSP_ADC_DMA_Init(void) {
    printf("ADC DMA Init \r\n"); 
    BSP_ADC_DMA_Register_Callback(default_adc_dma_callback);
}

void BSP_ADC_DMA_Start(void) {

    printf("ADC DMA Start \r\n"); 
    enabled = true;

}

void BSP_ADC_DMA_Stop(void) {

    printf("ADC DMA Stop \r\n"); 
    enabled = false;


}

void BSP_ADC_DMA_Register_Callback(bsp_adc_dma_callback_t callback_function){

    adc_dma_callback_function = callback_function;

}

