numDXL = 2;

Baud = 4;

Port = 6;

P_GOAL_POSITION = 30;

P_CURRENT_POSITION = 36;


loadlibrary('dynamixel','dynamixel.h')
id = zeros(numDXL);
res = calllib('dynamixel','dxl_initialize',Port,Baud);
if res == 1
    disp('Connected to USB2Dynamixel!');
    for i = 1:numDXL
        id(1,i) = i;
    end
    disp('Connected to'); disp(length(id)); disp('Dynamixel Motors');
else
    disp('Failed to open USB2Dynamixel!');
end

calllib('dynamixel', 'dxl_write_word',1,24, 0);

while 1~=0
    pos = calllib('dynamixel', 'dxl_read_word',1,P_CURRENT_POSITION);
    calllib('dynamixel', 'dxl_write_word',2,P_GOAL_POSITION, pos);
end

    

