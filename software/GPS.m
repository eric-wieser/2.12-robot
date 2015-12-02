classdef GPS
	properties (Access=private)
		conn
	end
	methods
		function obj = GPS(serverIP)
			if nargin == 0
				serverIP = '192.168.1.212';
			end
			obj.conn = tcpip(serverIP,2121,'NetworkRole','Client');
			fopen(obj.conn);
		end

		function delete(obj)
			fclose(obj.conn);
		end

		% returns values in origin-centered coordinates
		function res = read(obj)
			fwrite(obj.conn, 'r');
			data = [fread(obj.conn,3,'single')' fread(obj.conn,1,'single')];
			res.x = data(1);
			res.y = data(2);
			res.phi = data(3);
			res.timestamp = data(4);
		end
	end
end
