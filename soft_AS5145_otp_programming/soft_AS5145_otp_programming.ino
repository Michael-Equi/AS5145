// See https://ams.com/documents/20143/36005/AS5140_45_5245_AN000102_1-00.pdf/d4d35e92-cee7-9d6e-86c9-d8845921c0f7

#define pdio PB12
#define clk  PB13
#define csn  PB14

// For code compatibility with copied code
#define PPTRIMDelay 100 // 1/bitrate (ex 1/9600 ~ 1e-4 = 100 microseconds)

void setup() {
  pinMode(pdio, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(csn, OUTPUT);

  Serial.begin(115200);
  while (!Serial.isConnected());
  Serial.println("Begining programming");

  unsigned char SSIBuffer[3], PPTrimBuffer[7];
  unsigned char ZeroTemp;
  unsigned short angle;

  //  // Step 1: Read the 18 bit SSI value from the AS5145 (AS5140 uses 16 bit)
  //  ssiRead(SSIBuffer, 18);
  //  angle = extractAngleValueFromSsiBuffer(SSIBuffer);

  // Step 2: Read the 54 PPTRIM OTP bits from the AS5145
  pptrimRead(PPTrimBuffer, 54);
  for (int i = 0; i < 7; i++) {
    for(int b=0;b<8;b++){
      Serial.print((PPTrimBuffer[i]>>b)&0b1, BIN);
    }
  }
  Serial.println();

  //  // Step 3: Write the actual angle to the zero position field of the OTP bits
  //  ZeroTemp = (SSIBuffer[2] << 2) + (SSIBuffer[1] >> 6);
  //  ZeroTemp = reversebits(ZeroTemp);
  //  PPTrimBuffer[4] = ZeroTemp;
  //  ZeroTemp = (SSIBuffer[1] << 2) + (SSIBuffer[0] >> 6);
  //  ZeroTemp = reversebits(ZeroTemp);
  //  PPTrimBuffer[5] = ZeroTemp;

  // Step 4: Write back the 54 PPTRIM OTP bits containing the Zero position (which is
  //  the actual angle) to the AS5145
  //  pptrimWrite(PPTrimBuffer, 54);
  //
  //  // Step 5: Read the new angle. Must be 0 (+-1, if the mechanics is not very stable)
  //  ssiRead(SSIBuffer, 18);
  //  angle = extractAngleValueFromSsiBuffer(SSIBuffer);
}

void loop() {
  // put your main code here, to run repeatedly:

}
