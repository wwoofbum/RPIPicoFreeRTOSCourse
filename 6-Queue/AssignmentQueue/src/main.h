#ifndef MAIN_H
#define MAIN_H


// Globally define CounterAction and CounterCmd
enum CounterAction {CounterOff, CounterOn, CounterBlink};
struct CounterCmd {
	CounterAction action;
	uint8_t count;
};

#endif