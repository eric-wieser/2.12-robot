// Windows version
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dynamixel.h"

#pragma comment(lib, "dynamixel.lib")


// Control table address
#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

// Defulat setting
#define DEFAULT_PORTNUM		3 // COM3
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID			1


void PrintCommStatus(int CommStatus);
void PrintErrorCode();

int main()
{
	int GoalPos[2] = {0, 1023};
	//int GoalPos[2] = {0, 4095}; // for EX serise
	int index = 0;
	int Moving, PresentPos;
	int CommStatus;

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
	
	while(1)
	{
		printf( "Press any key to continue!(press ESC to quit)\n" );
		if(getch() == 0x1b)
			break;

		// Write goal position
		dxl_write_word( DEFAULT_ID, P_GOAL_POSITION_L, GoalPos[index] );
		do
		{
			// Read present position
			PresentPos = dxl_read_word( DEFAULT_ID, P_PRESENT_POSITION_L );
			CommStatus = dxl_get_result();
			if( CommStatus == COMM_RXSUCCESS )
			{
				printf( "%d   %d\n",GoalPos[index], PresentPos );
				PrintErrorCode();
			}
			else
			{
				PrintCommStatus(CommStatus);
				break;
			}

			// Check moving done
			Moving = dxl_read_byte( DEFAULT_ID, P_MOVING );
			CommStatus = dxl_get_result();
			if( CommStatus == COMM_RXSUCCESS )
			{
				if( Moving == 0 )
				{
					// Change goal position
					if( index == 0 )
						index = 1;
					else
						index = 0;					
				}

				PrintErrorCode();
			}
			else
			{
				PrintCommStatus(CommStatus);
				break;
			}
			
		}while(Moving == 1);
	}

	// Close device
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