#include "main.h"

/*
Why does 'A' have to be in apostrophes? 
What is the difference between an apostrophe and a quotation mark?
What are classes and objects; what exactly does :: do?
What is typedef and enum?
*/

pros::ADIDigitalOut pneumatics('A');
pros::Motor lock (4);

pros::Controller master (pros::E_CONTROLLER_MASTER);

pros::Distance distance_sensor_front (2);
pros::Distance distance_sensor_back (3);

void move_lock_wait(float distance){
	lock.tare_position();
	lock.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES); //degrees
	lock.move_absolute(distance, 10); //10 RPM; presumably? distance in degrees
	while ( !( lock.get_position() < distance + 5 && lock.get_position() > distance - 5 ) ){
		pros::delay(20);
	}
	//pros::lcd::set_text(3, "lock movement complete");
}

void on_center_button() {
	static bool pressed = false; //What is a static variable?
	pressed = !pressed;
	if(pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Code initialized");

	pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	//Variables
		int door_unlocked = 1; //1 is true and -1 is false
		int distance_sensor_last_used = 2; //0 is front, 1 is back, 2 is the initial state
		bool door_open = false;
	pros::lcd::set_text(2, "opcontrol initialized");
	while (true){
		//Lock AND unlock the door
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1){
			//pros::lcd::set_text(3, "A was pressed");
			move_lock_wait(-70.0 * door_unlocked);
			door_unlocked = -1 * door_unlocked;
			if (door_unlocked == 1){
				pros::lcd::set_text(3, "door unlocked");
			}
			else
			{
				pros::lcd::set_text(3, "door locked");
			}
		}
		if (distance_sensor_front.get() < 200 && !(distance_sensor_last_used == 0)){
			distance_sensor_last_used = 0;
			door_open = !door_open;
			pneumatics.set_value(door_open);
		}
		if (distance_sensor_back.get() < 200 && !(distance_sensor_last_used == 1)){
			distance_sensor_last_used = 1;
			door_open = !door_open;
			pneumatics.set_value(door_open);
		}
		if (door_open == true){
			pros::lcd::set_text(4, "door open");
		}
		if (door_open == false){
			pros::lcd::set_text(4, "door open");
		}
		pros::delay(20);
	}
}