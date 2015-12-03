classdef (ConstructOnLoad) ArduinoPacketEventData < event.EventData
	properties
		Packet
	end
	methods
		function data = ArduinoPacketEventData(packet)
			data.Packet = packet;
		end
	end
end

