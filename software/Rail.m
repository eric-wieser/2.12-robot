classdef Rail < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        left
        right
        
        top_height = 7880;
        max_diff = 2080;
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
        
        function setPoses(obj, pos, no_block)
            obj.right.GoalPosition = pos(1);
            obj.left.GoalPosition = - pos(2);
			
			if nargin == 3 && no_block
				return
			end
		
			tic
			while toc < 10
				left_done = abs(obj.left.GoalPosition - obj.left.PresentPosition) < 20;
				right_done = abs(obj.right.GoalPosition - obj.right.PresentPosition) < 20;
				if left_done && right_done
					return
				end
			end
        end
        function pos = getPoses(obj)
            pos = [
                obj.right.PresentPosition;
                -obj.left.PresentPosition;
            ];
        end
        
        function obj = setOrigin(obj)
			obj.left.MultiTurnOffset = 0;
			obj.right.MultiTurnOffset = 0;
            obj.left.MultiTurnOffset = -obj.left.PresentPosition;
            obj.right.MultiTurnOffset = -obj.right.PresentPosition;
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
			at = obj.getPoses();
            obj.setPoses(at / 2);
            obj.setPoses([0; 0]);
        end
        function raise(obj)
			target = [obj.top_height; obj.top_height];
            obj.setPoses(target / 2);
            obj.setPoses(target);
        end
        function dumpSnow(obj)
            obj.setPoses([obj.top_height - obj.max_diff; obj.top_height]);
        end
        function dumpDebris(obj)
            obj.setPoses([obj.top_height; obj.top_height - obj.max_diff]);
        end
        function wiggle(obj, n)
            if ~exist('n', 'var')
                n = 5;
            end
            pos = obj.getPoses;
            size = 50;
            delay = 0.05;
            for i=1:n
                obj.setPoses(pos + size * [1;1], true);
                pause(delay);
                obj.setPoses(pos + size * [1;-1], true);
                pause(delay);
                obj.setPoses(pos + size * [-1;-1], true);
                pause(delay);
                obj.setPoses(pos + size * [-1;1], true);
                pause(delay);
            end
        end
    end
end

