#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"
#include <iostream>
#include <cstdio>
#include <vector>

#include "CommandInput.h"
#include "Addons.h"
#include "MotionManager.h"
#include "ErrorLog.h"

#include "ParseCommand.h"

void clear_all(char* command, char* param1, char* param2, 
char* param3, char* param4, char* param5, char* param6, 
char* param7){
    for(int i = 0; i<20; i++){
			command[i] = 0;
			param1[i] = 0;
			param2[i] = 0;
			param3[i] = 0;
			param4[i] = 0;
			param5[i] = 0;
			param6[i] = 0;
			param7[i] = 0;
		}
}

CommandInput::CommandInput(Arm* t_robot){
    input_type = SERIAL;
    jog_type = jointsCo;
    robot = t_robot;
}

void CommandInput::changeInstructionInputType(instruction_input_type new_type, type_co jog_new_type){
    input_type = new_type;
    jog_type = jog_new_type;
}

void CommandInput::changeInstructionInputType(instruction_input_type new_type){
    input_type = new_type;
}

bool CommandInput::AddInstruction(instruction_code instruction, instruction_input_type from){
    if(from != input_type){
        return false;
    }
    else{
		if(instruction.comand == NOCOMMAND){
			ErrorLogs::err().send(30);
		    return false;
		}
		if(instruction.comand == CONFIG_COM_STRIM){
		    changeInstructionInputType(STREAM);
		    return true;
		}
		if(instruction.comand == CONFIG_COM_UI){
		    changeInstructionInputType(UI);
		    return true;
		}
		if(instruction.comand == CONFIG_COM_SERIAL){
		    changeInstructionInputType(SERIAL);
		    return true;
		}
		if(instruction.comand == CONFIG_COM_JOG){
		    changeInstructionInputType(JOG);
		    return true;
		}
		if(instruction.comand == CONFIG_COM_CODE){
		    changeInstructionInputType(CODE);
		    return true;
		}
		return robot->PassInstruction(instruction);
    }
}

bool CommandInput::AddInstruction(char* instruction, instruction_input_type from){
    if(from != input_type){
        return false;
    }
    else{
        char command[20];
		char param1[20];
		char param2[20];
		char param3[20];
		char param4[20];
		char param5[20];
		char param6[20];
		char param7[20];

		for(int i = 0; i<20; i++){
			command[i] = 0;
			param1[i] = 0;
			param2[i] = 0;
			param3[i] = 0;
			param4[i] = 0;
			param5[i] = 0;
			param6[i] = 0;
			param7[i] = 0;
		}

		int i = 0;
		int j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				command[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param1[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param2[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param3[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param4[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param5[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param6[j] = instruction[i];
			}
			i++;
			j++;
		}
		i++;
		j = 0;
		while(instruction[i] > 32){
			if(j < 20){
				param7[j] = instruction[i];
			}
			i++;
			j++;
		}
		return AddInstruction(CommandTranslation(command, param1, param2, param3, param4, param5, param6, param7), from);
    }
}

void CommandInput::AddInstructionStream(char* instruction, instruction_input_type from){
    if(from == input_type){
		int i = 0;
		int j = 0;
		while(instruction[i]>31){
			char* s;
			s = new char[255];
			for(int i = 0; i<160; i++){
				s[i] = 0;
			}
			if(instruction[i] == 32){
				i++;
			}
			while(instruction[i]!=59){
				s[j] = instruction[i];
				i++;
				j++;
			}
			i++;
			j = 0;
			AddInstruction(s, from);
			sys.delay(100);
		}
    }
}

instruction_code CommandInput::CommandTranslation(char* command, char* param1, char* param2, 
char* param3, char* param4, char* param5, char* param6, char* param7){
        char* temp;
        temp = new char[20];
        for(int i=0; i<20; i++){
            temp[i] = param1[i];
        }
        if (strcmp(command, "MOVE") == 0)
	    {   
	        if (strcmp(param2, "") == 0){
            instruction_code code = {MOVE, temp, 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
			if (strcmp(param2, "D") == 0){
            instruction_code code = {MOVE_D, temp, (float)atof(param3), 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
	        if (strcmp(param2, "JI") == 0){
            instruction_code code = {MOVE_JI, "", (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
	        if (strcmp(param2, "CI") == 0){
            instruction_code code = {MOVE_CI, "", (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
	        if (strcmp(param2, "JN") == 0){
            instruction_code code = {MOVE_JN, "", (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
	        if (strcmp(param2, "CN") == 0){
            instruction_code code = {MOVE_CN, "", (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	        }
	    }
	    if (strcmp(command, "MOVES") == 0)
	    {
			if (strcmp(param2, "") == 0){
            	instruction_code code = {MOVES, temp, 0, 0, 0, 0, 0};
            	clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            	return code;
			}
			if (strcmp(param2, "D") == 0){
            	instruction_code code = {MOVES_D, temp, (float)atof(param3), 0, 0, 0, 0};
            	clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            	return code;
			}
	    }
	    if (strcmp(command, "SET") == 0)
	    {
		if (strcmp(param2, "J") == 0)
		{
            instruction_code code = {SET_J, temp, (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "R") == 0)
		{
            instruction_code code = {SET_R, temp, (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "C") == 0)
		{
            instruction_code code = {SET_C, temp, (float)atof(param3), 
            (float)atof(param4), (float)atof(param5), (float)atof(param6), 
            (float)atof(param7)};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "HERE") == 0)
		{
		    if (strcmp(param3, "J") == 0)
		    {
                instruction_code code = {SET_HERE_J, temp, 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		    if (strcmp(param3, "R") == 0)
		    {
                instruction_code code = {SET_HERE_R, temp, 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		    if (strcmp(param3, "C") == 0)
		    {
                instruction_code code = {SET_HERE_C, temp, 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		}
	    }
	    if (strcmp(command, "SHOWALL") == 0)
	    {
            instruction_code code = {SHOWALL, "", 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	    }
	    if (strcmp(command, "SHOWCURRENT") == 0)
	    {
            if (strcmp(param1, "") == 0){
                instruction_code code = {SHOWCURRENT, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
            }
            if (strcmp(param1, "J") == 0){
                instruction_code code = {SHOWCURRENT_J, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
            }
            if (strcmp(param1, "R") == 0){
                instruction_code code = {SHOWCURRENT_R, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
            }
            if (strcmp(param1, "C") == 0){
                instruction_code code = {SHOWCURRENT_C, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
            }
	    }
	    if (strcmp(command, "SHOW") == 0)
	    {
		if (strcmp(param2, "") == 0)
		{
            instruction_code code = {SHOW, temp, 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "J") == 0)
		{
            instruction_code code = {SHOW_J, temp, 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "R") == 0)
		{
            instruction_code code = {SHOW_R, temp, 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param2, "C") == 0)
		{
            instruction_code code = {SHOW_C, temp, 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
	    }
	    if (strcmp(command, "DELAY") == 0)
	    {
            instruction_code code = {SHOW_C, "", (float)atof(param1), 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	    }
	    if (strcmp(command, "PRECYSION") == 0)
	    {
		if (strcmp(param1, "ON") == 0)
		{
            instruction_code code = {PRECYSION_ON, "", (float)atof(param2), (float)atof(param3), 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
		if (strcmp(param1, "OFF") == 0)
		{
            instruction_code code = {PRECYSION_OFF, "", (float)atof(param2), (float)atof(param3), 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
	    }
	    if (strcmp(command, "CONFIG") == 0)
	    {
		if (strcmp(param1, "COM") == 0)
		{
		    if (strcmp(param2, "UI") == 0)
		    {
                instruction_code code = {CONFIG_COM_UI, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		    if (strcmp(param2, "SERIAL") == 0)
		    {
			    instruction_code code = {CONFIG_COM_SERIAL, "", 0, 0, 0, 0, 0};
			    clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		}
		if (strcmp(command, "OFFSET") == 0)
		{
		    if (strcmp(param1, "ONPOINT") == 0)
		    {
                instruction_code code = {OFFSET_ONPOINT, "", 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		    if (strcmp(param1, "INPOINT") == 0)
		    {
                instruction_code code = {OFFSET_INPOINT, param1, 0, 0, 0, 0, 0};
                clear_all(command, param1, param2, param3, param4, param5, param6, param7);
                return code;
		    }
		}
		if (strcmp(command, "RESETPOINTS") == 0)
		{
            instruction_code code = {RESETPOINTS, "", 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
		}
	    }

	    if (strcmp(command, "H1OPEN") == 0)
	    {
            instruction_code code = {H1OPEN, "", 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	    }
	    if (strcmp(command, "H1CLOSE") == 0)
	    {
            instruction_code code = {H1CLOSE, "", 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	    }
	    if (strcmp(command, "H1STOP") == 0)
	    {
            instruction_code code = {H1STOP, "", 0, 0, 0, 0, 0};
            clear_all(command, param1, param2, param3, param4, param5, param6, param7);
            return code;
	    }

		instruction_code code = {NOCOMMAND, "", 0, 0, 0, 0, 0};
		clear_all(command, param1, param2, param3, param4, param5, param6, param7);
    	return code;
    }
