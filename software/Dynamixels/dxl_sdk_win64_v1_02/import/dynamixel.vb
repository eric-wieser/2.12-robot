Module dynamixel

    Public Const MAXNUM_TXPARAM = 150
    Public Const MAXNUM_RXPARAM = 60

    Public Const BROADCAST_ID = 254

    Public Const INST_PING = 1
    Public Const INST_READ = 2
    Public Const INST_WRITE = 3
    Public Const INST_REG_WRITE = 4
    Public Const INST_ACTION = 5
    Public Const INST_RESET = 6
    Public Const INST_SYNC_WRITE = 131

    Public Const ERRBIT_VOLTAGE = 1
    Public Const ERRBIT_ANGLE = 2
    Public Const ERRBIT_OVERHEAT = 4
    Public Const ERRBIT_RANGE = 8
    Public Const ERRBIT_CHECKSUM = 16
    Public Const ERRBIT_OVERLOAD = 32
    Public Const ERRBIT_INSTRUCTION = 64

    Public Const COMM_TXSUCCESS = 0
    Public Const COMM_RXSUCCESS = 1
    Public Const COMM_TXFAIL = 2
    Public Const COMM_RXFAIL = 3
    Public Const COMM_TXERROR = 4
    Public Const COMM_RXWAITING = 5
    Public Const COMM_RXTIMEOUT = 6
    Public Const COMM_RXCORRUPT = 7

    Public Declare Function dxl_initialize Lib "dynamixel.dll" Alias "dxl_initialize" (ByVal devIndex As Integer, ByVal baudnum As Integer) As Integer
    Public Declare Sub dxl_terminate Lib "dynamixel.dll" Alias "dxl_terminate" ()
    Public Declare Function dxl_get_result Lib "dynamixel.dll" Alias "dxl_get_result" () As Integer
    Public Declare Sub dxl_tx_packet Lib "dynamixel.dll" Alias "dxl_tx_packet" ()
    Public Declare Sub dxl_rx_packet Lib "dynamixel.dll" Alias "dxl_rx_packet" ()
    Public Declare Sub dxl_txrx_packet Lib "dynamixel.dll" Alias "dxl_txrx_packet" ()
    Public Declare Sub dxl_set_txpacket_id Lib "dynamixel.dll" Alias "dxl_set_txpacket_id" (ByVal id As Integer)
    Public Declare Sub dxl_set_txpacket_instruction Lib "dynamixel.dll" Alias "dxl_set_txpacket_instruction" (ByVal instruction As Integer)
    Public Declare Sub dxl_set_txpacket_parameter Lib "dynamixel.dll" Alias "dxl_set_txpacket_parameter" (ByVal index As Integer, ByVal value As Integer)
    Public Declare Sub dxl_set_txpacket_length Lib "dynamixel.dll" Alias "dxl_set_txpacket_length" (ByVal length As Integer)
    Public Declare Function dxl_get_rxpacket_error Lib "dynamixel.dll" Alias "dxl_get_rxpacket_error" (ByVal errbit As Integer) As Integer
    Public Declare Function dxl_get_rxpacket_length Lib "dynamixel.dll" Alias "dxl_get_rxpacket_length" () As Integer
    Public Declare Function dxl_get_rxpacket_parameter Lib "dynamixel.dll" Alias "dxl_get_rxpacket_parameter" (ByVal index As Integer) As Integer
    Public Declare Function dxl_makeword Lib "dynamixel.dll" Alias "dxl_makeword" (ByVal lowbyte As Integer, ByVal highbyte As Integer) As Integer
    Public Declare Function dxl_get_lowbyte Lib "dynamixel.dll" Alias "dxl_get_lowbyte" (ByVal word As Integer) As Integer
    Public Declare Function dxl_get_highbyte Lib "dynamixel.dll" Alias "dxl_get_highbyte" (ByVal word As Integer) As Integer
    Public Declare Sub dxl_ping Lib "dynamixel.dll" Alias "dxl_ping" (ByVal id As Integer)
    Public Declare Function dxl_read_byte Lib "dynamixel.dll" Alias "dxl_read_byte" (ByVal id As Integer, ByVal address As Integer) As Integer
    Public Declare Sub dxl_write_byte Lib "dynamixel.dll" Alias "dxl_write_byte" (ByVal id As Integer, ByVal address As Integer, ByVal value As Integer)
    Public Declare Function dxl_read_word Lib "dynamixel.dll" Alias "dxl_read_word" (ByVal id As Integer, ByVal address As Integer) As Integer
    Public Declare Sub dxl_write_word Lib "dynamixel.dll" Alias "dxl_write_word" (ByVal id As Integer, ByVal address As Integer, ByVal value As Integer)

End Module
