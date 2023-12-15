#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "nrf_delay.h"
#include "lsm303agr.h"
#include "motobit.h"  // Include I2C library or implement I2C functions


//=========================================================
//==================== I2C FUNCTIONS ======================
//=========================================================

static const nrf_twi_mngr_t* i2c_manager = NULL;

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 8-bit read value

uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);

  return rx_buf;
}

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  //TODO: implement me
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t array[2] = {reg_addr, data};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, array, 2, 0),
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
}

//=========================================================
//================= IR SENSOR FUNCTIONS ===================
//=========================================================

void ir_enable(const nrf_twi_mngr_t* i2c) {
    i2c_manager = i2c;
    i2c_reg_write(0x69, 0x00, 0x01);
    // uint8_t enable_cmd[2] = {CMD_ENABLE, 0x01};
    
    nrf_delay_ms(100);
     ; // Your I2C write function here
}

int8_t ir_read_temperature(void) {
  //TODO: implement me
  int8_t temp = i2c_reg_read(0x69, 0x80);
  // printf("%d\n", left)

  return temp;
}

//=========================================================
//================== MOTOBIT FUNCTIONS ====================
//=========================================================

void motobit_enable(const nrf_twi_mngr_t* i2c) {
    i2c_manager = i2c;
    // uint8_t enable_cmd[2] = {CMD_ENABLE, 0x01};
    
    nrf_delay_ms(100);
    i2c_reg_write(MOTOBIT_I2C_ADDR, 0x70,0x01); // Your I2C write function here
}

void motobit_disable() {
    // uint8_t disable_cmd[2] = {CMD_ENABLE, 0x00};
    i2c_reg_write(MOTOBIT_I2C_ADDR, 0x70,0x00); // Your I2C write function here
}

void motobit_drive(uint8_t cmd_speed, int speed, int invert) {
    uint8_t flags = 0;
    if (invert) {
        speed = -speed;
    }
    if (speed >= 0) {
        flags |= FORWARD_FLAG;
    }
    speed = (int)((float)speed / 100 * 127);
    if (speed < -127) {
        speed = -127;
    }
    if (speed > 127) {
        speed = 127;
    }
    speed = (speed & 0x7F) | flags;

    // uint8_t drive_cmd[2] = {cmd_speed, speed};
    i2c_reg_write(MOTOBIT_I2C_ADDR, cmd_speed,speed); // Your I2C write function here
}

void motobit_forward(int speed) {
    motobit_drive(CMD_SPEED_LEFT, speed, 0);
    motobit_drive(CMD_SPEED_RIGHT, speed, 0);
}

void motobit_reverse(int speed) {
    motobit_drive(CMD_SPEED_LEFT, speed, 1);
    motobit_drive(CMD_SPEED_RIGHT, speed, 1);
}

//=========================================================
//================== GRIPPER FUNCTIONS ====================
//=========================================================

