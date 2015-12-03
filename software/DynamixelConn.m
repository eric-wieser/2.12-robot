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

		ERRBIT_VOLTAGE = 1;       % If the Error-impressed voltage is deviated from the set movement voltage range in the control table, it is set to 1.
		ERRBIT_ANGLE = 2;         % If the writing of Goal Position value is deviated from the range between CW Angle Limit ~ CCW Angle Limit, it is set to 1.
		ERRBIT_OVERHEAT = 4;      % If the internal temperature of Dynamixel is deviated from the set movement temperature range in the control table, it is set to 1.
		ERRBIT_RANGE = 8;         % When the command is deviated from the using range, it is set to 1.
		ERRBIT_CHECKSUM = 16;     % When Checksum of the transmitted Instruction Packet is not matching, it is set to 1.
		ERRBIT_OVERLOAD = 32;     % When the set torque cannot control the current load, it is set to 1.
		ERRBIT_INSTRUCTION = 64;  % If an undefined instruction is transmitted, or action command is transmitted without reg_write command, it is set to 1.
	end
	methods
		function obj = DynamixelConn(PORTNUM, BAUDNUM)
			loadlibrary('dynamixel','dynamixel.h')
			res = calllib('dynamixel','dxl_initialize',PORTNUM,BAUDNUM);
			if res ~= 1
				error('dynamixel:nolibrary', 'Failed to open USB2Dynamixel! Is the dongle connected, is the port correct, and is the dynamixel library installed?');
			end
		end

		function ping(obj, id)
			calllib('dynamixel','dxl_ping', id);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= obj.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during ping: ' DynamixelConn.comm_names{stat+1}]);
			end
			obj.error_check();
		end

		function res = read_word(obj, id, reg)
			res = calllib('dynamixel','dxl_read_word', id, reg);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during read_word: ' DynamixelConn.comm_names{stat+1}]);
			end
			obj.error_check();
		end

		function write_word(obj, id, reg, value)
			calllib('dynamixel','dxl_write_word', id, reg, value);
			stat = calllib('dynamixel','dxl_get_result');
			if stat ~= DynamixelConn.COMM_RXSUCCESS
				error('dynamixel:io', ['Error during write_word: ' DynamixelConn.comm_names{stat+1}]);
			end
			obj.error_check();
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
			obj.error_check();
		end

		function delete(obj)
			calllib('dynamixel','dxl_terminate');
			unloadlibrary('dynamixel');
		end

		function m = mxmotor(obj, id)
			m = DynamixelMX(obj, id);
		end
	end

	methods(Access=private)
		function error_check(obj)
			error_bit = @(bit) calllib('dynamixel', 'dxl_get_rxpacket_error', bit) == 1;

			errors = {};
			if error_bit(DynamixelConn.ERRBIT_VOLTAGE)
				errors = [errors {'Input voltage'}];
			end
			if error_bit(DynamixelConn.ERRBIT_ANGLE)
				errors = [errors {'Angle limit'}];
			end
			if error_bit(DynamixelConn.ERRBIT_OVERHEAT)
				errors = [errors {'Overheat'}];
			end
			if error_bit(DynamixelConn.ERRBIT_RANGE)
				errors = [errors {'Out of range'}];
			end
			if error_bit(DynamixelConn.ERRBIT_CHECKSUM)
				errors = [errors {'Checksum'}];
			end
			if error_bit(DynamixelConn.ERRBIT_OVERLOAD)
				errors = [errors {'Overload'}];
			end
			if error_bit(DynamixelConn.ERRBIT_INSTRUCTION)
				errors = [errors {'Instruction code'}];
			end

			if ~empty(errors)
				error(['dynamixel:fault', 'Dynamixel reported one or more errors: ' strjoin(errors, ', ')])
			end
		end
	end
end
