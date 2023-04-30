#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//char printdist[] = "Distance to the nearest object is: ";
//char centimeter[] = " cm";
//char newline[] = " \r\n";
volatile int temp[2];

volatile float diff_f;
volatile float diff_r;
volatile float diff_l;

volatile unsigned int i=0;
volatile unsigned int j=0;
volatile unsigned int k=0;

int dst_int_forward;
int dis_int_right;
int dis_int_left;

//char dst_char_f[5];
//char dst_char_r[5];
//char dst_char_l[5];

volatile float distance_f;
volatile float distance_r;
volatile float distance_l;

void ser_output(char *str);
void main(void)==
{
    WDTCTL = WDTPW | WDTHOLD;
    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1DIR = BIT5; //BUZZER PINS
    P1OUT =~BIT5;
    P1DIR = BIT6; // FOR FORWARD DIFF
    P2SEL = BIT1;
    //P1SEL = BIT1|BIT2|BIT6;
    //P1SEL2 = BIT1|BIT2;
    P2DIR = BIT0; // FOR LEFT DIFF
    P1SEL = BIT3;

    P2DIR = BIT3; // FOR RIGHT DIFF
    P2SEL = BIT5;

    UCA0CTL1 |= UCSWRST+UCSSEL_2; // FOR RESET AND SELECTING OF CLOCK(SM)
    UCA0BR0 = 52; //BAUD RATE 19200, 1MHZ
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;//CLEARING DATA BEFORE
    TA0CTL = TASSEL_2|MC_1 ; // SM CLOCK , UPMODE
    TA0CCR0 = 0xFFFF;
    TA0CCR1 = 0x000A;   //TO TRIGGER ULTRASONIC SENSOR PIN
    TA0CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2|MC_2 ; //for sm clock ,continuous mode

    *TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;
    _enable_interrupts();

    while(1){
        distance_f = diff_f/58;
        dst_int_forward = floor(distance_f);
        //ltoa(dst_int_forward, dst_char,10);
        ser_output(dst_int_forward);
        }
        ser_output(newline);
        __delay_cycles(500000);  //0.5 second delay

    while(1){
        distance_r = diff_r/58;
        dst_int_right = floor(distance_r);
        //ltoa(dst_int_forward, dst_char,10);
        ser_output(dst_int_right);
        }
        ser_output(newline);
        __delay_cycles(500000);

    while(1){
        distance_l = diff_l/58;
        dst_int_left = floor(distance_l);
        //ltoa(dst_int_forward, dst_char,10);
        ser_output(dst_int_left);
        }
        ser_output(newline);
        __delay_cycles(500000);


    if (dst_int_left & dst_int_right <150){
        P1OUT= BIT5;                    //FOR FORWARD- 1 BEEP
        __delay_cycles(2000000);
    }
    else if (dst_int_forward & dst_int_right <150){
        P1OUT = BIT5;                   //FOR LEFT-2 BEEPS
        __delay_cycles(4000000);
        P1OUT= ~BIT5;
        __delay_cycles(2000000);
        P1OUT = BIT5;
        __delay_cycles(4000000);
        P1OUT= ~BIT5;
        __delay_cycles(2000000);
    }
    else if (dst_int_forward & dst_int_left <150){
        P1OUT = BIT5;                   //FOR right-3 BEEPS
        __delay_cycles(4000000);
        P1OUT= ~BIT5;
        __delay_cycles(2000000);
        P1OUT = BIT5;
        __delay_cycles(4000000);
        P1OUT= ~BIT5;
        __delay_cycles(2000000);
        P1OUT = BIT5;
        __delay_cycles(4000000);
        P1OUT= ~BIT5;
        __delay_cycles(2000000);
        }
    else if (dst_int_forward & dst_int_left & dst_int_right <150){
        P1OUT = BIT5;
        __delay_cycles(7000000);
        P1OUT = ~BIT5;
    }
    else (dst_int_forward & dst_int_left & dst_int_right >150){
        P1OUT = ~BIT5;
    }
}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA1CCR1;
        i += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff_f=temp[i-1]-temp[i-2];
            i=0;
            }

__interrupt void Timer_A(void){
        temp[k] = TA1CCR1;
        k += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (j==2) {
            diff_l=temp[j-1]-temp[j-2];
            j=0;
            }
__interrupt void Timer_A(void){
        temp[k] = TA1CCR1;
        k += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (k==2) {
            diff_r=temp[k-1]-temp[k-2];
            k=0;
            }

}
void ser_output(char *str){

        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }






