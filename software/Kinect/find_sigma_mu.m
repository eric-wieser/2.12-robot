%% Input
im_color=imread('im_color.jpg');   % color image taken by kinect
[~,~,mask_im_color]=imread('mask_im_color.png');  % image processed by paint

%% Find mu, sigma
reshaped=reshape(im_color(:,:,:),[],3);
mask=mask_im_color > 128;
reshaped_mask=reshape(mask,[],1);
col=find(reshaped_mask);

reshaped=reshaped(col,:);
sigma=cov(double(reshaped));
mu=mean(reshaped);
