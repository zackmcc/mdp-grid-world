//
//  MDP.cpp
//  AI_GridWorld
//
//  Created by cpsfaculty on 11/13/17.
//  Copyright (c) 2017 ___Ju Shen___. All rights reserved.
//

#include <stdio.h>
#include <algorithm> //included for the max_element function
//#include <iostream> //Added to use cout to assist in debugging
#include "MDP.h"


/*************************************************************** Below are the two functions you need to implement ****************************************************/


/*Compute a Q-value for a given state and its action
  Input: state variable s; action (go to East/North/West/South), the corresponding interger value for each action is defined in the "MDP.h" Macros definition on the top
  Output: you need to update the corresponding q_values of the input state variable s
 */
void MDP::computeQValue(State &s, const int action)
{
	//These are use to store the value used in the Q value calculation
	float east_value = 0.0;
	float west_value = 0.0;
	float north_value = 0.0;
	float south_value = 0.0;

	// This is used to stor the value of the state before the Q value calculation so it can be used for the convergence calculation.
	float old_value = 0.0;

	// Check if s' falls outside of grid
	if (s.location.x + 1 > 4 || (s.location.x + 1 == 1 && s.location.y == 1))
		east_value = s.state_value;
	else
		east_value = states[s.location.y][s.location.x + 1].state_value;

	
	if (s.location.x - 1 < 0 || (s.location.x - 1 == 1 && s.location.y == 1))
		west_value = s.state_value;
	else
		west_value = states[s.location.y][s.location.x - 1].state_value;
		

	if (s.location.y - 1 < 0 || (s.location.x == 1 && s.location.y - 1 == 1))
		north_value = s.state_value;
	else
		north_value = states[s.location.y - 1][s.location.x].state_value;
		

	if (s.location.y + 1 < 3 || (s.location.x == 1 && s.location.y + 1 == 1))
		south_value = s.state_value;
	else
		south_value = states[s.location.y + 1][s.location.x].state_value;
		
	//Calculate Q value based on action
	switch (action)
	{
	case ACTION_EAST:
		old_value = s.q_values[ACTION_EAST]; //store previous value for convergence calculation
		// Calculate Q value (the equation is typecasted to float because it defaults to a double, and 
		// since the precision is only to 4-5 digits in the output it can be truncated to get rid of
		// the possible loss of data 'double' to 'float' conversion warning.
		s.q_values[ACTION_EAST] = (float)((TRANSITION_SUCCEED * (ACTION_REWARD + (GAMMA * east_value))) 
								+ ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * north_value))) 
								+ ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * south_value))));
		// If the differnce in this convergence value is greater than the current convergence value
		// update the current convergence value
		if (std::abs(old_value - s.q_values[ACTION_EAST]) > cur_convergence)
			cur_convergence = std::abs(old_value - s.q_values[ACTION_EAST]);
		break;
	case ACTION_NORTH:
		old_value = s.q_values[ACTION_NORTH];

		s.q_values[ACTION_NORTH] = (float)((TRANSITION_SUCCEED * (ACTION_REWARD + (GAMMA * north_value))) 
								 + ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * east_value))) 
								 + ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * west_value))));

		if (std::abs(old_value - s.q_values[ACTION_NORTH]) > cur_convergence)
			cur_convergence = std::abs(old_value - s.q_values[ACTION_NORTH]);

		break;
	case ACTION_SOUTH:
		old_value = s.q_values[ACTION_SOUTH];

		s.q_values[ACTION_SOUTH] = (float)((TRANSITION_SUCCEED * (ACTION_REWARD + (GAMMA * south_value)))
								 + ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * east_value))) 
								 + ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * west_value))));

		if (std::abs(old_value - s.q_values[ACTION_SOUTH]) > cur_convergence)
			cur_convergence = std::abs(old_value - s.q_values[ACTION_SOUTH]);

		break;
	case ACTION_WEST:
		old_value = s.q_values[ACTION_WEST];

		s.q_values[ACTION_WEST] = (float)((TRANSITION_SUCCEED * (ACTION_REWARD + (GAMMA * west_value)))
								+ ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * north_value))) 
								+ ((TRANSITION_FAIL/2.0) * (ACTION_REWARD + (GAMMA * south_value))));

		if (std::abs(old_value - s.q_values[ACTION_WEST]) > cur_convergence)
			cur_convergence = std::abs(old_value - s.q_values[ACTION_WEST]);

		break;
	default:
		break;
	}
}



/*There is no return value, actually you need to use the computing result to update the state values of all the states defined as data member "State states[3][4]". Of course, you don't need to update the wall state: states[1][1], the diamond state: states[0][3], and pitfall state: states[1][3] */
void MDP::valueIteration()
{
	int q_values_len = sizeof(states[0][0].q_values) / sizeof(states[0][0].q_values[ACTION_EAST]);
	cur_convergence = 0;
	//Iterate through rows
	for (int y = 0; y < 3; y++)
	{
		//iterate through columns
		for (int x = 0; x < 4; x++)
		{
			//check if current coordinates are wall state, diamond state, or pitfall state
			// if it is not one of those three states then calculate Q value for all four actions
			if (!((x == 1 && y == 1) || (y == 0 && x == 3) || (y == 1 && x == 3)))
			{
				for (int action = 0; action <= 3; action++)
				{
					computeQValue(states[y][x], action);
					
				}
				//After calculating all four Q values find the largest one and update that value to be the new state_value
				states[y][x].state_value = *std::max_element(states[y][x].q_values, states[y][x].q_values + q_values_len);
			}
		}

	}
	//printed for debugging
	//std::cout << "Current Convergence: " << cur_convergence << std::endl;
}


/**********************************************************************	End of Assignment *********************************************************************/


MDP::MDP()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure 
    
    
}


MDP::~MDP()
{
    
}

/*Reset the current computed state and Q values*/
void MDP::resetData()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
        }
    }
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    
    /*Set the current convergence to a big number initially*/
    cur_convergence = 100; //the reason this value is set to a big value is to ensure
    
    
}
