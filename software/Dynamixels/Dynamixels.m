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
            % this is the Register on the Servo that corresponds to the
            % goal pos (positive to the up)
            P_GOAL_POSITION = 30;
            % write it
                init_pos1 = -15803;%500;
                init_pos2 = 1102;%6500;%2500;
%             init_pos1 = -500;
%             init_pos2 = 4000;
            if (id == 1)
                calllib('dynamixel','dxl_write_word',id,P_GOAL_POSITION,Pos+init_pos1);
            else
                calllib('dynamixel','dxl_write_word',id,P_GOAL_POSITION,-Pos+init_pos2);  %attention servo do not turn in same direction
            end
        end
        
        function freeze(obj, id, Pos)
            % this is the Register on the Servo that corresponds to the
            % goal pos (positive to the up)
            P_GOAL_POSITION = 30;
            % write it
            if (id == 1)
                calllib('dynamixel','dxl_write_word',id,P_GOAL_POSITION,Pos);
            else
                calllib('dynamixel','dxl_write_word',id,P_GOAL_POSITION,Pos);  %attention servo do not turn in same direction
            end
        end
        
        function SetAngle(obj, id, addr, ang)
            value = obj.rad2value(ang);
            calllib('dynamixel', 'dxl_write_word', id, addr, value);
        end        
        
        function SetBounds(obj, id, min, max)
            SetAngle(obj, id, 6, min);
            SetAngle(obj, id, 8, max);
        end
        
       function SetTorque(obj, id, torque_limit)
            calllib('dynamixel', 'dxl_write_word', id, 14, torque_limit);
       end
        
       function SetSpeed(obj, id, speed)
            calllib('dynamixel', 'dxl_write_word', id, 32, speed);
       end
       
       function Pos = GetCurrentPos(obj, id)
            % this is the Register on the Servo that corresponds to the current pos
            P_CURRENT_POSITION = 36;
            % read it
            Pos = calllib('dynamixel','dxl_read_word',id,P_CURRENT_POSITION);
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