/* This structure represents one point in simulation */
struct FluidPoint {
    int size;
    float dt;
    float diff;
    float visc;
    
    float *s;
    float *density;
    
    float *vxCurr;
    float *vyCurr;

    float *vxPrev;
    float *vyPrev;
};

/* Free allocated memory */
void FluidPointFree(FluidCube *point)
{
    free(point->s);
    free(point->density);
    
    free(point->vxCurr);
    free(point->vyCurr);
    
    free(point->vxPrev);
    free(point->vyPrev);
    
    free(point);
}

/* Works like constructor */
FluidCube* FluidPointCreate(int size, int diffusion, int viscosity, float dt)
{
    FluidCube *point = malloc(sizeof(*point));
    int N = size;
    
    point->size = size;
    point->dt = dt;
    point->diff = diffusion;
    point->visc = viscosity;
    
    point->s = calloc(N * N, sizeof(float));
    point->density = calloc(N * N, sizeof(float));
    
    point->vxCurr = calloc(N * N, sizeof(float));
    point->vyCurr = calloc(N * N, sizeof(float));
    
    point->vxPrev = calloc(N * N, sizeof(float));
    point->vyPrev = calloc(N * N, sizeof(float));
    
    return point;
}
