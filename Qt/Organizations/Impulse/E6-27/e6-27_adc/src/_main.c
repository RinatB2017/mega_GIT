#include <stm32f0xx.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>

volatile uint32_t msTicks;

void Delay(int ms)
{
    uint32_t curTicks = msTicks;
    while((msTicks - curTicks) < 1000);
}

void SysTick_Handler()
{
    msTicks++;
}

void blink(void)
{
    GPIO_InitTypeDef PORT;
    RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);
    PORT.GPIO_Pin = GPIO_Pin_1;
    PORT.GPIO_Mode = GPIO_Mode_OUT;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( GPIOA , &PORT);
    
    /* Конфигурируем таймер SysTick на срабатывание 1000 раз в секунду */
    SysTick_Config(SystemCoreClock / 1000);

    while(1)
    {
        GPIOC->ODR |= (GPIO_Pin_1);
	Delay(1000);
        GPIOC->ODR &= ~(GPIO_Pin_1);
	Delay(1000);
    }
}

int main(void)
{
    blink();
        
    return 0;
}
