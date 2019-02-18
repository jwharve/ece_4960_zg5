$fn = 100;
cu = 25.4;

br = 11/2*cu;
edge = 0.3*cu;
tr = 12/2*cu - edge/2;

height = 2*cu;

gear_height = 1*cu;

angle = atan2(height,(tr-br)/2);

echo(angle);

module pan()
{
    translate(v = [0, 0, -gear_height])
    linear_extrude(height = height, center = false, convexity = 0, twist = 0, slices = 20, scale = tr/br, $fn = 100)
        circle(br);
}

pan();