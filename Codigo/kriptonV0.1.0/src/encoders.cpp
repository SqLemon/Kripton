#include "encoders.h"


void Timer2_init_Encoder(void)
    {   /* Explain function here */
                                    /* GPIO setup */
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable clock
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable clock

        /* ESTO NO LE FUNCABA  https://stackoverflow.com/questions/45066313/issues-with-timer2-on-stm32f103-in-encoder-interface-mode
        AFIO->MAPR |= (AFIO_MAPR_SWJ_CFG & 0b100); // Disable JTAG-DP and SW_DP to release PA15 and PB3
        AFIO->MAPR |= (AFIO_MAPR_TIM2_REMAP & 0b01); // Remap PA0->PA15, PA1->PB3 (needs to be 5V tol.)
        */


        // PA15 (Timer 2 Input CH1)
        GPIOA->CRH |= (GPIO_CRH_CNF15 & 0b01);  // 01 => Floating input (default)
        GPIOA->CRH &= ~GPIO_CRH_MODE15; // 00 => Input mode (default)

        // PB3 (Timer 2 Input CH2)
        GPIOB->CRL |= (GPIO_CRL_CNF3 & 0b01);   // 01 => Floating input (default)
        GPIOB->CRL &= ~GPIO_CRL_MODE3;  // 00 => Input mode (default)

                                    /* Timer setup */
        // Clock
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable clock
        TIM2->SMCR |= TIM_SMCR_SMS & 0b011; //011 => Encoder mode 3. Count on both TI1 and TI2 edges (gives 48 CPR)

        // Timebase unit
        TIM2->ARR = COUNTSPERREV - 1;   // CNT counts to this value and restarts
        TIM2->RCR = 0x0000;             // set repetition counter
        TIM2->CNT = 0;              // Set initial counter value (optional)

        // Control Register
        TIM2->CR1 |= TIM_CR1_URS;   // 1 => ONLY overflow/underflow generates interrupt
        TIM2->CR1 |= TIM_CR1_ARPE;  // 1 => ARR is updated at each UEV
        TIM2->CR1 &= ~TIM_CR1_UDIS; // 0 => Update Event (UEV) is generated at each overflow/underflow

        TIM2->CCMR1 |= TIM_CCMR1_CC1S & 0b01;   // 01 => CC1 Channel is configured as input
        TIM2->CCMR1 |= TIM_CCMR1_CC2S & 0b01;   // 01 => CC2 Channel is configured as input
        TIM2->CCMR1 &= ~TIM_CCMR1_IC1F; // 0000 => No Input Capture filter
        TIM2->CCMR1 &= ~TIM_CCMR1_IC2F; // 0000 => No Input Capture filter
        TIM2->CCER &= ~TIM_CCER_CC1P;   // 0 => Rising edge
        TIM2->CCER &= ~TIM_CCER_CC2P;   // 0 => Rising edge

        // Interrupts
        TIM2->DIER |= TIM_DIER_UIE; // DMA/Interrupt Register: Update Interrupt Enabled
        NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));  // enable interrupt globally

        TIM2->CR1 |= TIM_CR1_CEN;   // enable timer (counter begins one clock-cycle after enabling)
    }

void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_SR_UIF) == 1)
    {
        //si no me equivoco, aca el counter hizo overdlow --> vuelta entera
        TIM2->SR &= ~TIM_SR_UIF;    // clear UIF flag
    }
}


int getVal(void){
    return TIM2->CNT;
}
int prevVal = 0, val = 0;
long int initTime, endTime;

int getVel(){
    
    int vel;

    if(val == prevVal)
        initTime = micros();
    else if(val == (prevVal+1)){ 
        vel = 1765 * MAXSPEED / (initTime -micros()); //max speed -> intervalos de 1765us
        prevVal++;
    }
    else if(val == (prevVal-1)){ 
        vel = 1765 * (-MAXSPEED) / (initTime - micros()); //max speed -> intervalos de 1765us
        prevVal;
    }

    return vel;
    

}

