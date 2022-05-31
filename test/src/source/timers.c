
#include "timers.h"

/*
-----------------------------------------
    Timing 0.1s

    prescaler = AHB Clock / Timing Clock
              = 84(MHz) / 50(KHz)
              = 84000000(hz) / 50000(hz)
              = 1680

    T = 1 / f
      = 1(s) / f(Hz)
      = 1000000(us) / 50000(hz)
      = 20(us/times)

    Period = Timing / T
           = 0.1(s) / 20(us/times)
           = 100000(us) / 20(us/times)
           = 5000(times)

-----------------------------------------
    Timing 0.5s
    
    prescaler = AHB Clock / Timing Clock
              = 84(MHz) / 10(KHz)
              = 84000000(hz) / 10000(hz)
              = 8400

    T = 1 / f
      = 1(s) / f(Hz)
      = 1000000(us) / 10000(hz)
      = 100(us/times)

    Period = Timing / T
           = 0.5(s) / 100(us/times)
           = 500000(us) / 100(us/times)
           = 5000(times)
-----------------------------------------
*/

void timers_model_init(uint32_t times, uint32_t prescaler)
{
    TIM_TimeBaseInitTypeDef timer_init;

    RCC_APB1PeriphClockCmd(CURRENT_TIMER_CLOCK, ENABLE);

    // count times 0 - 4999 is 5000 times
    // timer_init.TIM_Period = 5000 - 1;
    timer_init.TIM_Period = times - 1;

    // TIMxCLK = 2 * PCLK1
    // PCLK1   = HCLK / 4
    // TIMxCLK = HCLK / 2 = SystemCoreClock / 2 = 84MHz
    // fosc    = TIMxCLK / (TIM_Prescaler + 1) = 10000Hz
    // timer_init.TIM_Prescaler = 8400 - 1;
    timer_init.TIM_Prescaler = prescaler - 1;

    TIM_TimeBaseInit(CURRENT_TIMER, &timer_init);
    TIM_ClearFlag(CURRENT_TIMER, TIM_FLAG_Update);
    TIM_ITConfig(CURRENT_TIMER,TIM_IT_Update,ENABLE);
    TIM_Cmd(CURRENT_TIMER, ENABLE); 
}

void timers_init(uint32_t times, uint32_t prescaler)
{
    nvic_init(NVIC_PriorityGroup_0, CURRENT_TIMER_IRQN, 0, 3);
    timers_model_init(times, prescaler);
}

void CURRENT_TIMER_IRQHANDLER()
{
    if (TIM_GetITStatus(CURRENT_TIMER, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(CURRENT_TIMER, TIM_IT_Update);
    }
}
