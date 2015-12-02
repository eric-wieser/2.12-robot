try
	arduino = Arduino('COM8');  % Use Arduino IDE to see which com port the Arduino UNO is using
catch e
	switch e.identifier
		case 'MATLAB:serial:fopen:opfailed'
			display('Serial connection could not be established. Make sure the Arduino is plugged in and on the right port (check in Arduino IDE).');
			return; %Quit the program
		otherwise
			rethrow(e)
	end
end

dynamixel_conn = DynamixelConn(7, 4);
rail = Rail(d);

arm = Arm(dynamixel_conn, arduino);

%TODO - all the things