function [x, y, phi, timestamp ] = getVals(sObj)
  %GETVALS Returns [x y phi timestamp]
  %   Argument: Socket object
  fwrite(sObj,'r');
  data = [fread(sObj,3,'single')' fread(sObj,1,'single')];
  x = data(1);
  y = data(2);
  phi = data(3);
  timestamp = data(4);
end