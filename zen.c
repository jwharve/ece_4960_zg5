/*
Still need to add electromagnet logic
need to think about converting data types (from value read to floats)
*/

#define MOVE 1
#define DRAW 2
#define CLIEAR 1
#define BLUNT 2
#define FINE 3
#define RAKE 4

#define DRAW_HEIGHT 5
#define MOVE_HEIGHT 6

#define DIST_WEIGHT 1
#define ANGLE_WEIGHT 1


#define MOUNT_RADIUS 0.5
#define MOUNT_HEIGHT 2
#define POST_HEIGHT 18
#define POST_RADIUS 18.33333333
#define DISH_RADIUS 6
#define DISH_HEIGHT 2

#define DTR 0.01745329251   /* (pi/180) */

struct gLine/* packed */
{
	unsigned char moveType;
	unsigned char tool;
	int x;
	int y;
	int theta;
};

struct point
{
	float x;
	float y;
	float z;
} h0, h1, h2, post0, post1, post2;

struct packet
{
	int S0;
	int S1;
	int S2;
	int R;
	char E;
} __attribute__((packed));

// generate movements for swapping tools
void swapTool(struct gLine prev, struct gLine curr);

// generate and send discrete points to tiva
void move(struct gLine prev, struct gLine curr, float prev_z, float curr_z, unsigned char E);

// calc number of steps needed for each move
unsigned long numSteps(struct gLine prev, struct gLine curr);

float * interp(float one, float two, unsigned long num);

int dist2steps(float dist);

int rot2steps(float rot);

void send(x, y, z, theta, e);

int readLine(FILE * fptr,struct gLine * curr);

void initGlobal(void);

struct packet calcStep(float x, float y, float z, float theta, char E);

int main(void)
{

	struct gLine line[2];
	FILE * fptr;
	unsigned long num = 0;
	float z[2];
	
	fptr = fopen("file.gcode","rb");
	
	readLine(fptr,line);
	swapTool(NULL,line);
	
	initGlobal();
	
	while (!feof(fptr))
	{
		readLine(fptr,line + num % 2);
		
		// Check for tool swap
		if (line[num%2].tool != line[(num+1)%2].tool)
		{
			if (line[(num+1)%2].moveType != MOVE)
			{
				// ERROR!
			}
			swapTool(line+num%2,line+(num+1)%2);
		}
		
		if (line[(num+1)%2].moveType == DRAW)
		{
			z[(num+1)%2] = DRAW_HEIGHT;
		}
		else
		{
			z[(num+1)%2] = MOVE_HEIGHT;
		}
		
		move(line+num%2, line+(num+1)%2, z[num%2], z[(num+1)%2], 1);
		
		num++;
	}

}

void move(struct gLine prev, struct gLine curr, float prev_z, float curr_z, unsigned char E)
{
	struct packet = current;
	// calc num_steps
	
	// interp x
	
	// interp y
	
	// interp z
	
	// interp theta
	
	for (i = 0; i < numPoints; i++)
	{
		current = calcStep(dist2steps(x[i]), dist2steps(y[i]), dist2steps(z[i]), rot2steps(theta[i]), 1);
		sendPacket(current);
	}
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

float addP(struct point one, struct point two)
{
	struct point ret;
	ret.x = one.x + two.x;
	ret.y = one.y + two.y;
	ret.z = one.z + two.z;
	return ret;
}

float distance(struct point one, struct point two)
{
	return sqrt(pow(two.x-one.x,2) + pow(two.y-one.y,2) + pow(two.z-one.z,2));
}

void initGlobal(void)
{
	h0.x = 0;
	h0.y = -MOUNT_RADIUS;
	h0.z = MOUNT_HEIGHT;
	
	h1.x = MOUNT_RADIUS*sin(60*DTR);
	h1.y = MOUNT_RADIUS*cos(60*DTR);
	h1.z = MOUNT_HEIGHT;
	
	h2.x = -MOUNT_RADIUS*sin(60*DTR);
	h2.y = MOUNT_RADIUS*cos(60*DTR);
	h2.z = MOUNT_HEIGHT;
	
	post0.x = 0;
	post0.y = -POST_RADIUS;
	post0.z = POST_HEIGHT;
	
	post1.x = POST_RADIUS*sin(60*DTR);
	post1.y = POST_RADIUS*cos(60*DTR);
	post1.z = POST_HEIGHT;
	
	post2.x = -POST_RADIUS*sin(60*DTR);
	post2.y = POST_RADIUS*cos(60*DTR);
	post2.z = POST_HEIGHT;
}