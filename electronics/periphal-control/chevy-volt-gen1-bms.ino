#include <mcp_can.h>
#include <SPI.h>

uint16_t Cell[97]; //cells, raw 0.00125/V
uint16_t Temp[24]; // temperatures 0.0556/C and -27.778
uint16_t celllow, cellhigh, templow, temphigh;

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
uint8_t rxBufInt[8];
char msgString[128];

uint32_t tlast = 0;
int counter = 0;

#define CAN0_INT 2
MCP_CAN CAN0(10);

byte dataRequestBytes[8] = {0x02, 0x00, 0x00};

void setup()
{
  Serial.begin(115200);
  
  if(CAN0.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);
}

void loop()
{
  if(!digitalRead(CAN0_INT))
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
  
    if((rxId & 0x40000000) == 0x40000000){
    } else {
      for(byte i = 0; i<len; i++){
          rxBufInt[i] = (unsigned int) (rxBuf[i]);
      }
      candecode(rxId, rxBufInt);
    }
  }


  if (tlast <  (millis() - 2000) && millis() > 2000) // 2hz loop
  {
    tlast = millis();
      transferCellVoltages();
      if (counter % 5 == 0) {
       sendDataRequest();
      }
      counter += 1;
  }
}

void sendDataRequest() {
  CAN0.sendMsgBuf(0x0200, 0, 3, dataRequestBytes);
}

void candecode(uint32_t canId, uint8_t* data)
{
  int t1 = (data[0] & 0x0F) << 8;
  int t1t = (data[0] & 0x03) << 8;
  int t2 = data[1];
  int t3 = (data[2] & 0x0F) << 8;
  int t3t = (data[2] & 0x03) << 8;
  int t4 = data[3];
  int t5 = (data[4] & 0x0F) << 8;
  int t6 = data[5];
  int t7 = (data[6] & 0x0F) << 8;
  int t8 = data[7];

  int x = 0;
  switch (canId)
  {
    case 0x460: //Module 1 cells 1-3
      Cell[1] = t1 + t2;
      Cell[2] = t3 + t4;
      Cell[3] = t5 + t6;
      break;
    case 0x470: //Module 1 cells 4-6
      Cell[4] = t1 + t2;
      Cell[5] = t3 + t4;
      Cell[6] = t5 + t6;
      break;
    case 0x461: //Module 1 cells 7-10
      Cell[7] = t1 + t2;
      Cell[8] = t3 + t4;
      Cell[9] = t5 + t6;
      Cell[10] = t7 + t8;
      break;
    case 0x471: //Module 1 cells 11-14
      Cell[11] = t1 + t2;
      Cell[12] = t3 + t4;
      Cell[13] = t5 + t6;
      Cell[14] = t7 + t8;
      break;
    case 0x462: //Module 1 cells 15-18
      Cell[15] = t1 + t2;
      Cell[16] = t3 + t4;
      Cell[17] = t5 + t6;
      Cell[18] = t7 + t8;
      break;
    case 0x472: //Module 1 cells 19-22
      Cell[19] = t1 + t2;
      Cell[20] = t3 + t4;
      Cell[21] = t5 + t6;
      Cell[22] = t7 + t8;
      break;
    case 0x463: //Module 1 cells 23-26
      Cell[23] = t1 + t2;
      Cell[24] = t3 + t4;
      Cell[25] = t5 + t6;
      Cell[26] = t7 + t8;
      break;
    case 0x473: //Module 1 cells 27-30
      Cell[27] = t1 + t2;
      Cell[28] = t3 + t4;
      Cell[29] = t5 + t6;
      Cell[30] = t7 + t8;
      break;
    case 0x464: //Module 2 cells 31-34
      Cell[31] = t1 + t2;
      Cell[32] = t3 + t4;
      Cell[33] = t5 + t6;
      Cell[34] = t7 + t8;
      break;
    case 0x474: //Module 2 cells 35-38
      Cell[35] = t1 + t2;
      Cell[36] = t3 + t4;
      Cell[37] = t5 + t6;
      Cell[38] = t7 + t8;
      break;
    case 0x465: //Module 2 cells 39-42
      Cell[39] = t1 + t2;
      Cell[40] = t3 + t4;
      Cell[41] = t5 + t6;
      Cell[42] = t7 + t8;
      break;
    case 0x475: //Module 2 cells 43-46
      Cell[43] = t1 + t2;
      Cell[44] = t3 + t4;
      Cell[45] = t5 + t6;
      Cell[46] = t7 + t8;
      break;
    case 0x466: //Module 2 cells 47-50
      Cell[47] = t1 + t2;
      Cell[48] = t3 + t4;
      Cell[49] = t5 + t6;
      Cell[50] = t7 + t8;
      break;
    case 0x476: //Module 2 cells 51-54
      Cell[51] = t1 + t2;
      Cell[52] = t3 + t4;
      Cell[53] = t5 + t6;
      Cell[54] = t7 + t8;
      break;
    case 0x468: //Module 3 cells 55-58
      Cell[55] = t1 + t2;
      Cell[56] = t3 + t4;
      Cell[57] = t5 + t6;
      Cell[58] = t7 + t8;
      break;
    case 0x478: //Module 3 cells 59-62
      Cell[59] = t1 + t2;
      Cell[60] = t3 + t4;
      Cell[61] = t5 + t6;
      Cell[62] = t7 + t8;
      break;
    case 0x469: //Module 3 cells 63-66
      Cell[63] = t1 + t2;
      Cell[64] = t3 + t4;
      Cell[65] = t5 + t6;
      Cell[66] = t7 + t8;
      break;
    case 0x479: //Module 3 cells 67-70
      Cell[67] = t1 + t2;
      Cell[68] = t3 + t4;
      Cell[69] = t5 + t6;
      Cell[70] = t7 + t8;
      break;
    case 0x46A: //Module 3 cells 71-74
      Cell[71] = t1 + t2;
      Cell[72] = t3 + t4;
      Cell[73] = t5 + t6;
      Cell[74] = t7 + t8;
      break;
    case 0x47A: //Module 3 cells 75-78
      Cell[75] = t1 + t2;
      Cell[76] = t3 + t4;
      Cell[77] = t5 + t6;
      Cell[78] = t7 + t8;
      break;

    case 0x46C: //Module 4 cells 79-81
      Cell[79] = t1 + t2;
      Cell[80] = t3 + t4;
      Cell[81] = t5 + t6;
      break;
    case 0x47C: //Module 4 cells 82-84
      Cell[82] = t1 + t2;
      Cell[83] = t3 + t4;
      Cell[84] = t5 + t6;
      break;
    case 0x46D: //Module 4 cells 85-87
      Cell[85] = t1 + t2;
      Cell[86] = t3 + t4;
      Cell[87] = t5 + t6;
      break;
    case 0x47D: //Module 4 cells 88-90
      Cell[88] = t1 + t2;
      Cell[89] = t3 + t4;
      Cell[90] = t5 + t6;
      break;
    case 0x46E: //Module 4 cells 91-93
      Cell[91] = t1 + t2;
      Cell[92] = t3 + t4;
      Cell[93] = t5 + t6;
      break;
    case 0x47E: //Module 4 cells 94-96
      Cell[94] = t1 + t2;
      Cell[95] = t3 + t4;
      Cell[96] = t5 + t6;
      break;

    case 0x7E0: //Module 1 Temp 1
      Temp[1] = t1t + t2;
      break;
    case 0x7E1: //Module 1 Temp 2
      Temp[2] = t1t + t2;
      break;
    case 0x7E2: //Module 1 Temp 3-4
      Temp[3] = t1t + t2;
      Temp[4] = t3t + t4;
      break;
    case 0x7E3: //Module 1 Temp 5
      Temp[5] = t1t + t2;
      break;

    case 0x7E4: //Module 2 Temp 6
      Temp[6] = t1t + t2;
      break;
    case 0x7E5: //Module 2 Temp 7-8
      Temp[7] = t1t + t2;
      Temp[8] = t3t + t4;
      break;
    case 0x7E6: //Module 2 Temp 9
      Temp[9] = t1t + t2;
      break;

    case 0x7E8: //Module 3 Temp 10
      Temp[10] = t1t + t2;
      break;
    case 0x7E9: //Module 3 Temp 11-12
      Temp[11] = t1t + t2;
      Temp[12] = t3t + t4;
      break;
    case 0x7EA: //Module 3 Temp 13
      Temp[13] = t1t + t2;
      break;

    case 0x7EC: //Module 4 Temp 14
      Temp[14] = t1t + t2;
      break;
    case 0x7ED: //Module 4 Temp 15
      Temp[15] = t1t + t2;
      break;
    case 0x7EE: //Module 4 Temp 16
      Temp[16] = t1t + t2;
      break;
  }
}


void transferCellVoltages()
{
  int x = 0;
  cellhigh = 0;
  celllow = 4000; //5V / 0.00125
  Serial.print("CELL,");
  for (int y = 1; y < 97; y++)
  {
    Serial.print(Cell[y] * 0.00125, 2);
    Serial.print(",");
    if (Cell[y] < celllow)
    {
      celllow = Cell[y];
    }
    if (Cell[y] > cellhigh)
    {
      cellhigh = Cell[y];
    }
    x++;
    if (x > 7)
    {
      x = 0;
    }
  }

  templow = 5000;
  temphigh = 0;
  x = 0;
  Serial.print("T,");
  for (int y = 1; y < 17; y++)
  {
    Serial.print((Temp[y] * 0.0556) - 27.778, 2);
    Serial.print(",");
    if (Temp[y] < templow)
    {
      templow = Temp[y];
    }
    if (Temp[y] > temphigh)
    {
      temphigh = Temp[y];
    }
    x++;
    if (x > 7)
    {
      x = 0;
    }
  }
  Serial.println("ENDCELL");
}

void printcells()
{
  int x = 0;
  cellhigh = 0;
  celllow = 4000; //5V / 0.00125
  Serial.println();
  Serial.println("Cell Voltages");
  for (int y = 1; y < 97; y++)
  {
    Serial.print(Cell[y] * 0.00125, 2);
    Serial.print(" | ");
    if (Cell[y] < celllow)
    {
      celllow = Cell[y];
    }
    if (Cell[y] > cellhigh)
    {
      cellhigh = Cell[y];
    }
    x++;
    if (x > 7)
    {
      x = 0;
      Serial.println();
    }
  }
  Serial.println();
  Serial.print("Lowest Cell: ");
  Serial.print(celllow * 0.00125, 2);
  Serial.print("  Highest Cell: ");
  Serial.print(cellhigh * 0.00125, 2);
  Serial.println();

  templow = 5000;
  temphigh = 0;
  x = 0;
  for (int y = 1; y < 17; y++)
  {
    Serial.print((Temp[y] * 0.0556) - 27.778, 2);
    Serial.print(" | ");
    if (Temp[y] < templow)
    {
      templow = Temp[y];
    }
    if (Temp[y] > temphigh)
    {
      temphigh = Temp[y];
    }
    x++;
    if (x > 7)
    {
      x = 0;
      Serial.println();
    }
  }
  Serial.println();
  Serial.print("Lowest Temp: ");
  Serial.print((templow * 0.0556) - 27.778, 2);
  Serial.print("  Highest Temp: ");
  Serial.print((temphigh * 0.0556) - 27.778, 2);
}
