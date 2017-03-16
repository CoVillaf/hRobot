#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"
#include <iostream>
#include <cstdio>
#include <vector>

#include "Arm.h"
#include "Addons.h"
#include "MotionManager.h"
#include "ErrorLog.h"

#include "GeoMath.h"
#include "UI_Buttons.h"
#include "UI_Labels.h"
#include "ParseCommand.h"
#include "MotorManager.h"

Arm::Arm(){}

bool Arm::PassInstruction(instruction_code instruction){
	return MotionManager::get().instruction(instruction);
}

void Arm::ArmInit(){

	motorManagerInit();
	sys.setSysLogDev(&devNull);
	sys.taskCreate(printfErrorTask);
	sys.taskCreate(comandInputTaskSerial, 1, 600, "ComITS");
	platform.begin(&RPi);
	platform.ui.configHandler = cfgHandler;
	platform.ui.onButtonEvent = onButtonEvent;
	platform.ui.onValueChangeEvent = onValueChangeEvent;
	platform.ui.setProjectId("68b5fe1f1473854f");
	sys.taskCreate(motorManagerUpdateTask, 2, 600, "MorManU");
	sys.taskCreate(MotionTask, 2, 1000, "MotManT");
	sys.taskCreate(taskPrintOnLabels, 2, 1500, "labelsT");
}

bool Arm::SET(char* Pt, type_co Co, float k1, float k2, float k3, float k4, float k5){
	char* temp;
	temp = new char[20];
    for(int i=0; i<20; i++){
        temp[i] = Pt[i];
    }
	if(Co == jointsCo){
		instruction_code code = {instruction_command::SET_J, temp, "", "", k1, k2, k3, k4, k5};
		return PassInstruction(code);
	}
	if(Co == cylindricalCo){
		instruction_code code = {instruction_command::SET_R, temp, "", "", k1, k2, k3, k4, k5};
		return PassInstruction(code);
	}
	if(Co == cartesianCo){
		instruction_code code = {instruction_command::SET_C, temp, "", "", k1, k2, k3, k4, k5};
		return PassInstruction(code);
	}
	return false;
}

bool Arm::MOVE(char* Pt){
	char* temp;
	temp = new char[20];
    for(int i=0; i<20; i++){
        temp[i] = Pt[i];
    }
	instruction_code code = {instruction_command::MOVE, temp, "", "", 0, 0, 0, 0, 0};
	return PassInstruction(code);
}

bool Arm::MOVES(char* Pt){
	char* temp;
	temp = new char[20];
    for(int i=0; i<20; i++){
        temp[i] = Pt[i];
    }
	instruction_code code = {instruction_command::MOVES, temp, "", "", 0, 0, 0, 0, 0};
	return PassInstruction(code);
}

bool Arm::DLY(float time){
	instruction_code code = {instruction_command::DELAY, "", "", "", time*1000, 0, 0, 0, 0};
	return PassInstruction(code);
}

bool Arm::DEPART(char* Pt, float distance){
	char* temp;
	temp = new char[20];
    for(int i=0; i<20; i++){
        temp[i] = Pt[i];
    }
	instruction_code code = {instruction_command::MOVE_D, temp, "", "", distance, 0, 0, 0, 0};
	return PassInstruction(code);
}

bool Arm::DEPARTS(char* Pt, float distance){
	char* temp;
	temp = new char[20];
    for(int i=0; i<20; i++){
        temp[i] = Pt[i];
    }
	instruction_code code = {instruction_command::MOVES_D, temp, "", "", distance, 0, 0, 0, 0};
	return PassInstruction(code);
}
