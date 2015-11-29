%{
lab5.m
Lab 5: Computer Vision

2.12 Introduction to Robotics
Fall 2015
%}

clear; clc; close all
% supportPackageInstaller; % install from internet for kinect/webcam drivers
% imaqtool; % connect to video devices

%% Section 1: Set up Kinect and take image data (Do Not Edit)
imaqreset;
pause(2); % wait for two seconds

utilpath = fullfile(matlabroot, 'toolbox', 'imaq', 'imaqdemos', 'html', 'KinectForWindows');
addpath(utilpath);
colorVid = videoinput('kinect', 1, 'RGB_640x480');
depthVid = videoinput('kinect', 2, 'Depth_640x480');
% depthVid = imaq.VideoDevice('kinect', 2, 'Depth_640x480');
srcDepth = getselectedsource(depthVid);

% Show image seen by Kinect
preview(depthVid);
% if not clear
%preview(colorVid);

% Set frame to take image per trigger
colorVid.FramesPerTrigger = 1;
depthVid.FramesPerTrigger = 1;
colorVid.TriggerRepeat = 1;
depthVid.TriggerRepeat = 1;
triggerconfig([colorVid depthVid],'manual'); % manually triggered
start([colorVid depthVid]);

% Take color image of the scene without the object
disp('Be ready to take snapshot without object in 5 s ...');
pause(5);
trigger([colorVid depthVid]); %trigger both objects
bg1 = getdata(colorVid);
bgdp = getdata(depthVid);

% Take color image of the scene with the object
disp('Please hold up an object for 5 s ...');
pause(5);
trigger([colorVid depthVid]); %trigger both objects
im1 = getdata(colorVid);
imdp = getdata(depthVid);


% Take color image of the scene without the object
% disp('Be ready to take snapshot without object in 5 s ...');
% pause(5);
% bg1 = getsnapshot(colorVid); %bg = background
% dpbg1 = step(depthVid);

% Take color image of the scene with the object
% disp('Please hold up an object for 5 s ...');
% pause(5);
% im1 = getsnapshot(colorVid); %im  = image
% dpim1 = step(depthVid);

stop([colorVid depthVid]);
% stop(colorVid);
% release(depthVid);

disp('...Done!');
closepreview(depthVid);

% View captured images
figure(1)
subplot(1,2,1)
imshow(bg1);
subplot(1,2,2)
imshow(bgdp, []);
figure(2)
subplot(1,2,1)
imshow(im1);
subplot(1,2,2)
imshow(imdp, []);

% save data and images
%%  Section 2: Subtraction Method (using the images with and without the object)

% Initialize timer
tic;

% Convert to greyscale
bgGray = rgb2gray(bg1);
imGray = rgb2gray(im1);

% Subtract the greyscale matrices
imDiff = imGray-bgGray;

% Visualize the raw difference image
figure(3)
imshow(imDiff);
title('Raw Subtracted Image');

% Use im2bw2(Image, threhold level) to seperate only ball from background
% level = 0 (black) means no difference between two images, want anything other than black.
level = .1;
BW1 = im2bw2(imDiff,level);

% Visualize the filtered difference image
figure(4)
imshow(BW1)
title('Filtered Subtracted Image');

% Get the centroid and area of the largest blob that is in the image
[xc, yc, areac] = getLargestCentroid(BW1);

% End timer for subtraction method
SUBTime = toc;
fprintf('The subtraction method takes %f seconds. \n',SUBTime);

% Plot original image and our detected object centriod
figure(5)
imshow(im1);
hold on;
plot(xc,yc,'b +');
plotCircle(xc,yc,sqrt(100/pi));
title('Object Detection using Subtraction');
hold off;

%% Section 3: Histogram Thresholding (using only the image with the object)
figure(6)
imhist(imGray);

% manual thresholding
manThresh = 120/255; % has to be normalized to max possible intensity value
imBWMan = im2bw(imGray, manThresh); % filter image based on threshold

figure(7)
imshow(imBWMan);
title('Filtered image using manually selected histogram thresholding');

% Initialize timer
tic;

% otsu thresholding
otsuThresh = graythresh(imGray);
display(otsuThresh*255) % optimal otsu threshold value
imBWOtsu = im2bw(imGray,otsuThresh); % filter image based on threshold

figure(8)
imshow(imBWOtsu);
title('Filtered image using Otsu Optimal Thresholding');

% Get the centroid and area of the largest blob that is in the image
[xc, yc, areac] = getLargestCentroid(imBWOtsu);

% End timer for Otsu Thresholding method
SUBTime = toc;
fprintf('The Otsu Thresholding method takes %f seconds. \n',SUBTime);

% Plot original image and our detected object centriod
figure(9)
imshow(im1);
hold on;
plot(xc,yc,'b +');
plotCircle(xc,yc,sqrt(100/pi));
title('Object Detection using Histogram Thresholding');
hold off;

%%  Section 4: RGB Thresholding (using only the image with the object)

% Initialize timer
tic;

% Define the RGB thresholds for the pixels we want to keep.
% Every pixel within these thresholds becomes 1, the rest 0.
Rplus = 1.0; % red upper boundary
Rminus = 0.2; % red lower boundary
Gplus = 0.9; % green upper boundary
Gminus = 0.0; % green lower boundary
Bplus = .9; % blue upper boundary
Bminus = 0.0; % blue lower boundary

% Threshold each color component between upper and lower boundary
imR = im2bw2(im1(:,:,1),Rminus).*~im2bw2(im1(:,:,1),Rplus);
imG = im2bw2(im1(:,:,2),Gminus).*~im2bw2(im1(:,:,2),Gplus);
imB = im2bw2(im1(:,:,3),Bminus).*~im2bw2(im1(:,:,3),Bplus);

% Combine each component for the final filtered image (in grayscale)
imBW2 = imR.*imG.*imB;

figure(10)
imshow(imBW2)
title('Filtered image using RGB Thresholding');

% Get the centroid and area of the largest blob that is in the image
[xc, yc, areac] = getLargestCentroid(imBW2);

% End timer for RGB Thresholding
RGBTime = toc;
fprintf('The RGB Thresholding takes %f seconds. \n', RGBTime);

%Plot results
figure(11);
imshow(im1);
hold on;
plotCircle(xc,yc,sqrt(100/pi));
plot(xc,yc,'b +');
title('Object Detection using RGB Thresholding');
hold off;

%%  Section 5:  HSV Thresholding (using only the image with the object)

%Start timer
tic;

% Convert RGB Image to HSV so that each pixel is a vector [Hue, Saturation, Value] instead of [R,G,B]
imHSV = rgb2hsv_fast(im1);

% Select center value and +- threshold boundaries (from 0 to 1)
hVal = 0.07;
hThresh = 0.05;
sVal = 0.5;
sThresh = 0.4;
vVal = 0.5;
vThresh = 0.5;

imHSV = imHSV*255;

% Threshold each HSV component (center value +/- threshold boundaries)
imH = im2bw2(imHSV(:,:,1),hVal-hThresh).*~im2bw2(imHSV(:,:,1),hVal+hThresh);
imS = im2bw2(imHSV(:,:,2),sVal-sThresh).*~im2bw2(imHSV(:,:,2),sVal+sThresh);
imV = im2bw2(imHSV(:,:,3),vVal-vThresh).*~im2bw2(imHSV(:,:,3),vVal+vThresh);

%Combine each component for a final filtered image (in grayscale)
imBW3 = imH.*imS.*imV;

figure(12);
imshow(imBW3);
title('Filtered image with HSV Thresholding')

% Find the centroid and area of the largest blob in our thresholded image
[xc, yc, areac] = getLargestCentroid(imBW3);

%End timer for HSV Thresholding
HSVTime = toc;
fprintf('The HSV Thresholding  takes %f seconds. \n',HSVTime);

%Plot results
figure(13);
imshow(im1);
hold on;
plotCircle(xc,yc,sqrt(100/pi));
plot(xc,yc,'b +');
title('Object Detection using HSV Thresholding');
hold off;

%% Section 6: Hough Transform (using only the image with the object)

figure(13)
imshow(imGray);
[centers, radii, metric] = imfindcircles(imGray,[40 50],'ObjectPolarity','bright', 'Sensitivity',0.98,'EdgeThreshold',0.05);
if(~isempty(radii))
    viscircles(centers (1,:), radii(1),'EdgeColor','b');
end

% also try using the depth data
imdp2 = 4000*ones(size(imdp), 'uint16') - imdp;
imdp2(imdp2 == 4000) = 0;
% imp2 = imdp;
figure(14)
imshow(imdp2, [0 4000]);
[centers, radii, metric] = imfindcircles(imdp2,[10 20],'ObjectPolarity','bright', 'Sensitivity',0.95,'EdgeThreshold',0.05);
if(~isempty(radii))
    viscircles(centers(1,:), radii(1),'EdgeColor','r');
end

%% Section 7: Live Object Detection

% Histogram Thresholding
frames = 100;
colorVid.TriggerRepeat = frames;
start(colorVid);
for i=1:frames-1
    trigger(colorVid); %trigger both objects
    im = imresize(getdata(colorVid), 0.5);
    otsuThresh = graythresh(im);
    imBWOtsu = im2bw(im,otsuThresh);
    figure(15); imshow(imBWOtsu);
    %     im = imresize(getsnapshot(colorVid),.5);
    %getlargestcentriod
end
stop(colorVid);

% RGB Thresholding


% HSV Thresholding


% Hough Transform
% With grayscale color data
frames = 100;
colorVid.TriggerRepeat = frames;
start(colorVid);
for i=1:frames-1
    trigger(colorVid); %trigger both objects
    im = imadjust(rgb2gray(imresize(getdata(colorVid), 0.5)));
    %     im = imadjust(rgb2gray(imresize(getsnapshot(colorVid),.5)));
    figure(18); imshow(im);
    [centers, radii, metric] = imfindcircles(im,[10 20],'ObjectPolarity','bright', 'Sensitivity',0.95,'EdgeThreshold',0.05);
    viscircles(centers(1,:), radii(1),'EdgeColor','b');
    %display(centers);
end
stop(ColorVid);

% With depth data
% depthVid2 = imaq.VideoDevice('kinect', 2, 'Depth_640x480');
frames = 100;
depthVid.TriggerRepeat = frames;
start(depthVid);
for i=1:frames-1
    trigger(depthVid);
    depthImage = imresize(getdata(depthVid),0.5);
    %     depthImage = step(depthVid2);
    %     depthImage = imresize(depthImage,.25);
        depthImage = 4000*ones(size(depthImage), 'uint16') - depthImage;
        depthImage(depthImage == 4000) = 0;
    
    imshow(depthImage, [0 4000]);
    [centers, radii, metric] = imfindcircles(depthImage,[10 20],'ObjectPolarity','bright', 'Sensitivity',0.95,'EdgeThreshold',0.05);
    if(~isempty(radii))
        centerBest = centers(1,:);
        radiusBest = radii(1);
        viscircles(centerBest,radiusBest,'EdgeColor','r');
    end
end
% release(depthVid2);
stop(depthVid);
%% Read and crop saved images

%Read images. Default is (480 rows x 640 pixels per row x 3 colors),
% im1(:,:,1) is the red component, im1(:,:,2) is the green component,
% im1(:,:,3) is the blue component.
% RGB values range from [0:255]

% bg1 = imread('bg1.jpg');
% im1 = imread('im1.jpg');

% Crop the image to show only the field
% bg1 = bg1(90:420,80:600,:);
% im1 = im1(90:420,80:600,:);
%
% figure(51)
% imshow(bg1);

