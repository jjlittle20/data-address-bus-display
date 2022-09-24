int ioSelect = 5;    //Pin 15
int clockPulse = 3;  //Pin 7
int dataOut = 4;     // pin 13

int addrOut = 6;
//595
int latchPin = 8;
int clockPin = A0;
int dataPin = 11;

int addrPin = 10;

int cpuClockInterruptPin = 2;

int DataClockPin = 12;


int value;
int addrValue;

byte switchVar = 0;
byte switchAddrVar = 0;

int getCharacter(char c) {
  Serial.println(c);
  if (c == '0') {
    return 252;
  } else if (c == '1') {
    return 96;
  } else if (c == '2') {
    return 218;
  } else if (c == '3') {
    return 242;
  } else if (c == '4') {
    return 102;
  } else if (c == '5') {
    return 182;
  } else if (c == '6') {
    return 190;
  } else if (c == '7') {
    return 224;
  } else if (c == '8') {
    return 254;
  } else if (c == '9') {
    return 230;
  } else if (c == 'a') {
    return 238;
  } else if (c == 'b') {
    return 62;
  }

  else if (c == 'c') {
    return 156;
  } else if (c == 'd') {
    return 122;
  } else if (c == 'e') {
    return 158;
  } else if (c == 'f') {
    return 142;
  } else if (c == ' ') {
    return 252;
  } else {
    return 252;
  }
};


void setup() {

  pinMode(ioSelect, OUTPUT);
  pinMode(clockPulse, OUTPUT);
  pinMode(dataOut, INPUT_PULLUP);
  pinMode(addrOut, INPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(addrPin, OUTPUT);

  pinMode(DataClockPin, OUTPUT);
  pinMode(cpuClockInterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(cpuClockInterruptPin), getLineValues, RISING);
  Serial.begin(9600);
  Serial.println("ready");
  getLineValues();
}
void getLineValues() {

  uint16_t addrIn = 0;
  byte dataIn = 0;
  digitalWrite(ioSelect, 0);    // enables inputs
  digitalWrite(clockPulse, 0);  // start pin low
  digitalWrite(clockPulse, 1);  // set clock pin high, data goes in SR
  digitalWrite(ioSelect, 1);    // deisable the parallel serial output

  // we enabled the SR to take in data

  for (int j = 0; j < 16; j++) {  //sets a value from 0-7
    if (j < 8) {
      value = digitalRead(dataOut);
    }
    //reads the data from SR
    addrValue = digitalRead(addrOut);
    if (value) {
      int a = (1 << j);

      dataIn = dataIn | a;
    }
    if (addrValue) {
      int a = (1 << j);  // shifts bit to its proper place in sequence.
      /*for more information see Arduino "BitShift" */
      addrIn = addrIn | a;  //combines data from shifted bits to form a single 8-bit number
      /*for more information see Arduino "Bitwise operators" */
    }
    digitalWrite(clockPulse, LOW);
    digitalWrite(clockPulse, HIGH);
  }

  uint8_t reverseData = 0;
  for (int i = 0; i < 8; i++) {
    uint8_t bit = (dataIn & (1 << i)) > 0 ? 1 : 0;
    reverseData |= bit << (7 - i);
  }

  uint16_t reverseAddr = 0;
  for (int i = 0; i < 16; i++) {
    uint16_t bit = (addrIn & (1 << i)) > 0 ? 1 : 0;
    reverseAddr |= bit << (15 - i);
  }


  String dataString = String(reverseData, HEX);
  String addrString = String(reverseAddr, HEX);

  Serial.println("addr string:");
  Serial.println(addrIn, BIN);
  Serial.println(addrIn, HEX);
  Serial.println(addrString);

  Serial.println(reverseAddr, HEX);
  Serial.println("data string:");
  Serial.println(dataIn, BIN);
  Serial.println(reverseData, HEX);

  if (dataString.length() == 1) {
    dataString = "0" + dataString;
  }
  if (addrString.length() == 1) {
    addrString = "000" + addrString;
  }
   if (addrString.length() == 2) {
    addrString = "00" + addrString;
  }
  if (addrString.length() == 3) {
    addrString = "0" + addrString;
  }

  int data0 = getCharacter(dataString.charAt(1));
  int data1 = getCharacter(dataString.charAt(0));

  int addr3 = getCharacter(addrString.charAt(3));
  int addr2 = getCharacter(addrString.charAt(2));
  int addr1 = getCharacter(addrString.charAt(1));
  int addr0 = getCharacter(addrString.charAt(0));




  Serial.println("data:");
  Serial.println(data0, BIN);
  Serial.println(data1, BIN);



  Serial.println("addr:");
  Serial.println(addr0, BIN);
  Serial.println(addr1, BIN);
  Serial.println(addr2, BIN);
  Serial.println(addr3, BIN);

  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, data1);
  shiftOut(dataPin, clockPin, MSBFIRST, data0);

  shiftOut(addrPin, DataClockPin, MSBFIRST, addr0);
  shiftOut(addrPin, DataClockPin, MSBFIRST, addr1);
  shiftOut(addrPin, DataClockPin, MSBFIRST, addr2);
  shiftOut(addrPin, DataClockPin, MSBFIRST, addr3);





  digitalWrite(latchPin, HIGH);
}


void loop() {
}