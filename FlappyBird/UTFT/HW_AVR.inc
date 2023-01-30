#define B_CLK 0
#define B_CS 1
#define B_DIN 2
#define B_DOUT 3
#define B_IRQ 4

#define P_CLK PORTB
#define P_CS PORTB
#define P_DIN PORTB
#define P_DOUT PORTB
#define P_IRQ PORTB

void URTouch::touch_WriteData(byte data)
{
	byte temp;

	temp=data;
	cbi(P_CLK, B_CLK);

	for(byte count=0; count<8; count++)
	{
		if(temp & 0x80)
			sbi(P_DIN, B_DIN);
		else
			cbi(P_DIN, B_DIN);
		temp = temp << 1; 
		cbi(P_CLK, B_CLK);                
		sbi(P_CLK, B_CLK);
	}
}

word URTouch::touch_ReadData()
{
	word data = 0;

	for(byte count=0; count<12; count++)
	{
		data <<= 1;
		sbi(P_CLK, B_CLK);
		cbi(P_CLK, B_CLK);                
		if (rbi(P_DOUT, B_DOUT))
			data++;
	}
	return(data);
}

