void printPacket(struct packet p);

void initGlobal(void);

int readLine(FILE * fptr,struct gLine * curr);

void send(float x, float y, float z, float theta, char e);

float * interp(float one, float two, unsigned long num);

float addP(struct point one, struct point two);

float distance(struct point one, struct point two);

int dist2steps(float dist);

int rot2steps(float rot);