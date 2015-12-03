classdef Rail < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        left
        right
        
        top_height = 7880;
        max_diff = 1880;
    end
    
    methods
        function obj = Rail(conn)
            obj.right = conn.mxmotor(3);
            obj.left = conn.mxmotor(4);
            
            max_torque = 500;
            right.MaxTorque = max_torque;
            left.MaxTorque = max_torque;
            right.TorqueLimit = max_torque;
            left.TorqueLimit = max_torque;

            right.MovingSpeed = 1023;
            left.MovingSpeed = 1023;
            
            right.ResolutionDivider = 4;
            left.ResolutionDivider = 4;
        end
        
        function setPoses(obj, pos)
            obj.right.GoalPosition = pos(1);
            obj.left.GoalPosition = - pos(2);
        end
        function pos = getPoses(obj)
            pos = [
                obj.right.PresentPosition;
                -obj.left.PresentPosition;
            ];
        end
        
        function obj = setOrigin(obj)
            obj.left.MultiTurnOffset = obj.left.MultiTurnOffset - obj.left.PresentPosition;
            obj.right.MultiTurnOffset = obj.right.MultiTurnOffset - obj.right.PresentPosition;
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
            obj.setPoses([obj.top_height; obj.top_height]);
        end
        function dumpSnow(obj)
            obj.setPoses([obj.top_height - obj.max_diff; obj.top_height]);
        end
        function dumpDebris(obj)
            obj.setPoses([obj.top_height; obj.top_height - obj.max_diff]);
        end
        function wiggle(obj, n)
           pos = obj.getPoses;
           size = 50;
           delay = 0.05;
           for i=1:n
               obj.setPoses(pos + size * [1;1]);
               pause(delay);
               obj.setPoses(pos + size * [1;-1]);
               pause(delay);
               obj.setPoses(pos + size * [-1;-1]);
               pause(delay);
               obj.setPoses(pos + size * [-1;1]);
               pause(delay);
           end
        end
    end
end

