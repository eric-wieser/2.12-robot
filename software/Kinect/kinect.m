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
im_color = double(getdata(colorVid));
im_depth = double(getdata(depthVid));
imwrite(im_color,'color_125cm_2.png');
imwrite(im_depth,'depth_125cm_2.png');

%Stop
stop([colorVid depthVid]);
disp('...Done!');

% View captured images
figure(1)
subplot(1,2,1)
imshow(im_color);
subplot(1,2,2)
imshow(im_depth, []);

%% Perform object detection

im_colour= imread('colour_125cm.jpg');
im_color = double(im_colour);
im_depth = double(imread('depth_125cm.png'));
%Create gaussian multivariate normal distribution 
mu=[162.927745664740,67.5626204238921,72.4499036608863];
sigma=[1161.30465631311,1060.01512918727,891.004089421269;1060.01512918727,1081.52041414212,911.879283144137;891.004089421269,911.879283144137,821.917007219630];

%Get size
im_size = size(im_color); 
rowim = im_size(1);
columnim = im_size(2);

%Mahalanobis distance of a color within "gaussian"
%grayscale image
reshaped=reshape(im_color(:,:,:),[],3);
inverse = inv(sigma);

for i=1:rowim*columnim
    mahalanobis_distance(i) = (reshaped(i,:)-mu)*inverse*(reshaped(i,:)-mu)'; 
    mahalanobis_distance(i)= exp(-0.25*mahalanobis_distance(i));
end

mahalanobis_distance=reshape(mahalanobis_distance(:),[rowim,columnim]);
%Threshold to get a binary image
thresh = 0.2;
mask = im2bw(mahalanobis_distance, thresh);

%Hough transform
[H,theta,rho] = hough(mask);
P = houghpeaks(H,5,'threshold',ceil(0.3*max(H(:))));
lines = houghlines(mask,theta,rho,P,'FillGap',5,'MinLength',7);
figure;
imshow(im_colour), hold on
distance_mask=zeros([rowim,columnim]);

for k = 1:length(lines)
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');
    plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
    plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');
    distance_mask(xy(1,2),xy(1,1))=1;
    distance_mask(xy(2,2),xy(2,1))=1;
end

% Find distance
figure;
imshow(distance_mask);
distance_mask = 1-distance_mask;
reshaped_mask=reshape(distance_mask,[],1);
col_depth=find(reshaped_mask);

reshaped_depth=reshape(im_depth(:,:,:),[],3);
distance_depth=reshaped_depth(col_depth,:);
move=mean(reshaped);


