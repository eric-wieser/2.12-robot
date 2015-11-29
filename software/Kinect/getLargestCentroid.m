function [xc, yc, ac] = getLargestCentroid(bwIm)
%Returns the centroid and area of the largest blob in the segmented image

% Find the pixels of the largest blob
% Search for and label each contiguous section (blob)
[labeledIm, totalLabels] = bwlabel(bwIm, 8);

% Find the largest blob and get its pixels' coordinates and the number of
% pixels
[yVals, xVals] = find(labeledIm == 1);
bestSize = length(yVals);
for i = 2:totalLabels
  [testY, testX] = find(labeledIm == i);
  if length(testY) >bestSize
    yVals = testY;
    xVals = testX;
    bestSize = length(testY);
  end
end


%  Find the centroid of the pixels belonging to the largest blob
ac = length(xVals);
xSum = sum(xVals);
ySum = sum(yVals);
xc = xSum/ac;
yc = ySum/ac;

end

