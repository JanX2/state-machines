//
//  main.c
//  state-machines
//
//  Created by Jan on 12.04.17.
//  Copyright © 2017 Jan. All rights reserved.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const long millisecond = 1000000L;

void millisecond_sleep(long milliseconds) {
	nanosleep((const struct timespec[]){0, (milliseconds * millisecond)}, NULL);
}

void emit(const char *string) {
	printf("%s"
		   "\r", // Move the cursor back to the start of the current line. CLI support required.
		   string);
	
	fflush(stdout); // Force the output to be displayed.
}

void led_on() {
	emit(" ☀︎ ");
}

void led_off() {
	emit(" ◦ ");
}

typedef enum {
	LED_ON_2_CYCLES,
	LED_OFF_1_CYCLE,
	LED_ON_1_CYCLES,
	LED_OFF_3_CYCLES,
} state_t;

int main(int argc, const char *argv[]) {
	const int first_cycle = 1;
	int cycle = first_cycle;

	state_t state = LED_ON_2_CYCLES;
	
	while (true) {
		switch (state) {
			case LED_ON_2_CYCLES:
				led_on();
				if (cycle >= 2) {
					state = LED_OFF_1_CYCLE; // Turn LED off next round.
					cycle = first_cycle;
				}
				break;
				
			case LED_OFF_1_CYCLE:
				led_off();
				state = LED_ON_1_CYCLES; // LED back on next round.
				break;
				
			case LED_ON_1_CYCLES:
				led_on();
				state = LED_OFF_3_CYCLES;
				cycle = first_cycle;
				break;
				
			case LED_OFF_3_CYCLES:
				led_off();
				if (cycle >= 3) {
					state = LED_ON_2_CYCLES; // Restart cycle next round.
					cycle = first_cycle;
				}
				break;
				
			default:
				exit(EXIT_FAILURE);
				break;
		}
		
		millisecond_sleep(200);
		cycle++;
	}
	
	return EXIT_FAILURE; // Unreachable.
}
