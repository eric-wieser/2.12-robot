classdef Rail < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        left
        right
        
        init_pos_right = 46117;
        init_pos_left = 3995;
    end
    
    methods
        function obj = Rail(conn)
            obj.right = conn.mxmotor(1);
            obj.left = conn.mxmotor(2);
            
            max_torque = 500;
            right.MaxTorque = max_torque;
            left.MaxTorque = max_torque;
            right.TorqueLimit = max_torque;
            left.TorqueLimit = max_torque;

            right.MovingSpeed = 1023;
            left.MovingSpeed = 1023;
        end
        
        function setPoses(obj, pos)
            obj.right.GoalPosition = obj.init_pos_right + pos(1);
            obj.left.GoalPosition = obj.init_pos_left - pos(2);
        end
        function pos = getPoses(obj)
            pos = [
                obj.right.PresentPosition - obj.init_pos_right;
                obj.init_pos_left - obj.left.PresentPosition;
            ];
        end
        
        function obj = setOrigin(obj)
            obj.init_pos_right = obj.right.PresentPosition;
            obj.init_pos_left = obj.left.PresentPosition;
        end
        
        function raiseRel(obj, amt)
            p = obj.getPoses();
            obj.setPoses(p+amt);
        end
        
        function tiltRel(obj, amt)
            p = obj.getPoses();
            obj.setPoses(p+[1;-1]*amt);
        end
        
        % high level commands
        function lower(obj)
            obj.setPoses([0; 0]);
        end
        function raise(obj)
            obj.setPoses([20500; 20500]);
        end
        function dumpSnow(obj)
            obj.setPoses([20500; 27900]);
        end
        function dumpDebris(obj)
            obj.setPoses([27900; 20500]);
        end
    end
end

