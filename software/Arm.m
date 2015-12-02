classdef Arm
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

		end

		function sweepRoof(obj)
			% TODO
		end
	end
end
