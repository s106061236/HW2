#include "mbed.h"

Serial pc( USBTX, USBRX );
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8); // 7-seg output
DigitalIn  Switch(SW3); // buttom input
DigitalOut redLED(LED1); // LED output
DigitalOut greenLED(LED2); // LED output

AnalogOut Aout(DAC0_OUT); // DAC0 output
AnalogIn Ain(A0); // A0 input
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char TABLE[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

int sample = 1024;
int i;
float j;
float ADCdata[1024];

int main(){
    /*------- for FFT analysis -------*/
    for (i = 0; i < sample; i++)
    {
        Aout = Ain;
        ADCdata[i] = Ain;
        wait(1./sample);
    }

    for (i = 0; i < sample; i++)
    {
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.001);
    }
    /*------- initialize -------*/
    redLED = 0;
    greenLED = 0;
    int start = 0;
    int freq;
    int temp = 0;
    int k=0;
    int a[3];
    /*------- frequency counter -------*/    
    while(true){
        if(Ain==0) // catch the first trough
        {
            start = 1; // if the couter start or not
            k++; // the number of troughs
        }
        if(k==2) // if catch two troughs, it takes one cycle 
        {
            freq=(1000/temp); 
            a[0]=freq/100;
            a[1]=(freq%100)/10;
            a[2]=freq%10;
            break;
        }     
        if(start==1) // starting sampling to catch trough
        {
            temp++; // counter
            wait(0.001); // sampling rate
        }
    }
    /*------- sinwave generator & LED -------*/
    while(true)
    {
        for( j=0; j<2; j+=0.05 ) // generate a sinwave with same freq.
        {
            Aout = 0.5+0.5*sin(j*3.14159);
            wait(1./(freq*40));
        }
        if(Switch==1) // LED and 7-seg LED control
        {
            redLED=1;
            greenLED=0;
            display=0;
        }
        else
        {
            redLED = 0;
            greenLED=1;
            display = table[a[0]];
            wait(1);
            display = table[a[1]];
            wait(1);
            display = TABLE[a[2]];
            wait(1);
        }
    }
  }