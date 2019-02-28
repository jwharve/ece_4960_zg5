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

struct gLine/* packed */
{
	unsigned char moveType;
	unsigned char tool;
	int x;
	int y;
	int theta;
}

void swapTool(struct gLine prev, struct gLine curr);

void move(struct gLine prev, struct gLine curr, float prev_z, float curr_z);

float * interp(float one, float two, unsigned long num);

int dist2steps(float dist);

int rot2steps(float rot);

int main(void)
{

	struct gLine line[2];
	FILE * fptr;
	unsigned long num = 0;
	float z[2];
	
	fptr = fopen("file.gcode","rb");
	
	readLine(fptr,line);
	swapTool(NULL,line);
	
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
		
		move(line+num%2, line+(num+1)%2, z[num%2], z[(num+1)%2]);
		
		num++;
	}

}

void move(struct gLine prev, struct gLine curr, float prev_z, float curr_z)
{
	// interp x
	
	// interp y
	
	// interp z
	
	// interp theta
	
	for (i = 0; i < numPoints; i++)
	{
		send(dist2steps(x[i]), dist2steps(y[i]), dist2steps(z[i]), rot2steps(theta[i]));
	}
}