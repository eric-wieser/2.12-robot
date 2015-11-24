classdef Arduino < handle
	properties
		conn
		incompleteLine
		unparsedPackets
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

			obj.incompleteLine = '';
			obj.unparsedPackets = {};
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
			% recieve a packet from the arduino. If there is no pending packet
			% then return [], which can be tested for with isempty

			available = obj.conn.BytesAvailable;
			if available > 0
				% read the buffered port contents, and concatenate it with whatever was left last time
				data = [obj.incompleteLine fread(obj.conn, available)'];
				
				% split into lines - Serial.println sends \r\n
				parts = strsplit(data, '\r\n');
				
				% the last line is incomplete
				obj.incompleteLine = parts{end};
				
				% if we completed any lines, add them to the end of our queue
				obj.unparsedPackets = [obj.unparsedPackets parts(1:end-1)];
			end

			% if the queue is empty, we have no packet to return
			if isempty(obj.unparsedPackets)
				packet = [];
				return
			end

			% otherwise, pop the queue
			serialData = obj.unparsedPackets{1};
			obj.unparsedPackets = obj.unparsedPackets(2:end);

			% and decode
			packet = obj.decode_packet(serialData);
		end

		function packet = decode_packet(~, serialData)
			contents = serialData(2:end);
			switch serialData(1)
				case 'P'
					% position update packet
					splitData = strsplit(contents, ',');
					if length(splitData) < 4
						MException('arduino:decode:truncated', 'Packet too short').throw;
					end

					packet = struct();
					packet.type = 'location';
					packet.x = str2double(splitData(1));
					packet.y = str2double(splitData(2));
					packet.phi = str2double(splitData(3));
					packet.status = str2double(splitData(4));
				case 'E'
					% error packet
					packet = struct();
					packet.type = 'error';
					packet.message = contents;
				case 'D'
					% debug packet
					packet = struct();
					packet.type = 'debug';
					packet.message = contents;
				otherwise
					% unprefixed packet - assume debug
					packet = struct();
					packet.type = 'debug';
					packet.message = serialData;
			end
		end

		function delete(obj)
			% close the serial connection

			try
				fclose(obj.conn);
			catch; end
			delete(obj.conn)
		end
	end
end

