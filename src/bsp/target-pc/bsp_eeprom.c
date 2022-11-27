#include <stdio.h>
#include <stdbool.h>
#include "bsp_eeprom.h"


#define SIMULATED_EEPROM_FILE "./simulated_eeprom.txt"
#define MAX_ADDRESSES          128

eeprom_result_t BSP_eeprom_init(){
    printf("EEPROM INIT\r\n");

    FILE *fp;
    fp = fopen(SIMULATED_EEPROM_FILE, "a");
    fclose(fp);

    return EEPROM_OK;

}

eeprom_result_t BSP_eeprom_read(uint16_t address, uint16_t* data){

    if (address > MAX_ADDRESSES){
        printf("EEPROM INVALID ADDRESS %hu\r\n", address);
        *data = 0;
        return EEPROM_ERROR;
    }

    FILE *fp;
    fp = fopen(SIMULATED_EEPROM_FILE, "r");

    char line[MAX_ADDRESSES]; 

    // find address to read
    while (fgets(line, MAX_ADDRESSES, fp) != NULL){
        uint16_t read_address;
        uint16_t read_data;
        sscanf(line, "%hu : %hu", &read_address, &read_data);
        if (read_address == address){
            *data = read_data;
            printf("EEPROM read %hu from address %hu\r\n", *data, address);
            fclose(fp);
            return EEPROM_OK;
        }
    }

    fclose(fp);
    *data = 0;
    printf("EEPROM_DATA_NOT_FOUND at addr %d\r\n", address);
    return EEPROM_DATA_NOT_FOUND;
}


eeprom_result_t BSP_eeprom_write(uint16_t address, uint16_t data){

    if (address > MAX_ADDRESSES){
        printf("EEPROM INVALID ADDRESS %hu\r\n", address);
        return EEPROM_ERROR;
    }

    printf("EEPROM Write %hu in address %hu\r\n", data, address);

    FILE *fp, *fp_aux;
    fp = fopen(SIMULATED_EEPROM_FILE, "r+");
    fp_aux = fopen("./aux.txt", "w");

    bool address_in_file = false;
    char line[MAX_ADDRESSES]; 

    // find address to write
    while (fgets(line, MAX_ADDRESSES, fp) != NULL){
        uint16_t read_address;
        sscanf(line, "%hu : ", &read_address);
        if (read_address == address){
            uint8_t buf[32];
            sprintf((char * restrict)buf, "%hu : %hu\r\n", address, data);
            address_in_file = true;
            fputs((char * restrict)buf, fp_aux);
        } else {
            fputs(line, fp_aux);
        }
    }


    if (!address_in_file){
        uint8_t buf[32];
        sprintf((char * restrict)buf, "\r\n%hu : %hu", address, data);
        fputs((char * restrict)buf, fp_aux);
    }

    fclose(fp);
    fclose(fp_aux);


    remove(SIMULATED_EEPROM_FILE);
    rename("./aux.txt", SIMULATED_EEPROM_FILE);


    return EEPROM_OK;



}
