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
typedef struct FluidPoint FluidPoint;

/* Free allocated memory */
void FluidPointFree(FluidPoint *point);

/* Works like constructor */
FluidPoint* FluidPointCreate(int size, int diffusion, int viscosity, float dt);
