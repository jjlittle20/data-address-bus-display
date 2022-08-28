int ioSelect = 5; //Pin 15
int clockPulse = 3; //Pin 7
int dataOut = 4; // pin 13
//595
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int cpuClockInterruptPin = 2;

int j;
int value;

byte switchVar = 0;

int getCharacter(char c) {
  Serial.println(c);
  if (c == '0') {
    return 252;
  }
  else if (c == '1') {
    return 96;
  }
  else if (c == '2') {
    return 218;
  }
  else if (c == '3') {
    return 242;
  }
  else if (c == '4') {
    return 102;
  }
  else if (c == '5') {
    return 182;
  }
  else if (c == '6') {
    return 190;
  }
  else if (c == '7') {
    return 224;
  }
  else if (c == '8') {
    return 254;
  }
  else if (c == '9') {
    return 230;
  }
  else if (c == 'a') {
    return 238;
  }
  else if (c == 'b') {
    return 62;
  }

  else if (c == 'c') {
    return 156;
  }
  else if (c == 'd') {
    return 122;
  }
  else if (c == 'e') {
    return 158;
  }
  else if (c == 'f') {
    return 142;
  }
  else if(c==' '){
    return 252;
    }
  else {
    return 2;
  }
};


void setup() {

  pinMode(ioSelect, OUTPUT);
  pinMode(clockPulse, OUTPUT);
  pinMode(dataOut,  INPUT_PULLUP);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(cpuClockInterruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(cpuClockInterruptPin), getLineValues, RISING);
  Serial.begin(9600);



}
void getLineValues() {

  byte dataIn = 0;
  digitalWrite(ioSelect, 0); // enables inputs
  digitalWrite(clockPulse, 0); // start pin low
  digitalWrite(clockPulse, 1); // set clock pin high, data goes in SR
  digitalWrite(ioSelect, 1); // deisable the parallel serial output

  // we enabled the SR to take in data

  for (j = 0; j < 8; j++) { //sets a value from 0-7
    value = digitalRead(dataOut) == 1 ? 0 : 1; //reads the data from SR


    if (value) {
      int a = (1 << j);

      dataIn = dataIn | a;

    }
    digitalWrite(clockPulse, LOW);
    digitalWrite(clockPulse, HIGH);
  }
  String x = String(dataIn, HEX);
  if(x.length()== 1){
    x = "0"+x;
    }

  int f = getCharacter(x.charAt(1));
  int j = getCharacter(x.charAt(0));
  Serial.println(f);
  Serial.println(j);
  Serial.println(f, BIN);
  Serial.println(j, BIN);
  digitalWrite(latchPin, LOW);
  // shift out the bits:
  shiftOut(dataPin, clockPin, MSBFIRST, j);
  shiftOut(dataPin, clockPin, MSBFIRST, f);
  //take the latch pin high so the LEDs will light up:
  digitalWrite(latchPin, HIGH);




}


void loop() {


}
