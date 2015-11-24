function Dynamixel_mecem 
%% Initializations
% initialize the object for interfacing with the dynamixels
d = Dynamixels(2);
port = 7;
kbps = 4;
d.Connect(port, kbps);

function doCleanup
  disp('harm avoided');
  d.SetGoalPos(1,d.GetCurrentPos(1));
  d.SetGoalPos(2,d.GetCurrentPos(2));
  d.Disconnect();
end

onCleanup(@doCleanup);

% set limits
d.SetTorque(1,111);
d.SetTorque(2,111);
% d.SetBounds(1,,);
% d.SetBounds(2,,);
d.SetSpeed(1,1300);
d.SetSpeed(2,1300);

% go to initial configuration
% init_pos1 = -500;
% init_pos2 = 4000;
% d.SetGoalPos(1,0);
% d.SetGoalPos(2,0);

%% Move dynamixel
pos = 3000; %pos positive to the up
d.SetGoalPos(1,pos);
d.GetCurrentPos(1)
d.SetGoalPos(2,pos);
d.GetCurrentPos(2)

%% Disconnect
d.Disconnect();
end
