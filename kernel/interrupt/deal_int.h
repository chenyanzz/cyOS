#pragma once

/**
 * 中断响应的底层函数
 * 在deal_int.asm中实现
 * deal_*()  保存中断环境后，调用int_handler.cpp 中的C函数
 */


extern "C"
{

void deal_int_13();
void deal_int_0();
void deal_irq_0();
void deal_irq_1();

}

