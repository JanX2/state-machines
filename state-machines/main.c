//
//  main.c
//  state-machines
//
//  Created by Jan on 12.04.17.
//  Copyright © 2017 Jan. All rights reserved.
//

#include <assert.h>
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

typedef enum {
	STATE_LED_ON_2_CYCLES,
	STATE_LED_OFF_1_CYCLE,
	STATE_LED_ON_1_CYCLE,
	STATE_LED_OFF_3_CYCLES,
	STATE_COUNT // Needs to be the last entry.
} state_t;

bool is_valid_state(state_t state) {
	return ((STATE_LED_ON_2_CYCLES <= state) && (state < STATE_COUNT));
}

typedef void (*action_t)(void);

typedef struct {
	action_t action;
	int cycle_count;
	state_t next_state;
} state_element_t;

// Evil globals.
// Could be encapsulated in a state machine state struct that is passed around.
state_t _current_state;

const int first_cycle = 1;
int _cycle = first_cycle;


// Actions
void nil_action() {
	exit(EXIT_FAILURE); // Should never be triggered.
}

void led_on() {
	emit(" ☀︎ ");
}

void led_off() {
	emit(" ◦ ");
}


state_element_t stateMatrix[STATE_COUNT] = {
	/* STATE_LED_ON_2_CYCLES  */ {led_on,  2, STATE_LED_OFF_1_CYCLE, },
	/* STATE_LED_OFF_1_CYCLE  */ {led_off, 1, STATE_LED_ON_1_CYCLE,  },
	/* STATE_LED_ON_1_CYCLE   */ {led_on,  1, STATE_LED_OFF_3_CYCLES,},
	/* STATE_LED_OFF_3_CYCLES */ {led_off, 3, STATE_LED_ON_2_CYCLES, },
};

void state_evaluation() {
	assert(is_valid_state(_current_state));
	
	// Determine the state matrix element for the current state.
	state_element_t state_transition = stateMatrix[_current_state];
	
	if (_cycle >= state_transition.cycle_count) {
		_cycle = first_cycle;
		// Transition to the next state (set current state to the next state obtained from the matrix)…
		_current_state = state_transition.next_state;
	}
	
	if ((_cycle == first_cycle) &&
		(state_transition.action != nil_action)) {
		// … and trigger the appropriate action.
		state_transition.action();
	}
	
	_cycle += 1;
}


int main(int argc, const char *argv[]) {
	_current_state = STATE_LED_ON_2_CYCLES;
	
	while (true) {
		state_evaluation();
		
		millisecond_sleep(200);
	}
	
	return EXIT_FAILURE; // Unreachable.
}
