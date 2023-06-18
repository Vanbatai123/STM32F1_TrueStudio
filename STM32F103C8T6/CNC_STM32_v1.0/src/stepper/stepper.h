/*
 * stepper.h
 *
 * Created: 10/26/2018 9:09:43 PM
 *  Author: PC
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_


// define pin for stepper
#define	PIN_DIR_X		A2
#define	PIN_STEP_X		A3
#define	PIN_DIR_Y		A4
#define	PIN_STEP_Y		A5

// define direction of movement
#define DIR_UP			1
#define DIR_UP_RIGHT	2
#define DIR_RIGHT		3
#define DIR_DOWN_RIGHT	4
#define DIR_DOWN		5
#define DIR_DOWN_LEFT	6
#define DIR_LEFT		7
#define DIR_UP_LEFT		8
#define DIR_STAND		0

void _delay_stepper(uint16_t i);
void stepper_config();
void stepper_run(uint8_t dir);
void stepper_ctrl(int8_t delta_x, int8_t delta_y);


#endif /* STEPPER_H_ */
