#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <fstream>

#include "grid.h"
//#include "imaging.h"

#define E_DATA      0
#define E_MAT       1   // permitivitty
#define E_ENV      2   // conductivity
#define E_CONTRIB   3
//#define E_CURRENT   3


#define H_DATA      0
#define H_MAT       1   // permeability
#define H_ENV      2
#define H_CONTRIB   3

using namespace std;

typedef struct {
    unit acc;           // accumulates the maximum value over the analysis range
    int analysis_step;  // which iteration : [0, iterations-1] holds
    int boundr;         // the spatial bound where the environment changes (since the wave propagates from left to right)
} analysis_maximum;

class Simulation
{
private:
    int size;
    unit wave_len, res_s, res_t;
    Grid<unit> E, H;

    unit current_time;
    int frame;

    // refraction
    bool compute_reflection;
    analysis_maximum e_incoming, e_reflected;



    // I/O
    void copy_material(vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env); // loads material/environment data
    void copy_frame(ostream& buffer);   // outputs frame

public:
    Simulation(int size, unit wave_len, vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env); // no reflection analysis
    Simulation(int size, unit wave_len, vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env, int border, int incoming_analysis, int reflected_analysis);

    void boundary();
    void update_e();
    void update_h();
    void update_current();
    void update_max(int iteration);

    void load_contributions();
    void unload_contributions();



    void reset();
    void step();
    void run(int iterations);
    void run(int iterations, string output_prefix); // overloading for outputting frames in dat files

    unit computed_reflection();
};

#endif // SIMULATION_H
