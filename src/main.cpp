#include "main.h"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Program initialized.");
	pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	pros::lcd::set_text(2, "opcontrol initialized.");
//Variables
	int sensor_activated = 2;
	/*
	0 = front (port 1) was just activated
	1 = back (port 2) was just activated
	2 = none of them have been activated
	*/
	bool door_open = false;
	int locked = 1; //1 is not locked; -1 is locked
	float activation_distance = 1000.0; //mm
	float lock_distance = 0.5; //turns
//Sensors, output and controller
	pros::adi::Pneumatics piston('A');
	piston.set_value(false);
	pros::Distance distance_front(1);
	pros::Distance distance_back(2);
	pros::Controller master (E_CONTROLLER_MASTER);
	pros::Motor lock (3);
//Loop
	while(true){
	//Prevent from using the same sensor to open and close the door AND open and close the door
		if(distance_front.get() < activation_distance && sensor_activated != 0){
			door_open = !door_open; //logical negation
			piston.set_value(door_open);
			sensor_activated = 0;
		}
		if(distance_back.get() < activation_distance && sensor_activated != 1){
			door_open = !door_open; //logical negation
			piston.set_value(door_open);
			sensor_activated = 1;
		}
	//Lock and unlock the door
		if(master.get_digital(E_CONTROLLER_DIGITAL_L1)){
			lock.move_absolute(0, lock_distance*locked); // Moves 100 units forward or backward
			while (!((lock.get_position() < lock_distance*locked + 0.1) && (lock.get_position() > lock_distance*locked - 0.1))) {
				// Continue running this loop as long as the motor is not within +-0.1 turns of its goal
				pros::delay(2);
			}
		}
	pros::delay(20); //msec
	}
}