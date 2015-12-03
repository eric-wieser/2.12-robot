classdef Tasks < handle
	%NAVIGATOR Summary of this class goes here
	%   Detailed explanation goes here

	properties
		gps
        nav
        time
    end
    
    
    methods
		function obj = Tasks(nav,gps)
			obj.gps = gps;	
            obj.nav = nav;
        end
        
                
        function taskone= mt_simmons(obj)
        mt_simmons_x = 1.219;
        mt_simmons_y = .33;
        
        obj.nav.gpsFeedbackNav(obj.gps, mt_simmons_x, mt_simmons_y);
        

        %arm.sweepSnow();
        %arm.outoftheway();
        %rail.dumpSnow();
        %rail.wiggle();
        %rail.dumpDebris();
        %rail.wiggle();
        end
        
        function tasktwo = tree(obj)
        tree_x = .248;
        tree_y = -.987;

        obj.nav.gpsFeedbackNav(obj.gps, tree_x, tree_y);

        %arm.outoftheway();
        %rail.raise();
        %rail.dumpSnow(); 
        %rail.wiggle();
        % typing clear all should stop motors etc from failing
        end
        
        function taskthree = roof(obj)
        house_x = 0.008;
        house_y = 0.372;

        obj.nav.gpsFeedbackNav(obj.gps, house_x, house_y);
        %rail.lower();
        %arm.sweepRoof();
        end
    end
end