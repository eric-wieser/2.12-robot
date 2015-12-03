classdef Arduino < handle
	properties
		conn
		incompleteLine
		timerHandle
	end
	events
		PacketsArrived
    end
	methods
		function obj = Arduino(comPort)
			obj.conn = serial(comPort);
			obj.conn.BaudRate = 115200;

			% set up events


			fopen(obj.conn);
			display('Successfully connected to Arduino over Serial!');

			% Wait for the Arduino to reset
			pause(1);

			% Throw away the startup gibberish
			flushinput(obj.conn);

			obj.incompleteLine = '';

			obj.timerHandle = timer(...
				'StartDelay',0.05,...
				'Period', 0.05,...
				'ExecutionMode', 'fixedDelay',...
				'TimerFcn', @obj.recv_packets);
			start(obj.timerHandle);
		end

		function send_packet(obj, packet)
			% send a packet to the arduino

			switch packet.type
				case 'dest'
					fwrite(obj.conn, 'D');
					fwrite(obj.conn, num2str(packet.cid));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.x));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.y));
					fwrite(obj.conn, '\n');
				case 'turn'
					fwrite(obj.conn, 'T');
					fwrite(obj.conn, num2str(packet.cid));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.phi));
					fwrite(obj.conn, '\n');
				case 'gps'
					fwrite(obj.conn, 'G');
					fwrite(obj.conn, num2str(packet.x));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.y));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.phi));
					fwrite(obj.conn, ',');
					fwrite(obj.conn, num2str(packet.timestamp));
					fwrite(obj.conn, '\n');
				case 'servo'
					fwrite(obj.conn, 'S');
					fwrite(obj.conn, num2str(packet.target));
					fwrite(obj.conn, '\n');
				case 'kill'
					fwrite(obj.conn, 'K');
					fwrite(obj.conn, '\n');
				case 'ping'
					fwrite(obj.conn, '?');
					fwrite(obj.conn, '\n');
				otherwise
					MException('arduino:encode:unkn', 'Unknown packet type').throw;
			end
		end

		function recv_packets(obj, ~, ~)
			% recieve a packet from the arduino.

			available = obj.conn.BytesAvailable;
			if available > 0
				% read the buffered port contents, and concatenate it with whatever was left last time
				data = [obj.incompleteLine fread(obj.conn, available)'];

				% split into lines - Serial.println sends \r\n
				parts = strsplit(data, '\r\n');

				% the last line is incomplete
				obj.incompleteLine = parts{end};

				% process packets
				packets = {};
				for part = parts(1:end-1)
					part = part{1};
					packets = [packets {obj.decode_packet(part)}];
				end
				obj.onPacketsParsed(packets);
			end
		end

		function delete(obj)
			% close the serial connection
			disp('Stopping motors');
			obj.send_packet(struct('type', 'kill'));
			fclose(obj.conn);
			delete(obj.conn);
		end
	end

	methods (Access=private)
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
					packet.cid = str2double(splitData(5));
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
				case '!'
					% pong packet
					packet = struct();
					packet.type = 'pong';
				otherwise
					% unprefixed packet - assume debug
					packet = struct();
					packet.type = 'debug';
					packet.message = serialData;
			end
		end

		function onPacketsParsed(obj, packets)
			notify(obj, 'PacketsArrived', ArduinoPacketEventData(packets));
			for p = packets
				p = p{1};
				if strcmp(p.type, 'debug')
					disp(['Debug message:', p.message]);
				end
			end
		end
	end
end

