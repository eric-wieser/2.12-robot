% clean up leftover arduino sessions
clear arduino;
delete(instrfindall);

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
rail = Rail(dynamixel_conn);

nav = Navigator(arduino);
plotter = ArenaPlotter(arduino);

disp('connected to nav');
%arm = Arm(dynamixel_conn, arduino);

gpsON= false;
if (gpsON)
    gps= GPS('192.168.1.121');
    tasks = Tasks(nav, gps, rail);
else
    gps = [];
    tasks = Tasks(nav, gps, rail);
end

%%
tasks.mt_simmons();
tasks.tree();
tasks.roof();