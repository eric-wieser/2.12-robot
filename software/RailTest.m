function RailTest()
    d = DynamixelConn(7, 4);
    right = d.mxmotor(1);
    left  = d.mxmotor(2);

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


    % setPoses(0, 0);

    %% Debris in dump truck
    % setPoses(27500, 20500);

    %% Snow in dump truck
    % setPoses(20500, 27500);
end