// Radio 15.4 send app
//
// Sends wireless packets via the 802.15.4 radio

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin configurations
#include "microbit_v2.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_timer.h"

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "lsm303agr.h"

#include "nrf_802154.h"

#define PSDU_MAX_SIZE (127) // Max length of a packet
#define FCS_LENGTH (2) // Length of the Frame Control Sequence


APP_TIMER_DEF(my_timer_1);

// Global variables

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

// static void thing(void* _unused){

//   lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();
//   printf("accel x axis: %f\n", accel.x_axis);
//   printf("accel y axis: %f\n", accel.y_axis);
//   printf("accel z axis: %f\n\n", accel.z_axis);
// }
// callback fn when tx starts
void nrf_802154_tx_started(const uint8_t* p_frame) {
	printf("tx started\n");
}

// callback fn when tx fails
void nrf_802154_transmit_failed(const uint8_t* p_frame, nrf_802154_tx_error_t error) {
	printf("tx failed error %u!\n", error);
}

// callback fn for successful tx
void nrf_802154_transmitted_raw(const uint8_t* p_frame, uint8_t* p_ack, int8_t power, uint8_t lqi) {
	printf("frame was transmitted!\n");
}

// Radio 15.4 send app

// Include necessary header files...

// Define packet size and FCS length
#define PSDU_MAX_SIZE (127)
#define FCS_LENGTH (2)

// Function to send accelerometer data over 802.15.4 radio
void send_accel_data(void) {
    lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();
    printf("Accelerometer - X: %f, Y: %f, Z: %f\n", accel.x_axis, accel.y_axis, accel.z_axis);

    // Create packet with accelerometer data
    uint8_t pkt[PSDU_MAX_SIZE];
    memset(pkt, 0, sizeof(pkt)); // Clear packet buffer

    pkt[0] = 29 + FCS_LENGTH;       // Length for nrf_transmit (length of pkt + FCS)
    pkt[1] = 0x01;                  // Frame Control Field
    pkt[2] = 0xcc;                  // Frame Control Field
    pkt[3] = 0x00;                  // Sequence number
    pkt[4] = 0xff;                  // Destination PAN ID 0xffff
    pkt[5] = 0xff;                  // Destination PAN ID
    // Set source and destination extended addresses
    uint8_t src_extended_addr[] = {0xdc, 0xa9, 0x35, 0x7b, 0x73, 0x36, 0xce, 0xf4};
    uint8_t dst_extended_addr[] = {0x50, 0xb1, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};
    memcpy(&pkt[6], dst_extended_addr, 8);   // Destination extended address
    memcpy(&pkt[14], src_extended_addr, 8);  // Source extended address

    // Convert accelerometer data to integers for transmission
    int8_t x = (int8_t)(accel.x_axis * 100);
    int8_t y = (int8_t)(accel.y_axis * 100);
    int8_t z = (int8_t)(accel.z_axis * 100);

    // Store accelerometer data in the packet
    pkt[24] = (uint8_t)(x); // MSB of X-axis
    pkt[25] = (uint8_t)(y); // LSB of X-axis
    pkt[26] = (uint8_t)(z); // MSB of Y-axis
    // pkt[27] = (uint8_t)(y & 0xFF); // LSB of Y-axis
    // pkt[28] = (uint8_t)(z >> 8); // MSB of Z-axis
    // pkt[29] = (uint8_t)(z & 0xFF); // LSB of Z-axis

    // Send packet
    if (!nrf_802154_transmit_raw(pkt, true)) {
        printf("Failure to send radio packet!\n");
    } else {
        printf("Sent a radio packet with accelerometer data!\n");
    }
}

int main(void) {
    // Initialize peripherals and radio.
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = I2C_SCL;
  i2c_config.sda = I2C_SDA;
  i2c_config.frequency = NRF_TWI_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  lsm303agr_init(&twi_mngr_instance);
  nrf_gpio_cfg_output(LED_MIC);
  printf("About to init\n");
  nrf_802154_init();
  printf("Done with init\n");
  nrf_802154_channel_set(11);
  uint8_t src_pan_id[] = {0xcd, 0xab}; 
  nrf_802154_pan_id_set(src_pan_id);
  printf("Radio configured!\n");
    lsm303agr_init(&twi_mngr_instance);

    while (1) {
        // Send accelerometer data over the radio
        send_accel_data();

        // Toggle LED
        nrf_gpio_pin_toggle(LED_MIC);

        // Sleep
        nrf_delay_ms(10);
    }
}


// int main(void) {
//   printf("Board started!\n");



//   // Configure 154 radio
  
//    lsm303agr_measurement_t accel = lsm303agr_read_accelerometer();

//     // Create packet with accelerometer data
//   uint8_t src_extended_addr[] = {0xdc, 0xa9, 0x35, 0x7b, 0x73, 0x36, 0xce, 0xf4};
//   nrf_802154_extended_address_set(src_extended_addr);
//   uint8_t dst_extended_addr[] = {0x50, 0xbe, 0xca, 0xc3, 0x3c, 0x36, 0xce, 0xf4};

//     uint8_t pkt[PSDU_MAX_SIZE];
//     pkt[0] = 29 + FCS_LENGTH;     // Length for nrf_transmit (length of pkt + FCS)
//     pkt[1] = 0x01;                 // Frame Control Field
//     pkt[2] = 0xcc;                 // Frame Control Field
//     pkt[3] = 0x00;                 // Sequence number
//     pkt[4] = 0xff;                 // Destination PAN ID 0xffff
//     pkt[5] = 0xff;                 // Destination PAN ID
//     memcpy(&pkt[6], dst_extended_addr, 8);    // Destination extended address
//     memcpy(&pkt[14], src_pan_id, 2);          // Source PAN ID
//     memcpy(&pkt[16], src_extended_addr, 8);   // Source extended address
//     pkt[24] = (uint8_t)(accel.x_axis * 100); // Convert to integer for transmission
//     pkt[25] = (uint8_t)(accel.y_axis * 100); // Convert to integer for transmission
//     // pkt[26] = (uint8_t)(accel.z_axis * 100); // Convert to integer for transmission
//     printf("%u", pkt[24]);
//     // Send packet
//     if (!nrf_802154_transmit_raw(pkt, true)) {
//         printf("Failure to send radio packet!\n");
//     } else {
//         printf("Sent a radio packet with accelerometer data!\n");
    
  // Addresses (source and destination)
  
//   // TX Packet
//   uint8_t pkt[PSDU_MAX_SIZE];
//   pkt[0] = 26 + FCS_LENGTH; /* Length for nrf_transmit (length of pkt + FCS) */
//   pkt[1] = 0x01; /* Frame Control Field */
//   pkt[2] = 0xcc; /* Frame Control Field */
//   pkt[3] = 0x00; /* Sequence number */
//   pkt[4] = 0xff; /* Destination PAN ID 0xffff */
//   pkt[5] = 0xff; /* Destination PAN ID */
//   memcpy(&pkt[6], dst_extended_addr, 8); /* Destination extended address */
//   memcpy(&pkt[14], src_pan_id, 2); /* Source PAN ID */
//   memcpy(&pkt[16], src_extended_addr, 8);/* Source extended address */ 
//   pkt[24] = 0xAA; /* Payload */
//   pkt[25] = 0xBB; /* */
//   pkt[26] = 0xCC; /* */


//   // Enter main loop.
//   while (1) {
//     // Send packet
//     if (!nrf_802154_transmit_raw(pkt, true)) {
//       printf("Failure to send radio packet!\n");
//     } else {
//     //   printf("Sent a radio packet!\n");
//        printf("%u", pkt[24]);
//     }

//         // Toggle LED
//         nrf_gpio_pin_toggle(LED_MIC);

//         // Sleep
//         nrf_delay_ms(500);
//   }
// }

