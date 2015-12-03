classdef Navigator < handle
	%NAVIGATOR Summary of this class goes here
	%   Detailed explanation goes here
	
	properties
		listenHandle
		arduino
		in_progress = false;
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
					if packet.status == 1
						obj.in_progress = false;
						return
					end
				end
			end
		end
		
		function goTo(obj, x, y)
			% TODO - race condition?
			obj.arduino.send_packet(struct(...
				'type', 'dest',...
				'x', x,...
				'y', y,...
				'phi', 0 ...
			));
			obj.in_progress = true;
			
			while(obj.in_progress)
				pause(0.05);
			end
		end
	end 
	
end

