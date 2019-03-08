void move(struct gLine * prev, struct gLine * curr, float prev_z, float curr_z, unsigned char E)
{
	struct packet = current;
	int num_steps; 
	float * x;
	float * y;
	float * z;
	float * theta;
	
	num_steps = numSteps(prev, curr)
	
	x = (float * )malloc(num_steps*sizeof(float));
	y = (float * )malloc(num_steps*sizeof(float));
	z = (float * )malloc(num_steps*sizeof(float));
	theta = (float * )malloc(num_steps*sizeof(float));
	
	x = interp(prev.x,curr.x,num_steps);
	y = interp(prev.y,curr.y,num_steps);
	z = interp(prev_z,curr_z,num_steps);
	theta = interp(prev.theta,curr.theta,num_steps);
	
	for (i = 0; i < numPoints; i++)
	{
		current = calcStep(dist2steps(x[i]), dist2steps(y[i]), dist2steps(z[i]), rot2steps(theta[i]), 1);
		printPacket(current);
		sendPacket(current);
	}
	
	free(x); free(y); free(z); free(theta);
}

int numSteps(struct gLine prev, struct gLine curr)
{
	struct point one, two;
	float dist;
	float angle;
	
	one.x = prev.x;
	one.y = prev.y;
	if (prev.moveType == MOVE)
	{
		one.z = MOVE_HEIGHT;
	}
	else
	{
		one.z = DRAW_HEIGHT;
	}
	
	two.x = curr.x;
	two.y = curr.y;
	if (curr.moveType == MOVE)
	{
		two.z = MOVE_HEIGHT;
	}
	else
	{
		two.z = DRAW_HEIGHT;
	}
	
	dist = distance(one,two);
	angle = fabsf(curr.theta - prev.theta);
	
	if (dist*DIST_WEIGHT > angle*ANGLE_WEIGHT)
	{
		return (int) dist*DIST_WEIGHT;
	}
	else
	{
		return (int) angle*ANGLE_WEIGHT;
	}
}

struct packet calcStep(float x, float y, float z, float theta, char E)
{
	struct point draw;
	struct packet ret;
	draw.x = x;
	draw.y = y;
	draw.z = z;
	
	ret.S0 = dist2steps(dist(addP(draw,h0),post0));
	ret.S1 = dist2steps(dist(addP(draw,h1),post1));
	ret.S2 = dist2steps(dist(addP(draw,h2),post2));
	ret.R = rot2steps(theta);
	ret.E = E;
	
	return ret;
}

void swapTool(struct gLine prev, struct gLine curr) 
{

	return;

	//grab tool initially
	//go to prev tool position and release tool
	//go to new tool position from old tool position and grab new tool

	if (prev == NULL) 
	{
		//initial tool setup
		//go to toolbed and pick up tool

	} else 
	{

		//after inital tool setup
		//go to prev.tool position and drop it
		//go to new tool position from old tool position and grab new tool

		
	}


}