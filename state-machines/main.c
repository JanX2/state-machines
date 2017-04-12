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

// States
void led_on();
void led_off();

// State pointer
#if 0
// The C syntax for function pointers is… hard to get used to and remember.
void (*state_fp)();
#else
// Same as above. Just a bit more readable and reusable.
typedef void (*state_fp_t)(void);
state_fp_t state_fp;
#endif

void led_on() {
	emit(" ☀︎ ");
	state_fp = led_off;
}

void led_off() {
	emit(" ◦ ");
	state_fp = led_on;
}

int main(int argc, const char *argv[]) {
	state_fp = led_on;
	
	while (true) {
		state_fp();
		
		millisecond_sleep(200);
	}
	
	return EXIT_FAILURE; // Unreachable.
}
