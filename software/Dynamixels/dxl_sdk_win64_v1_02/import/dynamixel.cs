using System.Runtime.InteropServices;

namespace ROBOTIS
{
    unsafe class dynamixel
    {
        public const int MAXNUM_TXPARAM     = 150;
        public const int MAXNUM_RXPARAM     = 60;

        public const int BROADCAST_ID       = 254;

        public const int INST_PING          = 1;
        public const int INST_READ          = 2;
        public const int INST_WRITE         = 3;
        public const int INST_REG_WRITE     = 4;
        public const int INST_ACTION        = 5;
        public const int INST_RESET         = 6;
        public const int INST_SYNC_WRITE    = 131;

        public const int ERRBIT_VOLTAGE     = 1;
        public const int ERRBIT_ANGLE       = 2;
        public const int ERRBIT_OVERHEAT    = 4;
        public const int ERRBIT_RANGE       = 8;
        public const int ERRBIT_CHECKSUM    = 16;
        public const int ERRBIT_OVERLOAD    = 32;
        public const int ERRBIT_INSTRUCTION = 64;

        public const int COMM_TXSUCCESS     = 0;
        public const int COMM_RXSUCCESS     = 1;
        public const int COMM_TXFAIL        = 2;
        public const int COMM_RXFAIL        = 3;
        public const int COMM_TXERROR       = 4;
        public const int COMM_RXWAITING     = 5;
        public const int COMM_RXTIMEOUT     = 6;
        public const int COMM_RXCORRUPT     = 7;


        [DllImport("dynamixel.dll")]
        public static extern int dxl_initialize(int devIndex, int baudnum);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_terminate();

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_result();

        [DllImport("dynamixel.dll")]
        public static extern void dxl_tx_packet();

        [DllImport("dynamixel.dll")]
        public static extern void dxl_rx_packet();

        [DllImport("dynamixel.dll")]
        public static extern void dxl_txrx_packet();

        [DllImport("dynamixel.dll")]
        public static extern void dxl_set_txpacket_id(int id);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_set_txpacket_instruction(int instruction);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_set_txpacket_parameter(int index, int value);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_set_txpacket_length(int length);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_rxpacket_error(int errbit);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_rxpacket_length();

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_rxpacket_parameter(int index);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_makeword(int lowbyte, int highbyte);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_lowbyte(int word);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_get_highbyte(int word);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_ping(int id);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_read_byte(int id, int address);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_write_byte(int id, int address, int value);

        [DllImport("dynamixel.dll")]
        public static extern int dxl_read_word(int id, int address);

        [DllImport("dynamixel.dll")]
        public static extern void dxl_write_word(int id, int address, int value);
    }    
}