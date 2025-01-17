/***
 * Demo program to
 *
 * Jon Durrant
 * 15-Aug-2022
 */


#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/multicore.h"
#include "hardware/structs/sio.h"
#include <math.h>

#define TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED0_PAD		0
#define LED5_PAD	    5
#define LED2_PAD		2
#define LED3_PAD		3
#define LED4_PAD		4


//Blink Delay
#define DELAY			500


/***
 * Function to run on Core1
 */
void core1Main(void){

	printf("Main Core 1 task started %ld\n", sio_hw->cpuid);

	uint32_t r = 0x0F;

	uint8_t ledPads[4];
	ledPads[0] = LED2_PAD;
	ledPads[1] = LED3_PAD;
	ledPads[2] = LED4_PAD;
	ledPads[3] = LED5_PAD;

	for (int i = 0 ; i < 4; i++){
		gpio_init(ledPads[i]);
		gpio_set_dir(ledPads[i], GPIO_OUT);
		gpio_put(ledPads[i], 0);
	}				

/*	
	const uint ledPad = LED5_PAD;
	gpio_init(ledPad);

	gpio_set_dir(ledPad, GPIO_OUT);
*/
// Main Loop
	while(true) {

		for (int i = 0; i < 4; i ++){
			uint8_t m = 1 << i;
			if ( (r & m) > 0){
				gpio_put(ledPads[i], 1);
			} else {
				gpio_put(ledPads[i], 0);
			}
		}
		sleep_ms(DELAY);
/*
	while (true) { // Loop forever
		gpio_put(ledPad, 1);
		sleep_ms(DELAY);
		gpio_put(ledPad, 0);
		sleep_ms(DELAY);
*/
		if (multicore_fifo_pop_timeout_us(DELAY, &r)){
			printf("Received %ld\n", r);
		} else {
			printf("No pop\n");
		}
	}
}

/***
 * Function to run on Core 0
 */
void core0Main(void){

	printf("Main Core 0 task started %ld\n", sio_hw->cpuid);

	const uint ledPad = LED0_PAD;
	gpio_init(ledPad);

	gpio_set_dir(ledPad, GPIO_OUT);

	while (true) { // Loop forever
		gpio_put(ledPad, 1);
		sleep_ms(DELAY);
		gpio_put(ledPad, 0);
		sleep_ms(DELAY);

		uint32_t r = rand() & 0x0F;
		if (multicore_fifo_push_timeout_us	(r, 0)){
			printf("Sent %ld\n", r);
		} else {
			printf("No Push\n");
		}
	}
}


/***
 * Main
 * @return
 */
int main( void )
{
    stdio_init_all();
    sleep_ms(5000);
    printf("GO\n");

    multicore_launch_core1(core1Main);

    core0Main();



    return 0;
}
