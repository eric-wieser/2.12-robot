function plot_arena
	field_height = 2.48;
	field_width = 3.70;

	tl = [221;145];
	br = [1548;1034];

	background = imread('Arena.PNG');

	clipped = background(tl(2):br(2),tl(1):br(1),:);

	RI = imref2d(size(clipped));
	RI.XWorldLimits = field_width * [-1 1] /2;
	RI.YWorldLimits = field_height * [-1 1] /2;
	imshow(flipud(clipped), RI);
	set(gca,'YDir','normal');
	axis on;
end