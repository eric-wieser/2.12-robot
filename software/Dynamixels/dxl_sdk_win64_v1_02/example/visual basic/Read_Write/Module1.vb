Module Module1

    'Control table address
    Public Const P_GOAL_POSITION_L = 30
    Public Const P_GOAL_POSITION_H = 31
    Public Const P_PRESENT_POSITION_L = 36
    Public Const P_PRESENT_POSITION_H = 37
    Public Const P_MOVING = 46

    'Defulat setting
    Public Const DEFAULT_PORTNUM = 3 'COM3
    Public Const DEFAULT_BAUDNUM = 1 '1Mbps
    Public Const DEFAULT_ID = 1

    Dim GoalPos(0 To 1) As Integer
    Dim index, Moving, PresentPos, CommStatus As Integer

    Sub Main()

        GoalPos(0) = 0
        GoalPos(1) = 1023
        'GoalPos(1) = 4095 'for EX serise
        index = 0

        'Open device
        If (dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM) = 0) Then
            Console.WriteLine("Failed to open USB2Dynamixel!")
            Console.WriteLine("Press any key to terminate...")
            Console.ReadKey(True)
            Exit Sub
        Else
            Console.WriteLine("Succeed to open USB2Dynamixel!")
        End If

        While (True)
            Console.WriteLine("Press any key to continue!(press ESC to quit)")
            If (Console.ReadKey(True).Key = ConsoleKey.Escape) Then
                Exit While
            End If

            'Write goal position
            dxl_write_word(DEFAULT_ID, P_GOAL_POSITION_L, GoalPos(index))
            Moving = 1
            While (Moving = 1)
                'Read present position
                PresentPos = dxl_read_word(DEFAULT_ID, P_PRESENT_POSITION_L)
                CommStatus = dxl_get_result()
                If (CommStatus = COMM_RXSUCCESS) Then
                    Console.WriteLine("" & GoalPos(index) & "   " & PresentPos & "")
                    PrintErrorCode()
                Else
                    PrintCommStatus(CommStatus)
                    Exit While
                End If

                'Check moving done
                Moving = dxl_read_byte(DEFAULT_ID, P_MOVING)
                CommStatus = dxl_get_result()
                If (CommStatus = COMM_RXSUCCESS) Then
                    If (Moving = 0) Then

                        'Change goal position
                        If (index = 0) Then
                            index = 1
                        Else
                            index = 0
                        End If

                        PrintErrorCode()
                    End If
                Else

                    PrintCommStatus(CommStatus)
                    Exit While
                End If

            End While
        End While

        'Close device
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
