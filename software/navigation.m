%TA Solution to 2.12 Lab
%Written by Fangzhou Xia, Michael, Melanie, and Daniel
%Be sure to close the GUI before uploading the Arduino Code
function navigation
    %Change the points to get the ideal shape
    command_X = [0.2, 0.4, 0.6, 0.8]; %unit in meters
    command_Y = [0.2, 0.4, 0.6, 0.6]; %unit in meters
    command_Phi = [45, 45, 0, 0]; %unit in degrees


    %%% Don't Modify anything below this line for lab 3%%%
    %  Construct automatic mode components
    command_Index = 1; %pre-defined trajectory point index
    command_update = 1; % flag for automatic serial write
    nextData = [0 0 0]; % used to hold variable in the 'Manual' mode
    send_mode = 'Manual'; % mode can be 'Manual' or 'Automatic'
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
        'String',{'Manual','Automatic'},...
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
        serialConnection = serial('COM4');  %Use Arduino IDE to see which com port the Arduino UNO is using
        set(serialConnection, 'BaudRate', 115200); %Set the baud rate on the computer and Arduino to be the same
        fopen(serialConnection); %Try opening the serial connection
        display('Successfully connected to Arduino over Serial!');
        pause(1); %Wait for the Arduino to resete
        flushinput(serialConnection);  %Throw away the startup gibberish
    %If connecting over Serial throws an error, this catch section will run
    catch
        %Tell the user that Serial did not work
        display('Serial connection could not be established. Make sure the Arduino is plugged in and on the right port (check in Arduino IDE).');
        return; %Quit the program
    end

    c = onCleanup(@() fclose(serialConnection));
    %Create blank vectors for the incoming serial data
    xPos = linspace(0, 0, windowWidth*3);
    yPos = linspace(0, 0, windowWidth*3);

    %Flush the serial buffer before starting the infinite loop
    flushinput(serialConnection);
    %Read and discard any incomplete line
    incompleteRead = fscanf(serialConnection, '%s');
    %Initialize serialData
    serialData = '';

    while (serialConnection.BytesAvailable < 12); end;

    %Run the graphing window forever, until someone hits X on the graph
    while true
        %If the graph is gone because someone hit X, exit the graphing loop
        if ~ishghandle(f)
            break;
        end
        %Try reading new serial data
        try
            %Keep reading and processing until the buffer is almost empty
            while (serialConnection.BytesAvailable > 12)
                %Get a line of format: actual,desired\n from the Serial buffer
                serialData = fscanf(serialConnection, '%s');
    			if serialData(1) ~= 'P'
    				display(['Debug: ' serialData]);
    				continue;
    			end
    			serialData = serialData(2:end);
                %Break the line into actual and desired
                splitData = strsplit(serialData, ',');
                %Get actual and desired data as numbers instead of strings
                newxPos = str2double(splitData(1));
                newyPos = str2double(splitData(2));
                newAngle = (180/pi)*str2double(splitData(3));
                newStatus = str2double(splitData(4));
                %Drop the first elements of the data vectors
                xPos(1) = [];
                yPos(1) = [];
                %Append the new values to the _mdata vectors
                xPos = [xPos newxPos];
                yPos = [yPos newyPos];
    		end
        catch
            display(['Bad Serial Data: ' serialData]);
    		continue;
        end
        %Draw the robot at the latest x,y location
        switch newStatus
            case 1
                blobColor = 'green';
            case 0
                blobColor = 'red';
        end
        cla(f_a);
        axis 'equal';
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

        if (strcmp(send_mode, 'Automatic'))
            plot(f_a,command_X, command_Y,'-');
            plot(f_a,command_X, command_Y,'*');
        elseif (strcmp(send_mode, 'Manual'))
            hold on;
            nextData = get(f_table,'Data');
            plot(nextData(1), nextData(2),'*');
        end

        % Make the GUI visible.
        set(f,'Visible','on');
        drawnow

        %Logic for handling 'Automatic' mode operation
        if (strcmp(send_mode, 'Automatic'))
            %display(command_Index)
            if (newStatus ~= 0)
                if (abs(command_X(command_Index) - newxPos) <= 0.15) && (abs(command_Y(command_Index) - newyPos) <= 0.15)
                    %(abs(command_Phi(command_Index) - newAngle) <= 5) %Phi is not checked for lab 3
                    if command_Index < length(command_X)
                        command_Index = command_Index + 1;
                        command_update = 1;
                    else
                        display('Reached end of pre-defined trajectory!');
                        pause(5);
                    end
                else
                    display(['Matlab position check failed with X_diff =',num2str(abs(command_X(command_Index) - newxPos)),...
                        'm and Y_diff =',num2str(abs(command_X(command_Index) - newxPos))]);
                    pause(1);
                end
            end
            if (command_update ~= 0)
                packet = struct();
                packet.type = 'dest';
                packet.x = command_X(command_Index);
                packet.y = command_Y(command_Index);
                packet.phi = pi/180*command_Phi(command_Index);

                send_packet(packet);

                pause(1); %pause for the serial communication
                if command_update == 1
                    command_update = 0;
                end
            end
        end
        %display(serialData); % for debug only
    end

    %Close the serial connection
    fclose(serialConnection);
    delete(serialConnection);
    %clear serialConnection;
    display('Serial Connection gracefully closed!');

    function send_packet(packet)
        if packet.type == 'dest'
            fwrite(serialConnection,num2str(packet.x));
            fwrite(serialConnection,',');
            fwrite(serialConnection,num2str(packet.y));
            fwrite(serialConnection,',');
            fwrite(serialConnection,num2str(packet.phi));
            fwrite(serialConnection,'\n');
        else
            error('Unknown packet type')
        end
    end

    %send button push handler function
    function f_send_button_Callback(source,eventdata)
        % Display surf plot of the currently selected data.
        if strcmp(send_mode, 'Manual')
            nextData = get(f_table,'Data');
            packet = struct();
            packet.type = 'dest';
            packet.x = nextData(1);
            packet.y = nextData(2);
            packet.phi = pi/180*nextData(3);
            send_packet(packet);

            display(['Point command sent successfully with X = ',num2str(nextData(1)),...
                'm, Y = ',num2str(nextData(2)),...
                'm, Phi = ',num2str(nextData(3)),'deg']);
            pause(1); %pause for the serial communication
        else
            display('Please switch to manual mode');
        end
        %surf(current_data);
    end

    function f_mode_menu_Callback(source,eventdata)
        % Determine the selected data set.
        str = get(source, 'String');
        val = get(source,'Value');
        % Set current data to the selected data set.
        send_mode = str{val};
        display(send_mode);
        if strcmp(send_mode,'Automatic')
            fwrite(serialConnection,num2str(command_X(command_Index)));
            fwrite(serialConnection,',');
            fwrite(serialConnection,num2str(command_Y(command_Index)));
            fwrite(serialConnection,',');
            fwrite(serialConnection,num2str(pi/180*command_Phi(command_Index)));
            fwrite(serialConnection,'\n');
            pause(1); %pause for the serial communication
        end
    end
end
