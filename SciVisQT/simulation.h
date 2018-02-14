#ifndef SIMULATION_H
#define SIMULATION_H

#include <rfftw.h>

class simulation
{
public:
    simulation();
    void FFT();
    void solve();
    void diffuse_matter();
    void set_forces();
    void do_one_simulation_step();

private:
    const int DIM = 50;				//size of simulation grid
    double dt = 0.4;				//simulation time step
    float visc = 0.001;				//fluid viscosity
    fftw_real *vx, *vy;             //(vx,vy)   = velocity field at the current moment
    fftw_real *vx0, *vy0;           //(vx0,vy0) = velocity field at the previous moment
    fftw_real *fx, *fy;	            //(fx,fy)   = user-controlled simulation forces, steered with the mouse
    fftw_real *rho, *rho0;			//smoke density at the current (rho) and previous (rho0) moment
    rfftwnd_plan plan_rc, plan_cr;  //simulation domain discretization
};

#endif // SIMULATION_H
