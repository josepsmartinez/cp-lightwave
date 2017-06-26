#include "simulation.h"




Simulation::Simulation(int size, unit wave_len, vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env) :
    size(size), wave_len(wave_len),
    res_s(wave_len/50.0), res_t(0.9f*res_s),
    E(size+2, 4, res_t, res_s), H(size+1, 4, res_t, res_s),
    compute_reflection(false)

{

    copy_material(e_mat, h_mat, e_env, h_env);


    reset();

}

Simulation::Simulation(int size, unit wave_len, vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env, int border, int incoming_analysis, int reflected_analysis) :
    size(size), wave_len(wave_len),
    res_s(wave_len/50.0), res_t(0.9f*res_s),
    E(size+2, 4, res_t, res_s), H(size+1, 4, res_t, res_s),
    compute_reflection(true),
    e_incoming({.acc=0.f, .analysis_step=incoming_analysis, .boundr=border-1}), e_reflected({.acc=0.f, .analysis_step=reflected_analysis, .boundr=border-1})

{
    copy_material(e_mat, h_mat, e_env, h_env);


    reset();

    //e_incoming.acc = 0.f; e_incoming.analysis_step = incoming_analysis;  e_incoming.boundr = border-1;
    //e_reflected.acc = 0.f; e_ref.analysis_step = reflected_analysis; e_ref.boundr = border-1;

}



void Simulation::reset()
{
    current_time = 0.f;
    frame = 0;
    e_incoming.acc = 0.f; e_reflected.acc = 0.f;
}


void Simulation::update_e()
{
    for(int i=1; i < size; i++) {
        E.inc(i+1, E_DATA, res_t*0.5f*-(H.read(i, H_DATA) - H.read(i+1, H_DATA)) / (res_s * E.read(i+1, E_ENV)) );
    }
}


void Simulation::update_h()
{
    H.inc(0, H_DATA, res_t*E.read(1, E_DATA) / (res_s * H.read(0, H_ENV)));
    for(int i=1; i < size; i++) {
        H.inc(i, H_DATA, res_t*-(E.read(i, E_DATA) - E.read(i+1, E_DATA)) / (res_s * H.read(i, H_ENV)) );
    }
    H.inc(size, H_DATA, res_t*-E.read(size, E_DATA) / (res_s * H.read(size, H_ENV)));
}

void Simulation::update_current()
{
    // simple approach: wave function on single static source point
    int i_source = 20 * wave_len / res_s; // 1000

    unit value = sin(2 * M_PI * current_time) * exp((current_time-30.0)*(30.0-current_time)/100.0);
    //printf("\n %f \t %f\n", value, current_time);

    E.inc(i_source, E_DATA, -value/E.read(i_source, E_ENV));

}

void Simulation::update_max(int iteration)
{
 int i;

 //printf("Bounds: %d\t%d\t%d\t%d\n", e_inc.boundl, e_inc.boundr, e_ref.boundl, e_ref.boundr);

 if(iteration == e_incoming.analysis_step) for(i=0; i<=e_incoming.boundr; i++)
     e_incoming.acc = max(e_incoming.acc, ABS(E.read(i, E_DATA)));


 if(iteration == e_reflected.analysis_step) for(i=0; i<=e_reflected.boundr; i++)
     e_reflected.acc = max(e_reflected.acc, ABS(E.read(i, E_DATA)));
}

void Simulation::boundary()
{
    E.write(0,     E_DATA, 0.f);
    E.write(size+1,  E_DATA, 0.f);
}



void Simulation::load_contributions()
{
    int i;
    for(i=0; i<size+2; i++)
        E.write(i, E_CONTRIB, -E.read(i, E_DATA)*E.read(i, E_MAT)/E.read(i, E_ENV));
        //E.write(i, E_CONTRIB, -E.read(i, E_DATA)*1/1);
    for(i=0; i<size+1; i++)
        H.write(i, H_CONTRIB, -H.read(i, H_DATA)*H.read(i, H_MAT)/H.read(i, H_ENV));
        //H.write(i, H_CONTRIB, -H.read(i, H_DATA)*1/1);
}

void Simulation::unload_contributions()
{
    int i;
    for(i=0; i<size+2; i++)
        E.inc(i, E_DATA, E.read(i, E_CONTRIB));
    for(i=0; i<size+1; i++)
        H.inc(i, H_DATA, E.read(i, H_CONTRIB));
}


void Simulation::step()
{
    boundary();

    //load_contributions();

    update_e();
    update_h();
    update_e();

    load_contributions();
    unload_contributions();
    update_current();

    current_time += res_t;


}





void Simulation::run(int iterations)
{
    int i;

    reset();
    for(i=0; i < iterations; i++){
        step();
        if (compute_reflection) update_max(i);

    }

    printf("Computed Reflection: %f, from %f (incoming) and %f (reflected)\n", computed_reflection(), e_incoming.acc, e_reflected.acc);
}

void Simulation::run(int iterations, string output_prefix)
{
    char filename[32];
    ofstream datfile;
    int i;

    reset();
    for(i=0, frame=0; i<iterations; i++,frame++){
        step();


        if (frame==2500 || frame==5000){ // hardcoding for saving requested frames only
            sprintf (filename, "../lightwave/frames/%s_%04d.dat", output_prefix.c_str(), frame);
            datfile = ofstream(filename, fstream::out);

            copy_frame(datfile);


            datfile.close();
        }

    }
}

unit Simulation::computed_reflection()
{
    return (e_reflected.acc*e_reflected.acc) / (e_incoming.acc*e_incoming.acc);
}

void Simulation::copy_material(vector<unit> e_mat, vector<unit> h_mat, vector<unit> e_env, vector<unit> h_env)
{
    int i;

    // copies spatial and material configuration
    for(i=0; i<size+2; i++) E.write(i, E_ENV, e_env[i]);
    for(i=0; i<size+2; i++) E.write(i, E_MAT, e_mat[i]);
    for(i=0; i<size+1; i++) H.write(i, H_ENV, h_env[i]);
    for(i=0; i<size+1; i++) H.write(i, H_MAT, h_mat[i]);
}

void Simulation::copy_frame(ostream &buffer)
{
    string sep = "  ";

    // copies E field into buffer
    int x;
    unit y;

    // data block
    for(x=0; x < size+2; x++){
        y = E.read(x, E_DATA);
        buffer << sep << x << sep << y;
        buffer << endl;
    }
    buffer << endl;

    // mat block
    /*
    for(x=0; x < size+2; x++){
        y = E.read(x, E_MAT);
        if(y!=0) {
            buffer << sep << x << sep << y;
            buffer << endl;
        }
    }
    buffer << endl;

    // env block
    for(x=0; x < size+2; x++){
        y = E.read(x, E_ENV);
        if(y!=0) {
            buffer << sep << x << sep << y;
            buffer << endl;
        }
    }
    */
    buffer << endl;
}
