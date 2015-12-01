%% Set up Kinect and take image data
clc;
clear;
close all;

imaqreset;
pause(2); % wait for two seconds

utilpath = fullfile(matlabroot, 'toolbox', 'imaq', 'imaqdemos', 'html', 'KinectForWindows');
addpath(utilpath);
colorVid = videoinput('kinect', 1, 'RGB_640x480');
depthVid = videoinput('kinect', 2, 'Depth_640x480');

% Set frame to take image per trigger
colorVid.FramesPerTrigger = 1;
depthVid.FramesPerTrigger = 1;
colorVid.TriggerRepeat = 1;
depthVid.TriggerRepeat = 1;
triggerconfig([colorVid depthVid],'manual'); % manually triggered
start([colorVid depthVid]);

% Take color/depth images of the scene
trigger([colorVid depthVid]); %trigger both objects
im_color = getdata(colorVid);
im_depth = getdata(depthVid);

%Stop
stop([colorVid depthVid]);
disp('...Done!');

% View captured images
figure(1)
subplot(1,2,1)
imshow(im_color);
subplot(1,2,2)
imshow(im_depth, []);

% save data and images