classdef Arm < handle
	properties
		arduino
		shoulder
		elbow
	end

	properties (Constant)
		% TODO
		l1 = 0;
		l2 = 0;
		% etc
	end

	methods
		function obj = Arm(arduino, dynamixel_conn)
			obj.arduino = arduino;
			obj.shoulder = dynamixel.mxmotor(3);
			obj.elbow = dynamixel.mxmotor(4);

			% TODO: configure torque / position limits on arm
		end

		function setHandTarget(obj, target)
			obj.arduino.send_packet(struct('type', 'servo', 'target', target))
		end

		function sweepSnow(obj)
			% TODO
			% obj.shoulder.GoalPosition = ...;
			% obj.setHandTarget(...); 
            f = 0.5*2*pi;
            a = 5;
            tic;
            while toc < 20
                x = 10+ a*cos(f*toc);
                y = 30 + a*sin(f*toc);               
                [theta1, theta2] = obj.findThetas(x,y);
                obj.shoulder.GoalPosition = obj.rad2value(theta1);
                obj.elbow.GoalPosition = obj.rad2value(theta2);
            end
        end

		function sweepRoof(obj)
			% TODO
        end
        
        function [theta1, theta2] = findThetas(obj, x, y)
            hyp = sqrt(x^2 + y^2);
            alpha = atan2(y,x);
            beta = acos((obj.l1^2 + hyp^2 - obj.l2^2)/(2 * hyp * obj.l1));
            gamma = acos((obj.l2^2 + obj.l1^2 - hyp^2)/(2 * obj.l1 * obj.l2));
            theta1 = pi/2 - alpha - beta;
            theta2 = pi - gamma;
        end
        
        function Pos = rad2value(positionInRadians)
            constrained = mod((positionInRadians + pi),(2*pi)) - pi;
            Pos = round((constrained + pi) * (4095) / (2*pi));
        end
	end
end
