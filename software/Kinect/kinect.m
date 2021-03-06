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
imwrite(im_color,'color_125cm_2.png');
imwrite(im_depth,'depth_125cm_2.png');

%Stop
stop([colorVid depthVid]);
disp('Picture taken!');

%% Perform object detection

im_color2 = double(im_color);
im_depth2 = double(im_depth);

%Create gaussian multivariate normal distribution 
mu=[162.927745664740,67.5626204238921,72.4499036608863];
sigma=[1161.30465631311,1060.01512918727,891.004089421269;1060.01512918727,1081.52041414212,911.879283144137;891.004089421269,911.879283144137,821.917007219630];

%Get size
im_size = size(im_color2); 
rowim = im_size(1);
columnim = im_size(2);

%Mahalanobis distance of a color within "gaussian"
%grayscale image
reshaped=reshape(im_color2(:,:,:),[],3);
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
imshow(im_color), hold on
distance_mask=zeros([rowim,columnim]);
points_xy=zeros([rowim,columnim]);
nb_points = 0;

for k = 1:length(lines)
    xy = [lines(k).point1; lines(k).point2];
    angle = atan2(xy(2,2)-xy(1,2),xy(2,1)-xy(1,1))*180/pi;
    if angle < 30
        nb_points = nb_points + 1;
        plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');
        plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
        plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');
        distance_mask(xy(1,2),xy(1,1))=1;
        distance_mask(xy(2,2),xy(2,1))=1;
        points_xy(xy(1,2),xy(1,1))=1;
        nb_points = nb_points+1;
        points_xy(xy(2,2),xy(2,1))=1;
        for i=1:3
            abscisse= round(xy(1,2)+0.25*i*(xy(2,2)-xy(1,2)));
            ordonnee= round(xy(1,1)+0.25*i*(xy(2,1)-xy(1,1)));
            distance_mask(abscisse,ordonnee)=1;
            nb_points = nb_points+1;
            points_xy(abscisse,ordonnee)=1;
        end
    end
end

points_array = zeros([2,nb_points]);
normd=zeros([1,nb_points]);
points=zeros([nb_points,2]);

% Find distance
figure;
imshow(distance_mask);
reshaped_mask=reshape(distance_mask,[],1);
col_depth=find(reshaped_mask);

reshaped_depth=reshape(im_depth2(:,:,:),[],1);
distance_depth=reshaped_depth(col_depth,:);
move=mean(distance_depth)

%Find angle
% nb_points_2=0;
% for k = 1:length(lines)
%     xy = [lines(k).point1; lines(k).point2];
%     angle = atan2(xy(2,2)-xy(1,2),xy(2,1)-xy(1,1))*180/pi;
%     if angle < 30
%         nb_points_2 = nb_points_2 + 1;
%         points_array(1,nb_points_2)=xy(1,2);
%         points_array(2,nb_points_2)=xy(1,1);
%         nb_points_2 = nb_points_2+1;
%         points_array(1,nb_points_2)=xy(2,2);
%         points_array(2,nb_points_2)=xy(2,1);
%         for i=1:3
%             abscisse= round(xy(1,2)+0.25*i*(xy(2,2)-xy(1,2)));
%             ordonnee= round(xy(1,1)+0.25*i*(xy(2,1)-xy(1,1)));
%             nb_points_2 = nb_points_2+1;
%             points_array(1,nb_points_2)=abscisse;
%             points_array(2,nb_points_2)=ordonnee;
%         end
%     end
% end
% 
% reshaped_mask_angle=reshape(points_xy,[],1);
% col_depth_angle=find(reshaped_mask_angle);
% 
% reshaped_depth_angle=reshape(im_depth2(:,:,:),[],1);
% distance_depth_angle=reshaped_depth_angle(col_depth_angle,:);
% 
% alpha = 57*pi/2*180;
% 
% for i=1:nb_points
%     normd(i) = sqrt(points_array(1,i)*points_array(1,i)/4+(rowim/(2*tan(alpha)))*(rowim/(2*tan(alpha))));
% end
% 
% for i=1:nb_points
%     points(i,1) = points_array(1,i)*distance_depth_angle(i,1)/normd(i);
%     points(i,2) = rowim*distance_depth_angle(i,1)/(2*tan(alpha)*normd(i));
% end
% 
% linear_reg=fit(points(:,1),points(:,2),'linear');
% angle = linear_reg.Coefficients.Estimate(1);
