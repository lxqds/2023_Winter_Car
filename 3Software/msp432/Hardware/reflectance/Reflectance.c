#include "Reflectance.h"


void Reflectance_Init2(void)
{
	P5->DIR &= ~0xFF;
}
uint8_t Reflectance_Read2(void)
{
	uint8_t Result;
//	P5->DIR |= 0xFF;
//	P5->OUT |= 0xFF;
//	P5->DIR &= ~0xFF;
	Result = P5->IN;
	return (Result<<3);//将前三位挤占，后三位为0
}
void Reflectance_Init(void){
    // write this as part of Lab 6
  P6->DIR |= BIT0;
  P6->OUT &= ~BIT0;
  P7->DIR &= ~0xFF; // P7.0-7.7 input
}

uint8_t Reflectance_Read(uint32_t time){
static uint8_t result;
    // write this as part of Lab 6
  P6->OUT |= BIT0;  // Turn on IR light
  P7->DIR |= 0xFF;  // P7.0-7.7 output
  P7->OUT |= 0xFF;  // Set P7.0-7.7 high
  delay_us(10);
  P7->DIR &= ~0xFF; // P7.0-7.7 input
  delay_us(time);
  result = P7->IN;
  P6->OUT &= ~BIT0; // Turn off IR light

  return result;
}

uint8_t Reflectance_Center(uint32_t time){
    // write this as part of Lab 6
  int32_t position=0;
  position = Reflectance_Position(Reflectance_Read(8000));
  if(position>332 || position<-332) return 0;  // off road
  else if(position<0) return 1; // off to right
  else if(position>0) return 2; // off to left
  else return 3;    //on road
}

int32_t Reflectance_Position(uint8_t data){
    // write this as part of Lab 6
  uint8_t i=0;
  int32_t position=0, sum_b=0, sum_bw=0;
  const int32_t w[8] = {332,237,142,47,-47,-142,-237,-332};
  int8_t b[8] = {0,0,0,0,0,0,0,0};
  int32_t bw[8] = {0,0,0,0,0,0,0,0};

  if(!data) return 666;
  else{
    for(i=0;i<8;i++){
        b[i] = (data>>i)&0x01;
        sum_b += b[i];
        bw[i] = w[i]*b[i];
        sum_bw += bw[i];
    }
    position = sum_bw/sum_b;
    return position;
  }
}



void Reflectance_Start(void){
    // write this as part of Lab 10
  P6->OUT |= BIT0;  // Turn on IR light
  P5->DIR |= 0xFF;  // P7.0-7.7 output
  P5->OUT |= 0xFF;  // Set P7.0-7.7 high
  //delay_us(10);
  P5->DIR &= ~0xFF; // P7.0-7.7 input
}
uint8_t Reflectance_End(void){
    // write this as part of Lab 10
  uint8_t result;
  result = P5->IN;
//  P6->OUT &= ~BIT0; // Turn off IR light
  return result;
}
