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
	LED_ON,
	LED_OFF,
} state_t;

int main(int argc, const char *argv[]) {
	state_t state = LED_OFF;
	
	while (true) {
		switch (state) {
			case LED_OFF:
				led_on();
				state = LED_ON;
				break;
				
			case LED_ON:
				led_off();
				state = LED_OFF;
				break;
				
			default:
				exit(EXIT_FAILURE);
				break;
		}
		
		millisecond_sleep(200);
	}
	
	return EXIT_FAILURE; // Unreachable.
}
