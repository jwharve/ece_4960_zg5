width = 42;
length = 48;
spool_bottom_from_bot = length + 24-15;
desired_height = 70;
thickness = 3;
tol = 0.5;


pad_height = desired_height - spool_bottom_from_bot;



difference()
{
    cube(size = [width + thickness, width + thickness, length+pad_height], center = true);
    
    translate(v = [0, 0, pad_height/2])
        cube(size = [width+tol, width+tol, length], center = true);
}