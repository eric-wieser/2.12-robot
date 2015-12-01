% 2.12 Dynamixel interface code
% Zack Bright - zbright@mit.edu
% Steven Keyes - srkeyes@mit.edu
% Oct. 2015
%
% This class represents a set of Dynamixel servos connected to the computer

classdef DynamixelMX < handle
    properties
        id
        conn
    end
    properties (Constant, Access=private)
        registers = DynamixelMX.makeRegisters();
    end
    
    properties (Dependent)
       GoalPosition
       PresentPosition
       MovingSpeed
    end

    methods (Static, Access=private)
        function registers = makeRegisters()
            % see http://support.robotis.com/en/product/dynamixel/mx_series/mx-106.htm
            registers = {
                % eeprom
                'ModelNumber',             {hex2dec('00'), 2, false};  % R   model number                             (default: 64 (0X40), 1 (0X01))
                'VersionofFirmware',       {hex2dec('02'), 1, false};  % R   Information on the version of firmware   (default: -)
                'ID',                      {hex2dec('03'), 1, true};   % RW  ID of Dynamixel                          (default: 1 (0X01))
                'BaudRate',                {hex2dec('04'), 1, true};   % RW  Baud Rate of Dynamixel                   (default: 34 (0X22))
                'ReturnDelayTime',         {hex2dec('05'), 1, true};   % RW  Return Delay Time                        (default: 250 (0XFA))
                'CWAngleLimit',            {hex2dec('06'), 2, true};   % RW  clockwise Angle Limit                    (default: 0 (0X0000))
                'CCWAngleLimit',           {hex2dec('08'), 2, true};   % RW  counterclockwise Angle Limit             (default: 4095 (0X0FFF))
                'DriveMode',               {hex2dec('0A'), 1, true};   % RW  Dual Mode Setting                        (default: 0(0X00))
                'HighestLimitTemperature', {hex2dec('0B'), 1, true};   % RW  Internal Limit Temperature               (default: 80 (0X50))
                'LowestLimitVoltage',      {hex2dec('0C'), 1, true};   % RW  Lowest Limit Voltage                     (default: 60 (0X3C))
                'HighestLimitVoltage',     {hex2dec('0D'), 1, true};   % RW  Highest Limit Voltage                    (default: 160 (0XA0))
                'MaxTorque',               {hex2dec('0E'), 2, true};   % RW  Max. Torque                              (default: 1023 (0X03FF))
                'StatusReturnLevel',       {hex2dec('10'), 1, true};   % RW  Status Return Level                      (default: 2 (0X02))
                'AlarmLED',                {hex2dec('11'), 1, true};   % RW  LED for Alarm                            (default: 36 (0X24))
                'AlarmShutdown',           {hex2dec('12'), 1, true};   % RW  Shutdown for Alarm                       (default: 36 (0X24))
                'MultiTurnOffset',         {hex2dec('14'), 2, true};   % RW  multi-turn offset                        (default: 0 (0X0000))
                'ResolutionDivider',       {hex2dec('16'), 1, true};   % RW  Resolution divider                       (default: 1 (0X01))

                % ram
                'TorqueEnable',            {hex2dec('18'), 1, true};   % RW  Torque On/Off                            (default: 0 (0X00))
                'LED',                     {hex2dec('19'), 1, true};   % RW  LED On/Off                               (default: 0 (0X00))
                'DGain',                   {hex2dec('1A'), 1, true};   % RW  Derivative Gain                          (default: 0 (0X00))
                'IGain',                   {hex2dec('1B'), 1, true};   % RW  Integral Gain                            (default: 0 (0X00))
                'PGain',                   {hex2dec('1C'), 1, true};   % RW  Proportional Gain                        (default: 32 (0X20))
                'GoalPosition',            {hex2dec('1E'), 2, true};   % RW  Goal Position                            (default: -)
                'MovingSpeed',             {hex2dec('20'), 2, true};   % RW  Moving Speed (Moving Velocity)           (default: -)
                'TorqueLimit',             {hex2dec('22'), 2, true};   % RW  Torque Limit (Goal Torque)               (default: *0x0E)
                'PresentPosition',         {hex2dec('24'), 2, false};  % R   Current Position (Present Velocity)      (default: -)
                'PresentSpeed',            {hex2dec('26'), 2, false};  % R   Current Speed                            (default: -)
                'PresentLoad',             {hex2dec('28'), 2, false};  % R   Current Load                             (default: -)
                'PresentVoltage',          {hex2dec('2A'), 1, false};  % R   Current Voltage                          (default: -)
                'PresentTemperature',      {hex2dec('2B'), 1, false};  % R   Current Temperature                      (default: -)
                'Registered',              {hex2dec('2C'), 1, false};  % R   Means if Instruction is registered       (default: 0 (0X00))
                'Moving',                  {hex2dec('2E'), 1, false};  % R   Means if there is any movement           (default: 0 (0X00))
                'Lock',                    {hex2dec('2F'), 1, true};   % RW  Locking EEPROM                           (default: 0 (0X00))
                'Punch',                   {hex2dec('30'), 2, true};   % RW  Punch                                    (default: 0 (0X0000))
                'Current',                 {hex2dec('44'), 2, true};   % RW  Consuming Current                        (default: 0 (0X0000))
                'TorqueControlModeEnable', {hex2dec('46'), 1, true};   % RW  Torque control mode on/off               (default: 0 (0X00))
                'GoalTorque',              {hex2dec('47'), 2, true};   % RW  goal torque value                        (default: 0 (0X0000))
                'GoalAcceleration',        {hex2dec('49'), 1, true};   % RW  Goal Acceleration                        (default: 0 (0X00))
            }';
            registers = containers.Map(registers(1,:), registers(2,:));
        end
    end
    methods
        function obj = DynamixelMX(conn, id)
            obj.id = id;
            obj.conn = conn;

            obj.ping();
        end

        function delete(obj)
            set(obj, 'TorqueEnable', 0);
        end

        % wrap the connection functions
        function ping(obj)
            obj.conn.ping(obj.id);
        end
        function write_word(obj, reg, val)
            obj.conn.write_word(obj.id, reg, val);
        end
        function res = read_word(obj, reg)
            res = obj.conn.read_word(obj.id, reg);
        end
        function write_byte(obj, reg, val)
            obj.conn.write_byte(obj.id, reg, val);
        end
        function res = read_byte(obj, reg)
            res = obj.conn.read_byte(obj.id, reg);
        end
        
        function val = get(obj, name)
            if isscalar(obj)
                reg = DynamixelMX.registers(name);
                [addr, regsize, w] = reg{:};
                if regsize == 1
                    val = obj.read_byte(addr);
                elseif regsize == 2
                    val = obj.read_word(addr);
                else
                    error('unknown size')
                end
            else
                val = zeros(size(obj));
                for i=1:numel(obj)
                    val(i) = get(obj(i), name);
                end
            end
        end
        
        function set(obj, name, val)
            if isscalar(obj)
                reg = DynamixelMX.registers(name);
                [addr, size, w] = reg{:};

                if ~w
                    error('Register is readonly');
                end
            
                if size == 1
                    obj.write_byte(addr, val);
                elseif size == 2
                    obj.write_word(addr, val);
                else
                    error('unknown size')
                end
            else
                for i=1:numel(obj)
                    set(obj(i), name, val)
                end
            end
        end


		function val = subsref(obj, s)
			curr = s(1);
			if curr.type == '.'
                try
                    val = get(obj, curr.subs);
                catch                
                    val = builtin('subsref', obj, s);
                end
                
				if length(s) > 1
					val = subsref(val, s(2:end));
				end
			else
                val = builtin('subsref', obj, s);
			end
		end

		function obj = subsasgn(obj, s, val)
			curr = s(1);
			if curr.type == '.'
				if length(s) > 1
					error('cannot deal with nesting');
				end

				set(obj, curr.subs, val);
			else
                builtin('subsasgn', obj, s, val);
			end
		end
    end
end