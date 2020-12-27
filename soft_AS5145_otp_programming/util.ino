void SET_CLK() {
  digitalWrite(clk, HIGH);
}

void CLEAR_CLK() {
  digitalWrite(clk, LOW);
}

void SET_CSN() {
  digitalWrite(csn, HIGH);
}

void CLEAR_CSN() {
  digitalWrite(csn, LOW);
}

void SET_PROG() {
  digitalWrite(pdio, HIGH);
}

void CLEAR_PROG() {
  digitalWrite(pdio, LOW);
}

void PROG_HIGH_IMPED() {
  pinMode(pdio, INPUT);
}

void PROG_LOW_IMPED() {
  pinMode(pdio, OUTPUT);
}

int READ_PROG() {
  return digitalRead(pdio);
}

void pptrimDelay(unsigned int d) {
  delayMicroseconds(d);
}

void clkPulses(unsigned char num)
{
  unsigned char i;
  for (i = 0; i < num; i++)
  {
    SET_CLK(); pptrimDelay(PPTRIMDelay);
    CLEAR_CLK(); pptrimDelay(PPTRIMDelay);
  }
}
