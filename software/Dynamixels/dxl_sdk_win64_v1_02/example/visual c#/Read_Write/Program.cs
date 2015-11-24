using System;
using System.Collections.Generic;
using System.Text;
using ROBOTIS; // for dynamixel sdk

namespace Read_Write
{
    class Program
    {
        // Control table address
        public const int P_GOAL_POSITION_L      = 30;
        public const int P_GOAL_POSITION_H      = 31;
        public const int P_PRESENT_POSITION_L   = 36;
        public const int P_PRESENT_POSITION_H   = 37;
        public const int P_MOVING				= 46;

        // Defulat setting
        public const int DEFAULT_PORTNUM    = 3; // COM3
        public const int DEFAULT_BAUDNUM    = 1; // 1Mbps
        public const int DEFAULT_ID         = 1;

        static void Main(string[] args)
        {
            int[] GoalPos = new int[] {0, 1023};
	        //int[] GoalPos = new int[] {0, 4095}; // for EX serise
	        int index = 0;
	        int Moving, PresentPos;
	        int CommStatus;

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
        	
	        while(true)
	        {
                Console.WriteLine("Press any key to continue!(press ESC to quit)");
		        if(Console.ReadKey(true).Key == ConsoleKey.Escape)
			        break;

		        // Write goal position
                dynamixel.dxl_write_word(DEFAULT_ID, P_GOAL_POSITION_L, GoalPos[index]);
		        do
		        {
			        // Read present position
                    PresentPos = dynamixel.dxl_read_word(DEFAULT_ID, P_PRESENT_POSITION_L);
                    CommStatus = dynamixel.dxl_get_result();
                    if (CommStatus == dynamixel.COMM_RXSUCCESS)
			        {
                        Console.WriteLine("{0:d}   {1:d}", GoalPos[index], PresentPos);
				        PrintErrorCode();
			        }
			        else
			        {
				        PrintCommStatus(CommStatus);
				        break;
			        }

			        // Check moving done
                    Moving = dynamixel.dxl_read_byte(DEFAULT_ID, P_MOVING);
                    CommStatus = dynamixel.dxl_get_result();
                    if (CommStatus == dynamixel.COMM_RXSUCCESS)
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
