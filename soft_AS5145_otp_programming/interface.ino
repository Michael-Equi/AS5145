void initPPTRIM()
{
  PROG_LOW_IMPED();
  CLEAR_PROG();
}

void setupCondition() {
  CLEAR_CSN();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CLK();
  pptrimDelay(PPTRIMDelay);
  SET_PROG();
  pptrimDelay(PPTRIMDelay);
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CSN();
  pptrimDelay(PPTRIMDelay);
  SET_CLK();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CLK();
  pptrimDelay(PPTRIMDelay);
}

void exitCondition() {
  PROG_LOW_IMPED();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CSN();
  pptrimDelay(PPTRIMDelay);
  SET_CLK();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CLK();
  pptrimDelay(PPTRIMDelay);
  SET_CLK();
  pptrimDelay(PPTRIMDelay);
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  CLEAR_PROG();
  pptrimDelay(PPTRIMDelay);
}

void operationModeLoad()
{
  CLEAR_PROG();
  pptrimDelay(PPTRIMDelay);
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  clkPulses(4);
}

void operationModeRead()
{
  CLEAR_PROG();
  pptrimDelay(PPTRIMDelay);
  SET_CLK();
  pptrimDelay(PPTRIMDelay);
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CLK();
  pptrimDelay(PPTRIMDelay);
  clkPulses(1);
  PROG_HIGH_IMPED();
}

void operationModeWrite()
{
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  clkPulses(3);
}

void operationModeProg()
{
  SET_CLK();
  pptrimDelay(PPTRIMDelay);
  SET_CSN();
  pptrimDelay(PPTRIMDelay);
  CLEAR_CLK();
  pptrimDelay(PPTRIMDelay);
  clkPulses(4);
}

unsigned char reversebits(unsigned char val) // Endian switch
{
  unsigned char i = 0, result = 0;
  while (i < 8)
  {
    result += (val << i) & 0x80;
    if (i < 7) result = result >> 1;
    i++;
  }
  return result;
}

void pptrimLoad(unsigned char num_bits)
{
  setupCondition();
  operationModeLoad();
  clkPulses(num_bits);
  exitCondition();
}

void pptrimRead(unsigned char *buff, unsigned char num_bits)
{
  unsigned char current_byte = 0;
  unsigned char current_bit = 0;
  unsigned char temp = 0;
  if (!num_bits) return;
  current_byte = num_bits >> 3;
  current_bit = num_bits & ~0x07;
  setupCondition();
  operationModeRead();
  clkPulses(1); // position the first bit to read
  //-- read OTP Data --
  temp = 0;
  temp += READ_PROG();
  for (current_bit = num_bits; current_bit; current_bit--)
  {
    if (((current_bit - 1) & 0x07) == 0)
    {
      buff[current_bit >> 3] = temp;
      temp = 0;
    }
    if (current_bit)
    {
      temp <<= 1;
      SET_CLK();
      pptrimDelay(400);
      temp += READ_PROG();
      CLEAR_CLK();
      pptrimDelay(300);
    }
  }
  exitCondition();
}

void pptrimWrite(unsigned char *buff, unsigned char num_bits)
{
  unsigned char *current_byte;
  unsigned char current_bit = 0;
  unsigned char temp = 0;
  current_byte = buff + ((num_bits - 1) >> 3);
  temp = *current_byte;
  if (num_bits % 8)
    temp <<= 8 - (num_bits % 8);
  setupCondition();
  operationModeWrite();
  //-- send OTP Data
  for (current_bit = num_bits; current_bit; current_bit--)
  {
    if (temp & 0x80)
      SET_PROG();
    else
      CLEAR_PROG();
    pptrimDelay(200);
    SET_CLK();
    pptrimDelay(600);// delay, tzapp=2us(typ.)
    CLEAR_CLK();
    pptrimDelay(PPTRIMDelay);
    temp <<= 1;
    if (((current_bit - 1) & 0x07) == 0)
    {
      temp = *(--current_byte);
    }
  }
  SET_PROG();
  pptrimDelay(200);
  clkPulses(1); // data latched
  // END OTP-Write
  exitCondition();
}
