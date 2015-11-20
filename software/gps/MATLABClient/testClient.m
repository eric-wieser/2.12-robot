%{
2.12 GPS Client Test Code
MIT 2.12 Intro To Robotics 2015
Daniel J. Gonzalez - dgonz@mit.edu
Steven Keyes - srkeyes@mit.edu
%}

%%  Set up TCP/IP Connection
serverIP = 'localhost'; %Use for loopback testing on your own computer
%serverIP ='192.168.1.212'; %Use for testing with the 2.12 servers
s1 = tcpip(serverIP,2121,'NetworkRole','Client');
fopen(s1); 

%%  Run your main loop code. Here I'm just looping 100 times.

tic; %For timing purposes

N = 1000;
for n = 1:N
  [x, y, phi, timestamp ] = getVals(s1);
end

dT = toc; %for timing purposes
avgTimePerRead = toc/N %for timing purposes

%%  Close the connection when you end the program
fclose(s1);
