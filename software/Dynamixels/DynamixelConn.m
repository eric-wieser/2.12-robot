% Represents a connection to a usb2dynamixel port
% makes sure that errors are detected
classdef DynamixelConn < handle
	properties(Constant)
		COMM_TXSUCCESS = 0;
		COMM_RXSUCCESS = 1;
		COMM_TXFAIL    = 2;
		COMM_RXFAIL    = 3;
		COMM_TXERROR   = 4;
		COMM_RXWAITING = 5;
		COMM_RXTIMEOUT = 6;
		COMM_RXCORRUPT = 7;

		comm_names = {
			'COMM_TXSUCCESS',
			'COMM_RXSUCCESS',
			'COMM_TXFAIL',
			'COMM_RXFAIL',
			'COMM_TXERROR',
			'COMM_RXWAITING',
			'COMM_RXTIMEOUT',
			'COMM_RXCORRUPT',
		}
	end
	methods
		function obj = DynamixelConn(PORTNUM, BAUDNUM)
			loadlibrary('dynamixel','dynamixel.h')
			res = calllib('dynamixel','dxl_initialize',PORTNUM,BAUDNUM);
			if res ~= 1
				error('Failed to open USB2Dynamixel! Is the dongle connected, is the port correct, and is the dynamixel library installed?');
			end
		end

		function ping(obj, id)
			calllib('dynamixel','dxl_ping', id);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= obj.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during ping: ' DynamixelConn.comm_names{stat+1}]);
			end
		end

		function res = read_word(obj, id, reg)
			res = calllib('dynamixel','dxl_read_word', id, reg);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during read_word: ' DynamixelConn.comm_names{stat+1}]);
			end
		end

		function write_word(obj, id, reg, value)
			calllib('dynamixel','dxl_write_word', id, reg, value);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during write_word: ' DynamixelConn.comm_names{stat+1}]);
			end
		end

		function res = read_byte(obj, id, reg)
			res = calllib('dynamixel','dxl_read_byte', id, reg);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during read_byte: ' DynamixelConn.comm_names{stat+1}]);
			end
		end

		function write_byte(obj, id, reg, value)
			calllib('dynamixel','dxl_write_byte', id, reg, value);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during write_byte: ' DynamixelConn.comm_names{stat+1}]);
			end
		end

		function delete(obj)
			calllib('dynamixel','dxl_terminate');
			unloadlibrary('dynamixel');
		end

		function m = mxmotor(obj, id)
			m = DynamixelMX(obj, id);
		end
	end
end
