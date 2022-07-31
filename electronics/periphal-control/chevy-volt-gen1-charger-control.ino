#include <mcp_can.h>
#include <SPI.h>
#define CAN0_INT 2

MCP_CAN CAN0(10);


int canInitialized  = -1;

const int CHARGER_HEARTBEAT_INTERVAL_MS = 30;
const int CHARGER_CONFIG_INTERVAL_MS = 500;

const int CHARGE_CURRENT_A_HEX = 0x10; // 0x8 is 0.4A, 0x10 is 0.8A. Multiply amps by 20 and convert to hex
const int CHARGE_TARGET_VOLTAGE = 382; // Multiply amps by 20 and convert to hex
const int CHARGE_END_CURRENT = 0.4; // Stop charging when it's at 0.4 amp

uint16_t Vol_temp=0;

unsigned long lastSwitchTime;
unsigned long lastHeartbeatTime;
unsigned long lastChargeConfigCommandTime;

byte heartbeatBytes[1] = { 0x03 }; // 0x01 only 12V charging, 0x02 only HV, 0x03 HV and 12V
byte chargeCommandBytes[4] = { 0x00, 0x00, 0x00, 0x00 };

long unsigned int rxId;
unsigned char len = 0;

uint8_t rxBufInt[8];
uint8_t rxBuf[8];

boolean indicatorLEDOn;
boolean isCharging = false;
boolean hasInitializedCharging = false;

int currentCommand = -1;

float HVcur=0;
float HVvol=0;
float ACcur=0;
float ACvol=0;
float LVcur=0;
float LVvol=0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }

  pinMode(CAN0_INT, INPUT);

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK){
    Serial.println("MCP2515 Initialized Successfully!");
    canInitialized = 1;
  } else {
    Serial.println("Error Initializing MCP2515...");
    canInitialized = 0;
  }

  CAN0.setMode(MCP_NORMAL);
}

void loop() {

  if (!digitalRead(CAN0_INT)) {
   CAN0.readMsgBuf(&rxId, &len, rxBuf);

   if ((rxId & 0x40000000) == 0x40000000) {
   
   } else {
    for (byte i = 0; i < len; i++) {
      rxBufInt[i] = (unsigned int) (rxBuf[i]);
    }

    if (rxId == 0x212) {
          uint16_t HVcur_temp = (uint16_t)(rxBuf[0]<<8 | rxBuf[1]);
          HVcur = (float)(HVcur_temp>>3)*0.05;
         
         uint16_t HVvol_temp = (uint16_t)((((rxBuf[1]<<8 | rxBuf[2]))>>1)&0x3ff);
           HVvol = (float)(HVvol_temp)*.5;
           
         uint16_t LVcur_temp = (uint16_t)(((rxBuf[2]<<8 | rxBuf[3])>>1)&0x00ff);
          LVcur = (float)(LVcur_temp)*.2;

         
          uint16_t LVvol_temp = (uint16_t)(((rxBuf[3]<<8 | rxBuf[4])>>1)&0x00ff);
          LVvol = (float)(LVvol_temp)*.1;

          if (HVcur < CHARGE_END_CURRENT) {
            isCharging = false;
          }
    }

    if (rxId == 0x30A) {
         uint16_t ACcur_temp = (uint16_t)((rxBuf[0]<<8 | rxBuf[1])>>4);
         ACcur = (float)(ACcur_temp)*0.2;
         
         uint16_t ACvol_temp = (uint16_t)(((rxBuf[1]<<8 | rxBuf[2])>>4)&0x00ff);
         ACvol = (float)(ACvol_temp)*2;
    }
   }
  }


  if (millis() - lastSwitchTime > 1000) {
    lastSwitchTime = millis();
    Serial.print("HV-V=");
    Serial.print(HVvol);
    Serial.print(", HV-A=");
    Serial.print(HVcur);
    Serial.print(", AC-V=");
    Serial.print(ACvol);
    Serial.print(", AC-A=");
    Serial.print(ACcur);
    Serial.print(", LV-V=");
    Serial.print(LVvol);
    Serial.print(", LV-A=");
    Serial.print(LVcur);
    Serial.print(", CANINIT=");
    Serial.println(canInitialized);
    if (ACvol >= 1 && !isCharging && !hasInitializedCharging) {
        // AC CONNECTED FOR THE FIRST TIME
        isCharging = true;
        hasInitializedCharging = true;
    }
  }


  if (millis() - lastHeartbeatTime > CHARGER_HEARTBEAT_INTERVAL_MS) {
    if (isCharging) {
      lastHeartbeatTime = millis();

      CAN0.sendMsgBuf(0x30E, 0, 1, heartbeatBytes);
    }
  }

  if (millis() - lastChargeConfigCommandTime > CHARGER_CONFIG_INTERVAL_MS) {
    if (isCharging) {
      lastChargeConfigCommandTime = millis();

      int Vol_temp=CHARGE_TARGET_VOLTAGE*2;
      chargeCommandBytes[0] = 0x40;
      chargeCommandBytes[1] = CHARGE_CURRENT_A_HEX;
      chargeCommandBytes[2] = highByte(Vol_temp);
      chargeCommandBytes[3] = lowByte(Vol_temp);
      CAN0.sendMsgBuf(0x304, 0, 4, chargeCommandBytes);
    }
  }
}