$fn=50;
module main()
{
	module spur()
	{
		linear_extrude(height=20) polygon([[-1,-1],[-1,1],[0.7,0.7],[0.7,-0.7]],[[0,1,2,3,0]]);
	}
	
 difference()
 {	 
 	union()
 	{
 		//base
 		rotate_extrude()
 		{
 				square([9,8]);
 				square([10,7]);
 				translate([9,7]) circle(1);
 		}
    	
    	//shaft
    	cylinder(r=5,h=20);
    	
    	//spurs
    	for(i=[1:8]) rotate([0,0,i*(360/8)])
    	translate([5.5,0,0])spur();
   }
   
   //shaft hole
    translate([0,0,-1])cylinder(r=2.5,h=22);
    		
    //captive nut and grub holes
    for(j=[1:3]) rotate([0,0,j*(360/3)])
	translate([0,20,4])rotate([90,0,0])
	union()
	{
		//enterance
		translate([0,-3,14.5]) cube([5.4,6,2.4],center=true);
		//nut
		translate([0,0,13.3]) rotate([0,0,30])cylinder(r=3.12,h=2.4,$fn=6);
		//grub hole
		translate([0,0,9]) cylinder(r=1.5,h=10);
	}

 }

   
}

main();
