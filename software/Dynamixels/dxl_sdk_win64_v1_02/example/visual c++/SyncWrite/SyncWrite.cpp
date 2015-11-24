// Windows version
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include "dynamixel.h"

#pragma comment(lib, "dynamixel.lib")

#define PI					3.141592f

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33

// Defulat setting
#define DEFAULT_PORTNUM		3 // COM3
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define NUM_ACTUATOR		3 // Number of actuator
#define STEP_THETA			(PI / 100.0f) // Large value is more fast
#define CONTROL_PERIOD		(10) // msec (Large value is more slow) 


void PrintCommStatus(int CommStatus);
void PrintErrorCode();


int main()
{
	int id[NUM_ACTUATOR];
	float phase[NUM_ACTUATOR];
	float theta = 0;
	int AmpPos = 512;
	//int AmpPos = 2048; // for EX serise
	int GoalPos;
	int i;
	int CommStatus;

	// Initialize id and phase
	for( i=0; i<NUM_ACTUATOR; i++ )
	{
		id[i] = i+1;
		phase[i] = 2*PI * (float)i / (float)NUM_ACTUATOR;
	}

	// Open device
	if( dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press any key to terminate...\n" );
		getch();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
	
	// Set goal speed	
	dxl_write_word( BROADCAST_ID, P_GOAL_SPEED_L, 0 );
	// Set goal position
	dxl_write_word( BROADCAST_ID, P_GOAL_POSITION_L, AmpPos );

	while(1)
	{
		printf( "Press any key to continue!(press ESC to quit)\n" );
		if(getch() == 0x1b)
			break;

		theta = 0;
		do
		{
			// Make syncwrite packet
		    dxl_set_txpacket_id(BROADCAST_ID);
			dxl_set_txpacket_instruction(INST_SYNC_WRITE);
			dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
			dxl_set_txpacket_parameter(1, 2);
			for( i=0; i<NUM_ACTUATOR; i++ )
			{
				dxl_set_txpacket_parameter(2+3*i, id[i]);
				GoalPos = (int)((sin(theta+phase[i]) + 1.0) * (double)AmpPos);
				printf( "%d  ", GoalPos );
				dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(GoalPos));
				dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(GoalPos));
			}
			dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
			

			printf( "\n" );
			
			dxl_txrx_packet();
			CommStatus = dxl_get_result();
			if( CommStatus == COMM_RXSUCCESS )
			{
				PrintErrorCode();
			}
			else
			{
				PrintCommStatus(CommStatus);
				break;
			}
			
			theta += STEP_THETA;
			Sleep(CONTROL_PERIOD);

		}while(theta < 2*PI);
	}

	dxl_terminate();
	printf( "Press any key to terminate...\n" );
	getch();

	return 0;
}


// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");	
}