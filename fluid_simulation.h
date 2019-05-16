/* Set boundary of simulation box */
void SetBoundary(int b, float *x, int N);

/* Solve linear differential equation */
void LinSolve(int b, float *x, float *x0, float a, float c, int iter, int N);

/* Make equation linear */
void Diffuse (int b, float *x, float *x0, float diff, float dt, int iter, int N);

//TODO: Check what it does
void Project(float *velocX, float *velocY, float *p, float *div, int iter, int N);

/* Move object by vector */
void AddVection(int b, float *d, float *d0,  float *velocX, float *velocY, float dt, int N);
