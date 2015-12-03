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
        
        function taskone= mt_simmons()
        mt_simmons_x = 1.25;
        mt_simmons_y = .5;

        obj.nav.gpsFeedbackNav(obj.gps, mt_simmons_x, mt_simmons_y);

        % arm.sweepSnow();
        % arm.outoftheway();
        % rail.dumpSnow();
        % rail.wiggle();
        % rail.dumpDebris();
        % rail.wiggle();
        end
        
        function tasktwo = tree()
        kinect_offset = 0.65;

        %move to get close to tree
        start_point = gps.read();
        step1_x = 1.25;
        step1_y = start_point(2);
        nav.gpsFeedbackNav(gps, step1_x, step1_y);

        %call kinect
        move=kinect();
        step2_x = step1_x-move+kinect_offset;
        step2_y = step1_y;
        nav.gpsFeedbackNav(gps, step2_x, step2_y);

        %Lift sifter
        rail.raise();

        %go to step 3
        step3_x = step2_x;
        step3_y = 0;
        nav.gpsFeedbackNav(gps, step3_x, step3_y);

        % clear tree
        rail.dumpSnow(); 

        obj.nav.gpsFeedbackNav(obj.gps, tree_x, tree_y);

%         arm.outoftheway();
%         rail.raise();
%         rail.dumpSnow(); 
%         rail.wiggle();

        % typing clear all should stop motors etc from failing
        end
        
        function taskthree = roof()
        house_x = 0;
        house_y = 0;

        obj.nav.gpsFeedbackNav(obj.gps, house_x, house_y);
        rail.lower();
        arm.sweepRoof();
        end
    end
end