function plot_arena
	field_height = 2.48;
	field_width = 3.70;

	tl = [221;145];
	br = [1548;1034];

	background = imread('Arena.PNG');
	size(background)

	clipped = background(tl(2):br(2),tl(1):br(1),:);

	RI = imref2d(size(clipped));
	RI.XWorldLimits = [0 field_width];
	RI.YWorldLimits = [0 field_height];
	set(gca,'YDir','normal');
	imshow(flipud(clipped), RI);
	axis on;
end