%TA Solution to 2.12 Lab
%Written by Fangzhou Xia, Michael, Melanie, and Daniel
%Be sure to close the GUI before uploading the Arduino Code
function navigation
	nextData = [0 0 0]; % used to hold variable in the 'Manual' mode
	
	windowWidth = 1000; %WindowWidth is the number of location points to keep
	robotSize = 0.1651; %This is an actual-size robot, in meters
	%  Construct the GUI components.
	f = figure(...
		'Visible','off',...
		'Position',[360,500,700,450],...
		'Name','2.12 Matlab Interface');
	f_send = uicontrol(...
		'Style','pushbutton',...
		'String','Send Next Point',...
		'Position',[500,250,100,40],...
		'Callback',{@f_send_button_Callback});
	f_text = uicontrol(...
		'Style','text',...
		'String','Select Mode',...
		'Position',[430,380,100,20]);
	f_mode = uicontrol(...
		'Style','popupmenu',...
		'String',{'Manual'},...
		'Position',[550,385,100,18],...
		'Callback',{@f_mode_menu_Callback});
	f_a = axes(...
		'Units','Pixels',...
		'Position',[50,50,350,350]);
	axis 'equal';
	f_table = uitable(f,...
		'Data',nextData,...
		'ColumnName',{'X (m)','Y(m)','Phi(deg)'},...
		'ColumnEditable', [true true true],...
		'RowName',[],...
		'Position',[420,300,228,50]);
	set([f,f_a,f_send,f_text,f_mode,f_table],'Units','normalized');

	%Try starting a serial connection with the Arduino
	try
		arduino = Arduino('COM8');  % Use Arduino IDE to see which com port the Arduino UNO is using
	catch e
		switch e.identifier
			case 'MATLAB:serial:fopen:opfailed'
				display('Serial connection could not be established. Make sure the Arduino is plugged in and on the right port (check in Arduino IDE).');
				return; %Quit the program
			otherwise
				rethrow(e)
		end
	end

	c = onCleanup(@() delete(arduino));
	%Create blank vectors for the incoming serial data
	xPos = linspace(0, 0, windowWidth*3);
	yPos = linspace(0, 0, windowWidth*3);

	%Run the graphing window forever, until someone hits X on the graph
	while true
		%If the graph is gone because someone hit X, exit the graphing loop
		if ~ishghandle(f)
			break;
		end
		%Try reading new serial data

		%Keep reading and processing until the buffer is almost empty
		any_packet = false;
		while true
			% keep reading packets until we've had at least one, and there are no more
			try
				packet = arduino.recv_packet();
			catch e
				switch e.identifier
					case 'arduino:decode:truncated'
						disp(['Packet error' e.message]);
						continue
					otherwise
						rethrow(e)
				end
			end
			if isempty(packet)
				if any_packet
					break
				else
					continue
				end
			end
			% do something with the packet
			switch packet.type
				case 'debug'
					display(['Debug: ' packet.message]);
				case 'location'
					newxPos = packet.x;
					newyPos = packet.y;
					newAngle = (180/pi)*packet.phi;
					newStatus = packet.status;
					%Drop the first elements of the data vectors
					xPos(1) = [];
					yPos(1) = [];
					%Append the new values to the _mdata vectors
					xPos = [xPos newxPos];
					yPos = [yPos newyPos];
					
					any_packet = true;
			end
		end

		%Draw the robot at the latest x,y location
		switch newStatus
			case 1
				blobColor = 'green';
			case 0
				blobColor = 'red';
		end
		cla(f_a);
		plot_arena;
		hold on;

		robotXCenter = xPos(windowWidth*3);
		robotYCenter = yPos(windowWidth*3);
		robotBlob = patch(...
			robotXCenter + [-robotSize -robotSize 0             robotSize  robotSize],...
			robotYCenter + [-robotSize  robotSize 1.5*robotSize robotSize -robotSize],...
			blobColor);
		rotate(robotBlob, [0,0,1], newAngle - 90, [robotXCenter, robotYCenter, 0]);

		% draw the center of the robot
		plot(f_a, robotXCenter, robotYCenter, 'o', 'Color', blobColor);

		plot(f_a, xPos, yPos, '.');

		hold on;
		nextData = get(f_table,'Data');
		plot(nextData(1), nextData(2),'*');

		% Make the GUI visible.
		set(f,'Visible','on');
		drawnow
	end

	%Close the connection
	delete(arduino);

	%clear serialConnection;
	display('Serial Connection gracefully closed!');

	%send button push handler function
	function f_send_button_Callback(source,eventdata)
		% Display surf plot of the currently selected data.
		nextData = get(f_table,'Data');
		packet = struct();
		packet.type = 'dest';
		packet.x = nextData(1);
		packet.y = nextData(2);
		packet.phi = pi/180*nextData(3);
		arduino.send_packet(packet);

		display(['Point command sent successfully with X = ',num2str(nextData(1)),...
			'm, Y = ',num2str(nextData(2)),...
			'm, Phi = ',num2str(nextData(3)),'deg']);
		pause(1);
		%surf(current_data);
	end

	function f_mode_menu_Callback(source,eventdata)
		% Determine the selected data set.
		str = get(source, 'String');
		val = get(source,'Value');
		% Set current data to the selected data set.
		send_mode = str{val};
		display(send_mode);
	end
end
