Module Module1

    'Sleep function
    Public Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)

    'Control table address
    Public Const P_GOAL_POSITION_L = 30
    Public Const P_GOAL_POSITION_H = 31
    Public Const P_GOAL_SPEED_L = 32
    Public Const P_GOAL_SPEED_H = 33

    'Defulat setting
    Public Const DEFAULT_PORTNUM = 3 'COM3
    Public Const DEFAULT_BAUDNUM = 1 '1Mbps
    Public Const NUM_ACTUATOR = 3 'Number of actuator
    Public Const STEP_THETA = Math.PI / 100 'Large value is more fast
    Public Const CONTROL_PERIOD = 10 'msec (Large value is more slow)

    Dim id(0 To (NUM_ACTUATOR - 1)) As Integer
    Dim phase(0 To (NUM_ACTUATOR - 1)) As Double
    Dim AmpPos, GoalPos, CommStatus As Integer
    Dim theta As Double


    Sub Main()

        AmpPos = 512
        'AmpPos = 2048 'for EX serise

        'Initialize id and phase
        For i As Integer = 0 To (NUM_ACTUATOR - 1)
            id(i) = i + 1
            phase(i) = 2 * Math.PI * i / NUM_ACTUATOR
        Next i

        'Open device
        If (dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM) = 0) Then
            Console.WriteLine("Failed to open USB2Dynamixel!")
            Console.WriteLine("Press any key to terminate...")
            Console.ReadKey(True)
            Exit Sub
        Else
            Console.WriteLine("Succeed to open USB2Dynamixel!")
        End If

        'Set goal speed	
        dxl_write_word(dynamixel.BROADCAST_ID, P_GOAL_SPEED_L, 0)
        'Set goal position
        dxl_write_word(dynamixel.BROADCAST_ID, P_GOAL_POSITION_L, AmpPos)

        While (True)
            Console.WriteLine("Press any key to continue!(press ESC to quit)")
            If (Console.ReadKey(True).Key = ConsoleKey.Escape) Then
                Exit While
            End If

            theta = 0
            While (theta < 2 * Math.PI)
                'Make syncwrite packet
                dxl_set_txpacket_id(dynamixel.BROADCAST_ID)
                dxl_set_txpacket_instruction(dynamixel.INST_SYNC_WRITE)
                dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L)
                dxl_set_txpacket_parameter(1, 2)

                For i As Integer = 0 To (NUM_ACTUATOR - 1)
                    dxl_set_txpacket_parameter(2 + 3 * i, id(i))
                    GoalPos = (Math.Sin(theta + phase(i)) + 1) * AmpPos
                    Console.Write("" & GoalPos & "  ")
                    dxl_set_txpacket_parameter(2 + 3 * i + 1, dynamixel.dxl_get_lowbyte(GoalPos))
                    dxl_set_txpacket_parameter(2 + 3 * i + 2, dynamixel.dxl_get_highbyte(GoalPos))
                Next i

                dxl_set_txpacket_length((2 + 1) * NUM_ACTUATOR + 4)

                Console.WriteLine("")

                dxl_txrx_packet()
                CommStatus = dxl_get_result()
                If (CommStatus = dynamixel.COMM_RXSUCCESS) Then
                    PrintErrorCode()
                Else
                    PrintCommStatus(CommStatus)
                    Exit While
                End If

                theta += STEP_THETA
                Sleep(CONTROL_PERIOD)
            End While
        End While

        dxl_terminate()
        Console.WriteLine("Press any key to terminate...")
        Console.ReadKey(True)
    End Sub

    'Print communication result
    Sub PrintCommStatus(ByVal CommStatus As Integer)

        Select Case (CommStatus)

            Case COMM_TXFAIL
                Console.WriteLine("COMM_TXFAIL: Failed transmit instruction packet!")

            Case COMM_TXERROR
                Console.WriteLine("COMM_TXERROR: Incorrect instruction packet!")

            Case COMM_RXFAIL
                Console.WriteLine("COMM_RXFAIL: Failed get status packet from device!")

            Case COMM_RXWAITING
                Console.WriteLine("COMM_RXWAITING: Now recieving status packet!")

            Case COMM_RXTIMEOUT
                Console.WriteLine("COMM_RXTIMEOUT: There is no status packet!")

            Case COMM_RXCORRUPT
                Console.WriteLine("COMM_RXCORRUPT: Incorrect status packet!")

            Case Else
                Console.WriteLine("This is unknown error code!")

        End Select

    End Sub

    'Print error bit of status packet
    Sub PrintErrorCode()

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_VOLTAGE) = 1) Then
            Console.WriteLine("Input voltage error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_ANGLE) = 1) Then
            Console.WriteLine("Angle limit error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_OVERHEAT) = 1) Then
            Console.WriteLine("Overheat error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_RANGE) = 1) Then
            Console.WriteLine("Out of range error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_CHECKSUM) = 1) Then
            Console.WriteLine("Checksum error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_OVERLOAD) = 1) Then
            Console.WriteLine("Overload error!")
        End If

        If (dynamixel.dxl_get_rxpacket_error(dynamixel.ERRBIT_INSTRUCTION) = 1) Then
            Console.WriteLine("Instruction code error!")
        End If

    End Sub

End Module
