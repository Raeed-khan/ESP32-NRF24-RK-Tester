#include "RF24.h"
#include <ezButton.h>
#include "esp_bt.h"
#include "esp_wifi.h"

const byte TOGGLE_SWITCH_PIN = 33;
const byte NRF_CE_PIN = 22;
const byte NRF_CSN_PIN = 21;

SPIClass *vspi_bus = nullptr;
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN, 16000000); 

byte channel_index = 45;  
unsigned int hop_direction = 0; 

ezButton toggleSwitch(TOGGLE_SWITCH_PIN);

void initHardwareSPI() {
  vspi_bus = new SPIClass(VSPI);
  vspi_bus->begin();
  
  if (radio.begin(vspi_bus)) {
    delay(1000);
    Serial.println("[SUCCESS] SPI Bus & NRF24L01 Hardware Initialized.");
    
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5); 
    radio.setAddressWidth(3); 
    
    // 1. Setting Maximum Power Level
    radio.setPALevel(RF24_PA_MAX, true);
    
    // 2. LONG RANGE OPTIMIZATION: Lower data rate increases receiver sensitivity and range
    // Options: RF24_1MBPS (Recommended for balance) or RF24_250KBPS (Maximum Range)
    radio.setDataRate(RF24_1MBPS); 
    
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    
    radio.startConstCarrier(RF24_PA_MAX, channel_index); 
  } else {
    Serial.println("[ERROR] NRF24L01 Hardware Not Found!");
    while(1); 
  }
}

void executionModeTwo() {
  if (hop_direction == 0) {
    channel_index += 2;
  } else {
    channel_index -= 2;
  }
 
  if ((channel_index > 79) && (hop_direction == 0)) {
    hop_direction = 1;
  } else if ((channel_index < 2) && (hop_direction == 1)) {
    hop_direction = 0;
  }
 
  radio.setChannel(channel_index);
  delayMicroseconds(800); // Optimized delay for long-range frequency locking
}

void executionModeOne() {
  for (int j = 0; j < 15; j++) {
    radio.setChannel(j);
    delayMicroseconds(400); // Stable blast timing
  }
}

void setup(void) {
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("[INFO] Hardware Power Optimized. Starting Setup...");
  
  toggleSwitch.setDebounceTime(50);
  initHardwareSPI();
}

void loop(void) {
  toggleSwitch.loop(); 
 
  if (toggleSwitch.isPressed())
    Serial.println("[MODE CHANGE] Mode ONE: Active (Stable Blast)");
  if (toggleSwitch.isReleased())
    Serial.println("[MODE CHANGE] Mode TWO: Active (Long Range Sweep)");
 
  int switchState = toggleSwitch.getState();
 
  if (switchState == HIGH) {
    executionModeTwo();
  } else {
    executionModeOne();
  }
}
