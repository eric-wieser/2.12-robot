classdef Navigator < handle
	%NAVIGATOR Summary of this class goes here
	%   Detailed explanation goes here

	properties
		listenHandle
		arduino
		in_progress = false;
		command_pending = true;
		
		cmd_id = 1;
		last_seen_cmd_id = 0;
	end

	methods
		function obj = Navigator(arduino)
			obj.arduino = arduino;
			obj.listenHandle = addlistener(arduino,...
				'PacketsArrived', @obj.onArduinoPacket);
		end

		function onArduinoPacket(obj, ~, data)
			for packet = data.Packet
				packet = packet{1};
				if strcmp(packet.type, 'location')
					obj.in_progress = logical(packet.status);
					obj.last_seen_cmd_id = max(obj.last_seen_cmd_id, packet.cid);
				end
			end
		end

		function goTo(obj, x, y)
			obj.cmd_id = obj.cmd_id + 1;
			obj.arduino.send_packet(struct(...
				'type', 'dest',...
				'cid', obj.cmd_id,...
				'x', x,...
				'y', y ...
			));
			obj.awaitCompletion();
		end

		function turnTo(obj, phi)
			obj.cmd_id = obj.cmd_id + 1;
			obj.arduino.send_packet(struct(...
				'type', 'turn',...
				'cid', obj.cmd_id,...
				'phi', phi ...
			));
			obj.awaitCompletion();
		end
		
		function awaitCompletion(obj)
			% wait for the command to start
			while(obj.last_seen_cmd_id < obj.cmd_id)
				pause(0.05);
			end
			
			% now wait for it to end
			while(obj.last_seen_cmd_id == obj.cmd_id && obj.in_progress)
				pause(0.05);
			end
		end
		
		function overridePose(obj, x, y, phi)
			obj.arduino.send_packet(struct(...
				'type', 'gps',...
				'x', x, ...
				'y', y, ...
				'phi', phi, ...
				'timestamp', -1 ...
			));
		end
	end

end

