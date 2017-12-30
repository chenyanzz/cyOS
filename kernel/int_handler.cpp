#include "stdio.h"

extern "C"
{

void general_protection_fault_handler()
{
	printf("${RED}fatal:\tAn fatal error occured!  Please reboot your computer!\n");
	// while(true);
}

}