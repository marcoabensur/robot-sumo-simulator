#include "bsp_eeprom.h"
#include "eeprom.h"


uint16_t VirtAddVarTab[EEPROM_NB_OF_VAR];


eeprom_result_t BSP_eeprom_init(){

    for (int i = 0; i < EEPROM_NB_OF_VAR; i++) {
        VirtAddVarTab[i] = i;
    }
    
    HAL_FLASH_Unlock();
    uint16_t ret = EE_Init();
    HAL_FLASH_Lock();

    if (ret == 0){
        return EEPROM_OK;
    }

    return EEPROM_ERROR;

}

eeprom_result_t BSP_eeprom_read(uint16_t address, uint16_t* data){

    if (address >= EEPROM_NB_OF_VAR){
        *data = 0;
        return EEPROM_ERROR;
    }

    HAL_FLASH_Unlock();
    uint16_t ret = EE_ReadVariable(VirtAddVarTab[address], data);
    HAL_FLASH_Lock();


    if (ret == 0){
        return EEPROM_OK;
    }

    *data = 0;
    return EEPROM_ERROR;


}

eeprom_result_t BSP_eeprom_write(uint16_t address, uint16_t data){

    if (address >= EEPROM_NB_OF_VAR){
        return EEPROM_ERROR;
    }

    HAL_FLASH_Unlock();
    uint16_t ret = EE_WriteVariable(VirtAddVarTab[address], data);
    HAL_FLASH_Lock();

    if (ret == 0){
        return EEPROM_OK;
    }

    return EEPROM_ERROR;

}