//
//  Gravity
//
//  Created by Hassan Hijazi on May 10th 2018.
//
//
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include "../PowerNet.h"
#include <gravity/solver.h>
#include <stdio.h>
#include <stdlib.h>
#include <optionParser.hpp>

using namespace std;
using namespace gravity;


int main (int argc, char * argv[])
{
    
//    string fname = "../../data_sets/Power/Phase_0_IEEE14_1Scenario/scenario_1/pscopf14.m", mtype = "ACRECT";
    string fname = "Phase_0_IEEE14_1Scenario/scenario_1/pscopf_data.gms", mtype = "ACPOL";
    string solver_str="ipopt";
    int output = 0;
    bool relax = false, use_cplex = false, use_gurobi = false;
    double tol = 1e-6;
    string mehrotra = "no", log_level="5";
    
    /** create a OptionParser with options */
//    op::OptionParser opt;
//    opt.add_option("h", "help", "shows option help"); // no default value means boolean options, which default value is false
//    opt.add_option("f", "file", "Input file name (def. ../../data_sets/Power/Phase_0_IEEE14_1Scenario/scenario_1/pscopf_data.gms)", fname );
//    opt.add_option("l", "log", "Log level (def. 0)", log_level );
//    opt.add_option("m", "model", "power flow model: ACPOL/ACRECT (def. ACRECT)", mtype );
//    opt.add_option("s", "solver", "Solvers: ipopt/cplex/gurobi, default = ipopt", solver_str);
    
    /** parse the options and verify that all went well. If not, errors and help will be shown */
//    bool correct_parsing = opt.parse_options(argc, argv);
    
//    if(!correct_parsing){
//        return EXIT_FAILURE;
//    }
//    
//    fname = opt["f"];
//    mtype = opt["m"];
//    solver_str = opt["s"];
//    if (solver_str.compare("gurobi")==0) {
//        use_gurobi = true;
//    }
//    else if(solver_str.compare("cplex")==0) {
//        use_cplex = true;
//    }
//    output = op::str2int(opt["l"]);
//    bool has_help = op::str2bool(opt["h"]);
//    /** show help */
//    if(has_help) {
//        opt.show_help();
//        exit(0);
//    }
    double solver_time_end, total_time_end, solve_time, cont_solve_start, cont_solve_end, cont_solve_time, model_build_time = -1, total_time = -1;
    double total_time_start = get_wall_time();
    PowerNet grid;
    PowerModelType pmt = ACPOL;
    if(mtype.compare("ACPOL")==0) pmt = ACPOL;
    else if(mtype.compare("ACRECT")==0) pmt = ACRECT;
    if (pmt==ACPOL) {
        DebugOn("Using Polar Formulation\n");
    }
    else if(pmt==ACRECT) {
        DebugOn("Using Rectangular Formulation\n");
    }
    
    auto stat = grid.readGAMS(fname);
//    auto stat = grid.readgrid(fname.c_str());
    if (stat==-1) {
        cerr << "Error reading input file, exiting" << endl;
        return -1;
    }
    
    /* Grid Parameters */
    auto bus_pairs = grid.get_bus_pairs();
    auto Ngen = grid.gens.size();
    auto Nline = grid.get_nb_active_arcs();
    auto Nbus = grid.get_nb_active_nodes();
    DebugOn("nb generators = " << Ngen << endl);
    DebugOn("nb installed lines = " << Nline << endl);
    DebugOn("nb buses = " << Nbus << endl);
    
    auto ACOPF = grid.build_ACOPF_N_1(pmt,output,tol);
//    ACOPF->print_expanded();
    /* SOLVE */
    double base_case_obj = 0, ub = 0;
    int return_status = -1;
    auto solver_time_start = get_wall_time();

    solver ACOPF_SOLVER(*ACOPF,ipopt);
    return_status = ACOPF_SOLVER.run(output, relax = false, tol = 1e-6, 1e-6, "ma27");
    if (return_status != 100) {
        clog << "Cannot compute base case!\n";
    }
    else {
        base_case_obj = ACOPF->_obj_val;
        DebugOn("Base Case Objective = " << base_case_obj << endl);
        ACOPF->print_solution(false);
        grid.save_base_case_sol("solution1.txt");
        grid.save_all_sol("solution2.txt");
//        grid.Pg_.gravity::param<double>::print(true);
//        grid.Qg_.gravity::param<double>::print(true);
    }
    solver_time_end = get_wall_time();
    solve_time = solver_time_end - solver_time_start;
    total_time_end = get_wall_time();
    total_time = total_time_end - total_time_start;
    DebugOn("Solve time = " << solve_time << endl);
    DebugOn("Total time = " << total_time << endl);
    string out = "DATA_SCOPF, base_case = " + to_string(base_case_obj) + ", solve_time = " + to_string(solve_time);
    if(return_status==100){
        if(ACOPF->is_convex()){
            out += ", GlobalOpimal, ";
        }
        else{
            out += ", LocalOptimal, ";
        }
    }
    else {
        out += ", NoSolution, ";
    }
    out += to_string(total_time);
    DebugOn(out <<endl);
    
    return 0;
    auto nb_threads = 3;
    double start_built_time = get_wall_time();
    cont_solve_start = get_wall_time();
    if(return_status==100){
        bool all_feasible = false, added_all = false;
        unsigned nb_added = 0, nb_total = 0, nb_gens_cont = grid._conting_gens.size(), nb_lines_cont = grid._conting_lines.size();
        clog << "Building contingency models for lines." << endl;
        auto cont = 0;
        for (auto i = 0; i<nb_lines_cont; i++) {
            clog << "Switching Off Line: " << grid._conting_lines[i]->_name << endl;
            grid._conting_lines[i]->_active = false;
            if (i>0) {
                grid._conting_lines[i-1]->_active = true;
            }
            auto contingency_model = grid.build_SCOPF_line_contingency(cont++,grid._conting_lines[i]->_name);
            contingency_model->print_expanded();
            grid.conting_mods.push_back(contingency_model);
        }
        if (nb_gens_cont>0) {
            clog << "Building contingency models for generators." << endl;
        }
        else {
            clog << "No generator contingencies." << endl;
        }
        for (auto i = 0; i<nb_gens_cont; i++) {
            clog << "Deactivating Generator: " << grid._conting_gens[i]->_name << endl;
            grid._conting_gens[i]->_active = false;
            if (i>0) {
                grid._conting_gens[i-1]->_active = true;
            }
            auto contingency_model = grid.build_SCOPF_gen_contingency(cont++,grid._conting_gens[i]->_name);
            grid.conting_mods.push_back(contingency_model);
        }
        nb_total = nb_gens_cont + nb_lines_cont;
        double end_build_time = get_wall_time();
        model_build_time = end_build_time - start_built_time;
        DebugOn("Total time for building continency models = " << model_build_time << endl);
        while (!all_feasible && !added_all) {
            all_feasible = true;
            nb_total = 0;
            clog << "Running Feasibility Models in Parallel" << endl;
            run_parallel(grid.conting_mods,ipopt,tol=1e-12,nb_threads=3);
            clog << "Checking N-1 Scenarios" << endl;
            /* Check Feasibility */
            for (auto &mod:grid.conting_mods) {
                if (mod->_status!=100) {
                    clog << "ERROR WHEN SOLVING N-1 CONTINGENCY WITH :" << mod->get_name() << endl;
                    return -1;
                }
//                
//                auto max_shed = pshed_max->eval()*100;
//                if (max_shed>2) {
//                    auto Pshed_cstr = mod->get_constraint("max_load_shed");
//                    for (auto inst = 0; inst<Pshed_cstr->_nb_instances; inst++) {
//                        if (pshed->eval(inst)>1e-4 && Pshed_cstr->is_active(inst,1e-5)) {
//                            DebugOn("Max load shed active constraint: ");
//                            //                        Pshed_cstr->print(inst);
//                            DebugOn("P_shed[" << pshed->get_rev_indices()->at(inst) << "] = " << pshed->eval(inst) << endl);
//                        }
//                    }
//                    clog << "VIOLATED N-1 CONTINGENCY WITH:" << mod->get_name() << endl;
//                    clog << "Percentage of load shed = " << max_shed << "%" << endl;
//                    return_status = -1;
                    /* Add cuts */
//                }
//                else{
//                    clog << "N-1 CONTINGENCY SATISFIED FOR GEN:" << mod->get_name() << endl;
//                }
            }
            
            added_all = (nb_total==nb_added);
        }
        
    }
    cont_solve_end = get_wall_time();
    cont_solve_time = cont_solve_end - cont_solve_start;
    out = "DATA_SCOPF, base case obj = " + to_string(base_case_obj)+ ", base case solve_time = " + to_string(solve_time) + ", contingency cases solve_time = " + to_string(cont_solve_time) + " (inlcuding model build time = " + to_string(model_build_time) + ") ";
    if(return_status==100){
        clog << "Current Solution Satisfies N-1 Constraints." << endl;
        if(ACOPF->is_convex()){
            out += ", GlobalOpimal, ";
        }
        else{
            out += ", LocalOptimal, ";
        }
        ACOPF->print_solution();
    }
    else {
        clog << "WARNING: Current Solution Violates N-1 Constraints." << endl;
        out += ", NoSolution, ";
    }
    total_time_end = get_wall_time();
    total_time = total_time_end - total_time_start;
    out += "total_time = " + to_string(total_time);
    DebugOn(out <<endl);
    grid.save_all_sol("solution2.txt");
    return 0;
}
