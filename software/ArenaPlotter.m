classdef ArenaPlotter < handle
	%UNTITLED Summary of this class goes here
	%   Detailed explanation goes here
	
	properties
		listenHandle

		fig_handle = 212;
		robotPath = zeros(3, 0);
		robotSize = 0.1651; %This is an actual-size robot, in meters
		robotIdle = 0
	
		backgroundImage
	end
	
	methods
		function obj = ArenaPlotter(arduino)
			figure(obj.fig_handle)
			set(obj.fig_handle, 'Name', 'Arena Map');
			
			% set up the background image
			field_height = 2.48;
			field_width = 3.70;

			tl = [222;146] / 2;
			br = [1548;1034] / 2;
			background = imread('Arena.PNG');
			background = background(tl(2):br(2),tl(1):br(1),:);
			background = flipud(background);
			
			RI = imref2d(size(background));
			RI.XWorldLimits = field_width * [-1 1] /2;
			RI.YWorldLimits = field_height * [-1 1] /2;
			
			obj.backgroundImage = {background, RI};
			
			% start timers
			obj.listenHandle = addlistener(arduino, 'PacketsArrived', @obj.onArduinoPacket);
		end
		
		function update(obj, ~, ~)
            set(0, 'CurrentFigure', obj.fig_handle);
			cla;
			hold on;
			
			imshow(obj.backgroundImage{:});
			set(gca,'YDir','normal');
			axis on;
			
			obj.drawRobot();
		end
		
		function drawRobot(obj)
			if ~isempty(obj.robotPath)
				if obj.robotIdle
					blobColor = 'green';
				else
					blobColor = 'red';
				end
				robotPos = obj.robotPath(:,1);
				robotXCenter = robotPos(1);
				robotYCenter = robotPos(2);
				robotAngle = (180/pi)*robotPos(3);
				robotBlob = patch(...
					robotXCenter + obj.robotSize*[-1 -1 0   1  1],...
					robotYCenter + obj.robotSize*[-1  1 1.5 1 -1],...
					blobColor);
				rotate(robotBlob, [0,0,1], robotAngle - 90, [robotXCenter, robotYCenter, 0]);
			end
		end
		
		function onArduinoPacket(obj, ~, data)
			for packet = data.Packet;
				packet = packet{1};
				if strcmp(packet.type, 'location')
					obj.robotPath = [[packet.x; packet.y; packet.phi] obj.robotPath ];

					obj.robotIdle = packet.status == 1;
				end
				if size(obj.robotPath, 2) > 1000
					obj.robotPath = obj.robotPath(:, 1:1000);
				end
			end
			obj.update();
		end
		
		function delete(obj)
			delete(obj.listenHandle);
		end
	end
	
end

