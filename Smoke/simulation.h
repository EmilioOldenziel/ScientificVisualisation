#ifndef SIMULATION_H
#define SIMULATION_H

#include <rfftw.h>
#include <math.h>

class Simulation
{
public:
    Simulation();
    ~Simulation();
    void init_simulation(int n);
    void FFT(int direction,void* vx);
    int clamp(float x);
    float max(float x, float y);
    void solve(int n, fftw_real* vx, fftw_real* vy, fftw_real* vx0, fftw_real* vy0, fftw_real visc, fftw_real dt);
    void diffuse_matter(int n, fftw_real *vx, fftw_real *vy, fftw_real *rho, fftw_real *rho0, fftw_real dt);
    void set_forces(int DIM);
    void do_one_simulation_step();
    void drag(int mx , int my, int DIM, int winWidth ,int winHeight);

    fftw_real* get_rho() const;
    fftw_real* get_fy() const;
    fftw_real* get_fx() const;
    fftw_real* get_rho0() const;
    fftw_real* get_vx() const;
    fftw_real* get_vy() const;
    fftw_real* get_vx0() const;
    fftw_real* get_vy0() const;

    int dim;

    double get_dt();
    float get_visc();

private:
    int n;

    double dt = 0.4;				//simulation time step
    float visc = 0.001;				//fluid viscosity

    fftw_real *vx, *vy;             //(vx,vy)   = velocity field at the current moment
    fftw_real *vx0, *vy0;           //(vx0,vy0) = velocity field at the previous moment
    fftw_real *fx, *fy;	            //(fx,fy)   = user-controlled simulation forces, steered with the mouse
    fftw_real *rho, *rho0;			//smoke density at the current (rho) and previous (rho0) moment
    rfftwnd_plan plan_rc, plan_cr;  //simulation domain discretization
};

#endif // SIMULATION_H