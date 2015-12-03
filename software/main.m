% clean up leftover arduino sessions
clear arduino;
delete(instrfindall);

try
	arduino = Arduino('COM9');  % Use Arduino IDE to see which com port the Arduino UNO is using
catch e
	switch e.identifier
		case 'MATLAB:serial:fopen:opfailed'
			display('Serial connection could not be established. Make sure the Arduino is plugged in and on the right port (check in Arduino IDE).');
			return; %Quit the program
		otherwise
			rethrow(e)
	end
end

nav = Navigator(arduino);
nav.overridePoses(1.3, -1, pi/2)

plotter = ArenaPlotter(arduino);

dynamixel_conn = DynamixelConn(7, 4);
rail = Rail(dynamixel_conn);
arm = Arm(dynamixel_conn, arduino);

% TODO - all the things

% typing clear all should stop motors etc from failing