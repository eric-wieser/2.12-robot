function Dynamixel_mecem 


%% Initializations
% initialize the object for interfacing with the dynamixels
d = Dynamixels(2);
port = 7;
kbps = 4;
d.Connect(port, kbps);

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
% pos = 9500;
% d.SetGoalPos(1,pos);
% d.SetGoalPos(2,pos);

% %% Go to initial configuration
d.SetGoalPos(1,0);
d.SetGoalPos(2,0);

% %% Debris in dump truck
% pos1 = 27500;
% pos2 = 20500; %pos positive to the up
% d.SetGoalPos(1,pos1);
% d.GetCurrentPos(1)
% d.SetGoalPos(2,pos2);
% d.GetCurrentPos(2)

%% Snow in dump truck
% pos2 = 27500;
% pos1 = 20500; %pos positive to the up
% d.SetGoalPos(1,pos1);
% % d.GetCurrentPos(1)
% d.SetGoalPos(2,pos2);
% % d.GetCurrentPos(2)

finishup = onCleanup(@() doCleanup(d));

pause(10);
%% Disconnect
% d.Disconnect();

end

function doCleanup(d)
  disp('harm avoided');
  d.freeze(1,d.GetCurrentPos(1));
  d.freeze(2,d.GetCurrentPos(2));
  d.Disconnect();
end


function test_erics()
    d = DynamixelConn(7, 4);
    right = d.mxmotor(1);
    left  = d.mxmotor(2);

    % todo - is this needed?
    function doCleanup(d)
      disp('harm avoided');
      delete(right);
      delete(left);
      delete(d);
    end
    finishup = onCleanup(@() doCleanup(d));

    right.MaxTorque = 90;
    left.MaxTorque = 90;

    right.MovingSpeed = 1023;
    left.MovingSpeed = 1023;

    function setPoses(r, l)
        init_pos1 = -15803;
        init_pos2 = 1102;

        right.GoalPosition = init_pos1 + r;
        left.GoalPosition = init_pos2 - l;
    end

    setPoses(0, 0);

    %% Debris in dump truck
    % setPoses(27500, 20500);

    %% Snow in dump truck
    % setPoses(20500, 27500);
end