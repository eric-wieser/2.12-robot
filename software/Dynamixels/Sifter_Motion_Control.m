% Initialize the object for interfacing with the Dunamixels
dynamixel_obj = Dynamixels(2);
port = ;
kbps = 4;
dynamixel_obj.Connect(port, kbps)

% initialize object for deriving servo kinematics