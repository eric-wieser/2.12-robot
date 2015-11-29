function Dynamixel_mecem 

function doCleanup(d)
  disp('harm avoided');
  d.freeze(1,d.GetCurrentPos(1));
  d.freeze(2,d.GetCurrentPos(2));
  d.Disconnect();
end

%% Initializations
% initialize the object for interfacing with the dynamixels
d = Dynamixels(2);
port = 7;
kbps = 4;
d.Connect(port, kbps);

finishup = onCleanup(@() doCleanup(d));

% set limits
d.SetTorque(1,90);
d.SetTorque(2,90);
% d.SetTorque(1,111);
% d.SetTorque(2,111);
% d.SetBounds(1,,);
% d.SetBounds(2,,);
d.SetSpeed(1,1023);
d.SetSpeed(2,1023);

%% Go to given configuration
pos = 1000;
d.SetGoalPos(1,pos);
d.SetGoalPos(2,pos);

% %% Go to initial configuration
% d.SetGoalPos(1,0);
% d.SetGoalPos(2,0);
% finishup = onCleanup(@() doCleanup(d));

% %% Debris in dump truck
% pos1 = 27500;
% pos2 = 20500; %pos positive to the up
% d.SetGoalPos(1,pos1);
% d.GetCurrentPos(1)
% d.SetGoalPos(2,pos2);
% d.GetCurrentPos(2)

% %% Snow in dump truck
% pos2 = 27500;
% pos1 = 20500; %pos positive to the up
% d.SetGoalPos(1,pos1);
% finishup = onCleanup(@() doCleanup);
% % d.GetCurrentPos(1)
% d.SetGoalPos(2,pos2);
% % d.GetCurrentPos(2)
% finishup = onCleanup(@() doCleanup(d));

%% Disconnect
d.Disconnect();

end
