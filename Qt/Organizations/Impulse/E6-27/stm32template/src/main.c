#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
    PORT.GPIO_Pin = (GPIO_Pin_9 | GPIO_Pin_8);
    PORT.GPIO_Mode = GPIO_Mode_Out_PP;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( GPIOC , &PORT);
    
    /* Конфигурируем таймер SysTick на срабатывание 1000 раз в секунду */
    SysTick_Config(SystemCoreClock / 1000);

    while(1)
    {
        GPIOC->ODR |= (GPIO_Pin_9 | GPIO_Pin_8);
	Delay(1000);
        GPIOC->ODR &= ~(GPIO_Pin_9 | GPIO_Pin_8);
	Delay(1000);
    }
}

int main(void)
{
    blink();
        
    return 0;
}
