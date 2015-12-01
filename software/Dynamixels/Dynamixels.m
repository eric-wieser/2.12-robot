% 2.12 Dynamixel interface code
% Zack Bright - zbright@mit.edu
% Steven Keyes - srkeyes@mit.edu
% Oct. 2015
%
% This class represents a set of Dynamixel servos connected to the computer

classdef Dynamixels
    properties
        % the number of servos on the bus
        nServos
    end
    properties (Constant)
        % eeprom
        R_ModelNumber              = hex2dec('00');  % R   model number                             (default: 64 (0X40), 1 (0X01))
        R_VersionofFirmware        = hex2dec('02');  % R   Information on the version of firmware   (default: -)
        R_ID                       = hex2dec('03');  % RW  ID of Dynamixel                          (default: 1 (0X01))
        R_BaudRate                 = hex2dec('04');  % RW  Baud Rate of Dynamixel                   (default: 34 (0X22))
        R_ReturnDelayTime          = hex2dec('05');  % RW  Return Delay Time                        (default: 250 (0XFA))
        R_CWAngleLimit             = hex2dec('06');  % RW  clockwise Angle Limit                    (default: 0 (0X0000))
        R_CCWAngleLimit            = hex2dec('08');  % RW  counterclockwise Angle Limit             (default: 4095 (0X0FFF))
        R_DriveMode                = hex2dec('0A');  % RW  Dual Mode Setting                        (default: 0(0X00))
        R_HighestLimitTemperature  = hex2dec('0B');  % RW  Internal Limit Temperature               (default: 80 (0X50))
        R_LowestLimitVoltage       = hex2dec('0C');  % RW  Lowest Limit Voltage                     (default: 60 (0X3C))
        R_HighestLimitVoltage      = hex2dec('0D');  % RW  Highest Limit Voltage                    (default: 160 (0XA0))
        R_MaxTorque                = hex2dec('0E');  % RW  Max. Torque                              (default: 1023 (0X03FF))
        R_StatusReturnLevel        = hex2dec('10');  % RW  Status Return Level                      (default: 2 (0X02))
        R_AlarmLED                 = hex2dec('11');  % RW  LED for Alarm                            (default: 36 (0X24))
        R_AlarmShutdown            = hex2dec('12');  % RW  Shutdown for Alarm                       (default: 36 (0X24))
        R_MultiTurnOffset          = hex2dec('14');  % RW  multi-turn offset                        (default: 0 (0X0000))
        R_ResolutionDivider        = hex2dec('16');  % RW  Resolution divider                       (default: 1 (0X01))

        % ram
        R_TorqueEnable             = hex2dec('18');  % RW  Torque On/Off                            (default: 0 (0X00))
        R_LED                      = hex2dec('19');  % RW  LED On/Off                               (default: 0 (0X00))
        R_DGain                    = hex2dec('1A');  % RW  Derivative Gain                          (default: 0 (0X00))
        R_IGain                    = hex2dec('1B');  % RW  Integral Gain                            (default: 0 (0X00))
        R_PGain                    = hex2dec('1C');  % RW  Proportional Gain                        (default: 32 (0X20))
        R_GoalPosition             = hex2dec('1E');  % RW  Goal Position                            (default: -)
        R_MovingSpeed              = hex2dec('20');  % RW  Moving Speed (Moving Velocity)           (default: -)
        R_TorqueLimit              = hex2dec('22');  % RW  Torque Limit (Goal Torque)               (default: *0x0E)
        R_PresentPosition          = hex2dec('24');  % R   Current Position (Present Velocity)      (default: -)
        R_PresentSpeed             = hex2dec('26');  % R   Current Speed                            (default: -)
        R_PresentLoad              = hex2dec('28');  % R   Current Load                             (default: -)
        R_PresentVoltage           = hex2dec('2A');  % R   Current Voltage                          (default: -)
        R_PresentTemperature       = hex2dec('2B');  % R   Current Temperature                      (default: -)
        R_Registered               = hex2dec('2C');  % R   Means if Instruction is registered       (default: 0 (0X00))
        R_Moving                   = hex2dec('2E');  % R   Means if there is any movement           (default: 0 (0X00))
        R_Lock                     = hex2dec('2F');  % RW  Locking EEPROM                           (default: 0 (0X00))
        R_Punch                    = hex2dec('30');  % RW  Punch                                    (default: 0 (0X0000))
        R_Current                  = hex2dec('44');  % RW  Consuming Current                        (default: 0 (0X0000))
        R_TorqueControlModeEnable  = hex2dec('46');  % RW  Torque control mode on/off               (default: 0 (0X00))
        R_GoalTorque               = hex2dec('47');  % RW  goal torque value                        (default: 0 (0X0000))
        R_GoalAcceleration         = hex2dec('49');  % RW  Goal Acceleration                        (default: 0 (0X00))
    end
    methods
        function obj = Dynamixels(nServos)
            if nargin > 0
                if isnumeric(nServos)
                    obj.nServos = nServos;
                else
                    error('Value must be the number of servos')
                end
            else
                obj.nServos = 0;
            end
        end
        % method to connect to the dongle and identify its servos
        % returns a list of the IDs of all the servos
        function id = Connect(obj, PORTNUM,BAUDNUM)
            loadlibrary('dynamixel','dynamixel.h')
            id = zeros(1,obj.nServos);
            res = calllib('dynamixel','dxl_initialize',PORTNUM,BAUDNUM);
            if res == 1
                disp('Connected to USB2Dynamixel dongle');
                for i = 1:obj.nServos
                    id(1,i) = i;
                end
                disp(['Connected to ' num2str(length(id)) ' Dynamixel Motors']);
            else
                disp('Failed to open USB2Dynamixel! Is the dongle connected, is the port correct, and is the dynamixel library installed?');
            end
        end

        % set the position of a single servo -- range 0 to 4095, centered
        % at 2047
        function SetGoalPos(obj, id, Pos)
            % write it
            init_pos1 = -15803;%500;
            init_pos2 = 1102;%6500;%2500;
%             init_pos1 = -500;
%             init_pos2 = 4000;
            if (id == 1)
                calllib('dynamixel','dxl_write_word',id,obj.R_GoalPosition,Pos+init_pos1);
            else
                calllib('dynamixel','dxl_write_word',id,obj.R_GoalPosition,-Pos+init_pos2);  %attention servo do not turn in same direction
            end
        end

        function freeze(obj, id, Pos)
            if (id == 1)
                calllib('dynamixel','dxl_write_word',id,obj.R_GoalPosition,Pos);
            else
                calllib('dynamixel','dxl_write_word',id,obj.R_GoalPosition,Pos);  %attention servo do not turn in same direction
            end
        end

        function SetAngle(obj, id, addr, ang)
            calllib('dynamixel', 'dxl_write_word', id, addr, obj.rad2value(ang));
        end

        function SetBounds(obj, id, min, max)
            SetAngle(obj, id, obj.R_CWAngleLimit, min);
            SetAngle(obj, id, obj.R_CCWAngleLimit, max);
        end

        function SetTorque(obj, id, torque_limit)
            calllib('dynamixel', 'dxl_write_word', id, obj.R_MaxTorque, torque_limit);
        end

        function SetSpeed(obj, id, speed)
            calllib('dynamixel', 'dxl_write_word', id, obj.R_MovingSpeed, speed);
        end

        function Pos = GetCurrentPos(obj, id)
            Pos = calllib('dynamixel','dxl_read_word',id,obj.R_PresentPosition);
        end

        function Disconnect(obj)
            calllib('dynamixel','dxl_terminate');
            unloadlibrary('dynamixel');
        end

    end
    methods (Static)
        % convert angle in radians to angle in Dynamixel values (0-4095)
        function Pos = rad2value(positionInRadians)
            constrained = mod((positionInRadians + pi),(2*pi)) - pi;
            Pos = round((constrained + pi) * (4095) / (2*pi));
        end
    end
end