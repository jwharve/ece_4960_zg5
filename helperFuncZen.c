




int dist2steps(float dist) {

	//circum needs to be measures
	int steps;
	int circum = 0/*??*/;

	steps = dist/circum * 200;
	return steps;
}

int rot2steps(float rot) {


	int steps;

	steps = rot * (200/360);

	return steps;
}


float * interp(float one, float two, unsigned long num) {

	float pointsArr[num];

	float increment;
	int i;

	pointsArr[0]=one;
	pointsArr[num]=two;

	increment = (two-one)/num;
	for (i= 1;i<num-1;i++) {

		pointsArr[i] = pointsArr[i-1]+increment;
	}


}

float distance(float x, float y) {

	float dist;

	dist = sqrt((x*x)+(y*y));

	return dist;
}