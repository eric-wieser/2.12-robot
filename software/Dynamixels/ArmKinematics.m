% 2.12 2-Link Inverse Kinematics
% Steven Keyes - srkeyes@mit.edu
% Oct. 2015
%
% This class represents a 2-link arm and performs inverse kinematics for it

classdef ArmKinematics
    properties
        % lengths of the first link (l1) and the second (l2)
        l1
        l2
    end
    
    methods
        function obj = ArmKinematics(l1, l2)
            % initialize the object with the link lengths
            if nargin == 2
                obj.l1 = l1;
                obj.l2 = l2;
            else
                error('Provide the lengths of the two arm links')
            end
        end
        
        % method to check if a point x,y is in the workspace of the arm
        function isIn = inWorkspace(obj, x, y)
            isIn = true;
        end
        
        % inverse kinematics function to calculate servo positions (thetas)
        % from a x,y point in global coordinates. Thetas is an array of two
        % elements: [theta1, theta2] where theta1 is for the first servo,
        % etc.
        function [theta1, theta2] = findThetas(obj, x, y)
            hyp = sqrt(x^2 + y^2);
            alpha = atan2(y,x);
            beta = acos((obj.l1^2 + hyp^2 - obj.l2^2)/(2 * hyp * obj.l1));
            gamma = acos((obj.l2^2 + obj.l1^2 - hyp^2)/(2 * obj.l1 * obj.l2));
            theta1 = pi/2 - alpha - beta;
            theta2 = pi - gamma;
        end
    end
end

