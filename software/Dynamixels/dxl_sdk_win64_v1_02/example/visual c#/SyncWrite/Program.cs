using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using ROBOTIS; // for dynamixel sdk

namespace SyncWrite
{
    class Program
    {
        // Control table address
        public const int P_GOAL_POSITION_L	= 30;
        public const int P_GOAL_POSITION_H	= 31;
        public const int P_GOAL_SPEED_L		= 32;
        public const int P_GOAL_SPEED_H		= 33;

        // Defulat setting
        public const int DEFAULT_PORTNUM    = 3; // COM3
        public const int DEFAULT_BAUDNUM    = 1; // 1Mbps
        public const int NUM_ACTUATOR       = 3; // Number of actuator
        public const double STEP_THETA      = Math.PI / 100; // Large value is more fast
        public const int CONTROL_PERIOD     = 10; // msec (Large value is more slow) 

        static void Main(string[] args)
        {
            int[] id = new int[NUM_ACTUATOR];
	        double[] phase = new double[NUM_ACTUATOR];
            double theta = 0;
	        int AmpPos = 512;
	        //int AmpPos = 2048; // for EX serise
	        int GoalPos;
	        int CommStatus;

	        // Initialize id and phase
	        for( int i=0; i<NUM_ACTUATOR; i++ )
	        {
		        id[i] = i+1;
                phase[i] = 2 * Math.PI * (double)i / (double)NUM_ACTUATOR;
	        }

	        // Open device
	        if( dynamixel.dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM) == 0 )
	        {
                Console.WriteLine("Failed to open USB2Dynamixel!");
                Console.WriteLine("Press any key to terminate...");
                Console.ReadKey(true);
		        return;
	        }
	        else
                Console.WriteLine("Succeed to open USB2Dynamixel!");
        	
	        // Set goal speed	
            dynamixel.dxl_write_word(dynamixel.BROADCAST_ID, P_GOAL_SPEED_L, 0);
	        // Set goal position
            dynamixel.dxl_write_word(dynamixel.BROADCAST_ID, P_GOAL_POSITION_L, AmpPos);

	        while(true)
	        {
                Console.WriteLine("Press any key to continue!(press ESC to quit)");
                if (Console.ReadKey(true).Key == ConsoleKey.Escape)
                    break;

		        theta = 0;
		        do
		        {
                    // Make syncwrite packet
                    dynamixel.dxl_set_txpacket_id(dynamixel.BROADCAST_ID);
                    dynamixel.dxl_set_txpacket_instruction(dynamixel.INST_SYNC_WRITE);
			        dynamixel.dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
			        dynamixel.dxl_set_txpacket_parameter(1, 2);
			        for( int i=0; i<NUM_ACTUATOR; i++ )
			        {
				        dynamixel.dxl_set_txpacket_parameter(2+3*i, id[i]);
				        GoalPos = (int)((Math.Sin(theta+phase[i]) + 1.0) * (double)AmpPos);
                        Console.Write("{0:d}  ", GoalPos);
                        dynamixel.dxl_set_txpacket_parameter(2 + 3 * i + 1, dynamixel.dxl_get_lowbyte(GoalPos));
                        dynamixel.dxl_set_txpacket_parameter(2 + 3 * i + 2, dynamixel.dxl_get_highbyte(GoalPos));
			        }
                    dynamixel.dxl_set_txpacket_length((2 + 1) * NUM_ACTUATOR + 4);

                    Console.WriteLine("");

                    dynamixel.dxl_txrx_packet();
                    CommStatus = dynamixel.dxl_get_result();
                    if (CommStatus == dynamixel.COMM_RXSUCCESS)
			        {
				        PrintErrorCode();
			        }
			        else
			        {
				        PrintCommStatus(CommStatus);
				        break;
			        }
        			
			        theta += STEP_THETA;
			        Thread.Sleep(CONTROL_PERIOD);

		        }while(theta < 2 * Math.PI);
	        }

            dynamixel.dxl_terminate();
            Console.WriteLine("Press any key to terminate...");
            Console.ReadKey(true);
        }

        // Print communication result
        static void PrintCommStatus(int CommStatus)
        {
            switch (CommStatus)
            {
                case dynamixel.COMM_TXFAIL:
                    Console.WriteLine("COMM_TXFAIL: Failed transmit instruction packet!");
                    break;

                case dynamixel.COMM_TXERROR:
                    Console.WriteLine("COMM_TXERROR: Incorrect instruction packet!");
                    break;

                case dynamixel.COMM_RXFAIL:
                    Console.WriteLine("COMM_RXFAIL: Failed get status packet from device!");
                    break;

                case dynamixel.COMM_RXWAITING:
                    Console.WriteLine("COMM_RXWAITING: Now recieving status packet!");
                    break;

                case dynamixel.COMM_RXTIMEOUT:
                    Console.WriteLine("COMM_RXTIMEOUT: There is no status packet!");
                    break;

                case dynamixel.COMM_RXCORRUPT:
                    Console.WriteLine("COMM_RXCORRUPT: Incorrect status packet!");
                    break;

                default:
                    Console.WriteLine("This is unknown error code!");
                    break;
            }
        }

        // Print error bit of status packet
        static void PrintErrorCode()
        {
            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_VOLTAGE) == 1)
                Console.WriteLine("Input voltage error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_ANGLE) == 1)
                Console.WriteLine("Angle limit error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_OVERHEAT) == 1)
                Console.WriteLine("Overheat error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_RANGE) == 1)
                Console.WriteLine("Out of range error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_CHECKSUM) == 1)
                Console.WriteLine("Checksum error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_OVERLOAD) == 1)
                Console.WriteLine("Overload error!");

            if (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_INSTRUCTION) == 1)
                Console.WriteLine("Instruction code error!");
        }
    }
}
