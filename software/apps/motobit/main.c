// I2C sensors app
//
// Read from I2C accelerometer/magnetometer on the Microbit

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "app_timer.h"

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_twi_mngr.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "motobit.h"

#include "nrf_802154.h"

#define PSDU_MAX_SIZE (127) // Max length of a packet
#define FCS_LENGTH (2) // Length of the Frame Control Sequence

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_common_t sequence_data[1] = {0};

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};

APP_TIMER_DEF(my_timer_1);
APP_TIMER_DEF(my_timer_2);

// Global variables
// NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
NRF_TWI_MNGR_DEF(motobit_twi_mngr_instance, 1, 0);
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

uint8_t* packet;
int8_t directionL = 0;
int8_t directionR = 0;

static void control_motors(void* _unused){

  int8_t x = (int8_t)packet[24];
  int8_t y = (int8_t)packet[25];
  int8_t z = (int8_t)packet[26];

  volatile int8_t speed_R = 0;
  volatile int8_t speed_L = 0;

  int8_t directionL = 0;
  int8_t directionR = 0;

  // printf("x: %i\n", x);
  // printf("y: %i\n", y);

  if(y > 10){
      speed_R = y * 1.5;
    speed_L = speed_R;

  } else if(y < 10){
      speed_R = -y * 1.5;
      speed_L = speed_R;
      directionL = 1;
      directionR = 1;

  } else {
      speed_R = 0;
      speed_L = speed_R;

  }

  if(x > 10){
    speed_R = speed_R + x*0.5;
  } else if(x < -10){
    speed_L = speed_L + -x*0.5;
  }

  // printf("speed_R: %u\n", speed_R);
  // print  f("speed_L: %u\n", speed_L);

  if (speed_R >= 127){
    speed_R = 127;
  } else if (speed_R < -127){
    speed_R = -127;
  }
  
  if (speed_L >= 127) {
    speed_L = 127;
  } else if (speed_L <= -127){
    speed_L = -127;
  }

  printf("speed_L: %i:\n", speed_L);
  printf("speed_R: %i:\n", speed_R);
  printf("x: %i\n", x);
  printf("y: %i\n", y);
  printf("z: %i\n\n", z);

  motobit_drive(CMD_SPEED_LEFT, speed_R, directionL);
  motobit_drive(CMD_SPEED_RIGHT, speed_L, directionR);
}

static void pwm_init(void) {
  // Initialize the PWM
  // SPEAKER_OUT is the output pin, mark the others as NRFX_PWM_PIN_NOT_USED
  // Set the clock to 500 kHz, count mode to Up, and load mode to Common
  // The Countertop value doesn't matter for now. We'll set it in play_tone()
  // TODO
  nrfx_pwm_config_t pwm_config = {
    .output_pins = {EDGE_P16, \
    NRFX_PWM_PIN_NOT_USED, \
    NRFX_PWM_PIN_NOT_USED, \
    NRFX_PWM_PIN_NOT_USED}, \
    .irq_priority = NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY, \
    .base_clock = (nrf_pwm_clk_t) NRF_PWM_CLK_500kHz,
    .count_mode = (nrf_pwm_mode_t) NRF_PWM_MODE_UP,
    .top_value = NRFX_PWM_DEFAULT_CONFIG_TOP_VALUE,
    .load_mode = (nrf_pwm_dec_load_t) NRF_PWM_LOAD_COMMON,
    .step_mode = (nrf_pwm_dec_step_t) NRF_PWM_STEP_AUTO
  };
  nrfx_pwm_init(&PWM_INST, &pwm_config, NULL);
}
 
void set_gripper_position(uint16_t frequency, uint16_t duty_cycle) {
    uint16_t ticks_cycle = 500000 / frequency;
    NRF_PWM0->COUNTERTOP = ticks_cycle - 1;

    sequence_data[0] = (ticks_cycle * duty_cycle) / 100;
    pwm_sequence.values.p_common = sequence_data;

    nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);
}

static void gripper(void* _unused) {
  int8_t ir_temp = ir_read_temperature();

  if (ir_temp > 90) {
    set_gripper_position(1000, 1);
  } else {
    set_gripper_position(1000, 60);
  }
}

// callback fn for successful rx
void nrf_802154_received_raw(uint8_t* p_data, int8_t power, uint8_t lqi) {
  nrf_gpio_pin_toggle(20);
  printf("Packet: [ ");
  for (int i=0; i<p_data[0]-2; i++) {
    // printf("%02X ", p_data[i]);
  }
  printf("]\n");
  nrf_802154_buffer_free_raw(p_data);

  packet = p_data;
}

int main(void) {
  printf("Board started!\n");

  // initialize radio
  nrf_gpio_cfg_output(LED_MIC);

  // Configure 154 radio
  printf("About to init\n");
  nrf_802154_init();
  printf("Done with init\n");
  nrf_802154_channel_set(11);
  nrf_802154_auto_ack_set(false);
  nrf_802154_promiscuous_set(true);
  uint8_t src_pan_id[] = {0xcd, 0xab}; 
  nrf_802154_pan_id_set(src_pan_id);
  printf("Radio configured!\n");

  // Addresses (source and destination)
  uint8_t extended_addr[] = {0x50, 0xb1, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};
  nrf_802154_extended_address_set(extended_addr);


  if (nrf_802154_receive()) {
    printf("Entered receive mode\n");
  } else {
    printf("Could not enter receive mode\n");
  }

  // initialize motobit
  nrf_drv_twi_config_t motobit_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  motobit_config.scl = EDGE_P19;
  motobit_config.sda = EDGE_P20;
  motobit_config.frequency = NRF_TWI_FREQ_100K;
  motobit_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&motobit_twi_mngr_instance, &motobit_config);

  // initialize gripper
  pwm_init();

  // initialize app timers
  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, control_motors);
  app_timer_start(my_timer_1, 32768/100, NULL);

  app_timer_create(&my_timer_2, APP_TIMER_MODE_REPEATED, gripper);
  // app_timer_start(my_timer_2, 50/32768, NULL);
  app_timer_start(my_timer_2, 32768/2, NULL); 


  // set initial state of the gripper
  set_gripper_position(1000, 1);

  // initialize the ir sensor
  ir_enable(&motobit_twi_mngr_instance);

  // enable the motobit
  printf("Enabling MotoBit...\n");
  motobit_enable(&motobit_twi_mngr_instance);
  nrf_delay_ms(1000);
  printf("MotoBit enabled.\n");


  // Loop forever
  while (1) {
  }
}

