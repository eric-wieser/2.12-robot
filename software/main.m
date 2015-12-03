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
rail = Rail(d);

nav = Navigator(arduino);
plotter = ArenaPlotter(arduino);

arm = Arm(dynamixel_conn, arduino);

gps = GPS('192.168.1.121');

% TODO - all the things

%% Task One -- Mount Simmons
mt_simmons_x = 0;
mt_simmons_y = 0;
pos = gps.read();
ERROR = .1;

nav.gpsFeedbackNav(gps, mt_simmons_x, mt_simmons_y);

arm.sweepSnow();
arm.outoftheway();
rail.dumpSnow();
rail.wiggle();
rail.dumpDebris();
rail.wiggle();
%% Task 2 -- Pick Up Tree
tree_x = 0;
tree_y = 0;

nav.gpsFeedbackNav(gps, tree_x, tree_y);

arm.outoftheway();
rail.raise();
rail.dumpSnow(); 
rail.wiggle();
% typing clear all should stop motors etc from failing

%%Task 3 -- Rescue house
house_x = 0;
house_y = 0;

nav.gpsFeedbackNav(gps, house_x, house_y);
rail.lower();
arm.sweepRoof();
