#include <math.h>
#include "fluid_simulation.h"

extern const unsigned int WindowSize;
extern const unsigned int Iter;
extern const double dt;

#define N WindowSize

void SetBoundary(int b, float *x)
{
	// Manage top and bottom wall
	for (unsigned int i = 1; i < N - 1; i++) {
		x[i + 0] = b == 2 ? -x[i + 1*N] : x[i + 1*N];
		x[i + (N-1)*N] = b == 2 ? -x[i + (N-2)*N] : x[i + (N-2)*N];
	}
	// Manage left and right
	for (unsigned int j = 1; j < N -1; j++) {
		x[0 + j*N]   = b == 1 ? -x[1  + j*N]  : x[1  + j*N];
		x[N-1 + j*N] = b == 1 ? -x[N-2 + j*N] : x[N-2+ j*N];
	}
    
	x[0 + 0]         = 0.5f * (x[1 + 0]         + x[0 + 1*N]       + x[0 + 0*N]      );
	x[0 + (N-1)*N]   = 0.5f * (x[1 + (N-1)*N]   + x[0 + (N-2)*N]   + x[0 + (N-1)*N]  );
	x[N-1 + 0*N]     = 0.5f * (x[N-2 + 0*N]     + x[N-1 + 1*N]     + x[N-1 + 0*N]    );
	x[N-1 + (N-1)*N] = 0.5f * (x[N-2 + (N-1)*N] + x[N-1 + (N-2)*N] + x[N-1 + (N-1)*N]);
}

/**
 * Update state based on nieighbours
 * b -> wall we need to check later
 * x -> previous state
 * x0 -> current state
 */
void LinSolve(int b, float *x, float *x0, float a, float c)
{
	float cRecip = 1.0 / c;
	// Matrix revesre (as if...)
	for (unsigned int k = 0; k < Iter; k++) {
		for (unsigned int j = 1; j < N - 1; j++) {
			for (unsigned int i = 1; i < N - 1; i++) {
				x[i + j*N] = (x0[i + j*N] + a*( x[i+1 + j*N] + x[i-1 + j*N] + x[i + (j+1)*N] + x[i + (j-1)*N])) * cRecip;
			}
		}
		SetBoundary(b, x);
	}
}

/**
 * Update previous states [based on neighbour nodes]
 * b  -> direction in which we calculate
 * x  -> previous state
 * x0 -> current state
 * a  -> diffiusion rate
 */
void Diffuse (int b, float *x, float *x0, float diff)
{
    float a = dt * diff * (N - 2) * (N - 2);
    LinSolve(b, x, x0, a, 1 + 4 * a);
}

/**
 * Forces mass conserving
 * velocX -> previous velocity
 * velocY -> previous velocity
 * p      -> current velocity
 * div    -> current velocity
 */
void Project(float *velocX, float *velocY, float *p, float *div)
{
	for (unsigned int j = 1; j < N - 1; j++) {
		for (unsigned int i = 1; i < N - 1; i++) {
			div[i + j*N] = -0.5f*(
			         velocX[i+1 + j*N] - velocX[i-1 + j*N]
			        +velocY[i + (j+1)*N] - velocY[i + (j-1)*N]
			    )/N;
			p[i + j*N] = 0;
		}
	}
    
    SetBoundary(0, div); // handle wall direction 
   	SetBoundary(0, p);   // handle wall direction
    LinSolve(0, p, div, 1, 6);
    
	for (unsigned int j = 1; j < N - 1; j++) {
		for (unsigned int i = 1; i < N - 1; i++) {
			velocX[i + j*N] -= 0.5f * (p[i+1 + j*N] - p[i-1 + j*N]) * N;
			velocY[i + j*N] -= 0.5f * (p[i + (j+1)*N] - p[i + (j-1)*N]) * N;
		}
	}

    SetBoundary(1, velocX); // handle wall direction
    SetBoundary(2, velocY); // handle wall direction
}

void Advection(int b, float *d, float *d0,  float *velocX, float *velocY)
{
    float i0, i1, j0, j1;
    
    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);
    
    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;
    
    float Nfloat = N;
    float ifloat, jfloat;
	unsigned int i, j;    

	// Linear interpolation
	for(j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
		for( i = 1, ifloat = 1; i < N - 1; i++, ifloat++) { 
			tmp1 = dtx * velocX[i + j*N];
			tmp2 = dty * velocY[i + j*N];
			x    = ifloat - tmp1;
			y    = jfloat - tmp2;
                
			if(x < 0.5f) x = 0.5f; 
			if(x > Nfloat + 0.5f) x = Nfloat + 0.5f; 
			i0 = floorf(x); 
			i1 = i0 + 1.0f;
			if(y < 0.5f) y = 0.5f; 
			if(y > Nfloat + 0.5f) y = Nfloat + 0.5f; 
			j0 = floorf(y);
			j1 = j0 + 1.0f; 
			
			s1 = x - i0; 
			s0 = 1.0f - s1; 
			t1 = y - j0; 
			t0 = 1.0f - t1;
			
			int i0i = (int)i0;
			int i1i = (int)i1;
			int j0i = (int)j0;
			int j1i = (int)j1;
			
			d[i + j*N] =	
				s0 * ( t0 * d0[i0i + j0i*N] + t1 * d0[i0i + j1i*N])
			   +s1 * ( t0 * d0[i1i + j0i*N] + t1 * d0[i1i + j1i*N]);
        }
    }
    SetBoundary(b, d);
}
