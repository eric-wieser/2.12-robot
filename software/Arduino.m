classdef Arduino < handle
	properties
		conn
	end
	methods
		function obj = Arduino(comPort)
			obj.conn = serial(comPort);
			obj.conn.BaudRate = 115200;
			fopen(obj.conn);
			display('Successfully connected to Arduino over Serial!');

			% Wait for the Arduino to reset
			pause(1);

			% Throw away the startup gibberish
			flushinput(obj.conn);
		end
		function send_packet(obj, packet)
			% send a packet to the arduino
			switch packet.type
				case 'dest'
					fwrite(obj.conn, num2str(packet.x));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.y));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.phi));
					fwrite(obj.conn, '\n');
				otherwise
					MException('arduino:encode:unkn', 'Unknown packet type').throw;
			end
		end

		function packet = recv_packet(obj)
			% this is a temporary hack copied from lab3
			if (obj.conn.BytesAvailable < 12)
				packet = [];
				return
			end

			% Get a line from the buffer
			serialData = fscanf(obj.conn, '%s');

			if serialData(1) == 'P'
				% position update packet
				serialData = serialData(2:end);
				splitData = strsplit(serialData, ',');
				if length(splitData) < 4
					MException('arduino:decode:truncated', 'Packet too short').throw;
				end

				packet = struct();
				packet.type = 'location';
				packet.x = str2double(splitData(1));
				packet.y = str2double(splitData(2));
				packet.phi = (180/pi)*str2double(splitData(3));
				packet.status = str2double(splitData(4));
				return
			else
				% unrecognized packet - assume debug
				packet = struct();
				packet.type = 'debug';
				packet.message = serialData;
				return;
			end

		end

		function delete(obj)
			try
				fclose(obj.conn);
			catch; end
			delete(obj.conn)
		end
	end
end

