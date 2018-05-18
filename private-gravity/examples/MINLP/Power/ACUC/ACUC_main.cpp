//
//  ROMDST->cpp
//  Gravity
//
//  Created by Hassan Hijazi on April 10th 2018.
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

//int main_ACUC (int argc, char * argv[])
//{
//    string fname = "../../data_sets/Power/LANL_INPUT.xlsx", mtype = "ACPOL";
//    string solver_str="ipopt";
//    int output = 0;
//    bool relax = false, use_cplex = false, use_gurobi = false;
//    double tol = 1e-6;
//    string mehrotra = "no", log_level="0", nb_hours="1";
//
//    /** create a OptionParser with options */
//    op::OptionParser opt;
//    opt.add_option("h", "help", "shows option help"); // no default value means boolean options, which default value is false
//    opt.add_option("f", "file", "Input file name (def. ../data_sets/Power/nesta_case5_pjm.m)", fname );
//    opt.add_option("l", "log", "Log level (def. 0)", log_level );
//    opt.add_option("t", "time", "time in hours (def. 1)", nb_hours );
//    opt.add_option("m", "model", "power flow model: ACPOL/ACRECT/DISTFLOW/LINDISTFLOW (def. ACPOL)", mtype );
//    opt.add_option("s", "solver", "Solvers: ipopt/cplex/gurobi, default = ipopt", solver_str);
//
//    /** parse the options and verify that all went well. If not, errors and help will be shown */
//    bool correct_parsing = opt.parse_options(argc, argv);
//
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
//    auto max_nb_hours = op::str2int(opt["t"]);
//    bool has_help = op::str2bool(opt["h"]);
//    /** show help */
//    if(has_help) {
//        opt.show_help();
//        exit(0);
//    }
//    double solver_time_end, total_time_end, solve_time, total_time;
//    double total_time_start = get_wall_time();
//    PowerNet grid;
//    PowerModelType pmt = LDISTF;
//    if(mtype.compare("DISTF")==0) pmt = DISTF;
//    else if(mtype.compare("CDISTF")==0) pmt = CDISTF;
//    if (pmt==LDISTF) {
//        DebugOn("Using Linear Distflow model\n");
//    }
//    else if(pmt==DISTF) {
//        DebugOn("Using Nonconvex Distflow model\n");
//    }
//    else if(pmt==CDISTF) {
//        DebugOn("Using Convex Distflow model\n");
//    }
//
//    grid.readDERCAM(fname);
//
//    /* Grid Parameters */
//    auto bus_pairs = grid.get_bus_pairs();
//    auto Ngen = grid._diesel_gens.size();
//    auto Nbat = grid._battery_inverters.size();
//    auto Nline = grid.get_nb_active_arcs();
//    auto Nbus = grid.get_nb_active_nodes();
//    DebugOn("nb generators = " << Ngen << endl);
//    DebugOn("nb of potential generators = " << grid._potential_diesel_gens.size() << endl);
//    DebugOn("nb batteries = " << Nbat << endl);
//    DebugOn("nb of potential batteries = " << grid._potential_battery_inverters.size() << endl);
//    DebugOn("nb PV = " << grid._all_PV_gens.size() << endl);
//    DebugOn("nb of potential PV = " << grid._potential_PV_gens.size() << endl);
//    DebugOn("nb of existing PV = " << grid._existing_PV_gens.size() << endl);
//    DebugOn("nb installed lines = " << Nline << endl);
//    DebugOn("number of buses = " << Nbus << endl);
//
//    /** Indices Sets */
//    indices hours = time(1,max_nb_hours); /**< Hours */
//    //    indices months = time("jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"); /**< Months */
//    indices months = time("jan", "may", "aug"); /**< Months */
//    indices typical_days = time("week","peak","weekend");
//    auto T = indices(months,typical_days,hours);
//    double nT = T.size();
//    DebugOn("number of time periods = " << nT << endl);
//    grid.time_expand(T);
//    auto Nt = indices(grid.nodes,T);
//    auto Et = indices(grid.arcs,T);
//    auto Gt = indices(grid._diesel_gens,T);
//    auto exist_Gt = indices(grid._existing_diesel_gens,T);
//    auto exist_Bt = indices(grid._existing_battery_inverters,T);
//    auto pot_Gt = indices(grid._potential_diesel_gens,T);
//    auto pot_Bt = indices(grid._potential_battery_inverters,T);
//    auto Bt = indices(grid._battery_inverters,T);
//    auto Bt1 = indices(grid._battery_inverters,T.exclude("jan,week,1")); /**< Excluding first hour */
//    auto Gt1 = indices(grid._diesel_gens,T.exclude("jan,week,1")); /**< Excluding first hour */
//    auto Wt = indices(grid._wind_gens,T);
//    auto PVt = indices(grid._all_PV_gens,T);
//    auto PV_pot_t = indices(grid._potential_PV_gens,T);
//
//
//    /** MODEL DECLARATION */
//    Model ROMDST("ROMDST Model");
//
//    /** VARIABLES */
//
//    /* Unit Commitment of Gens */
//    //    var<Real> x("x",0,1); /**< On/Off status of generator */
//    //    var<Real> y("y",0,1); /**< Start-up status of generator */
//    //    var<Real> z("z",0,1); /**< Shut-down status of generator */
//    //    x._is_relaxed = true;
//    //    y._is_relaxed = true;
//    //    z._is_relaxed = true;
//    var<bool> x("x"); /**< On/Off status of generator */
//    var<bool> y("y"); /**< Start-up status of generator */
//    var<bool> z("z"); /**< Shut-down status of generator */
//    ROMDST->add(x.in(Gt), y.in(Gt), z.in(Gt));
//    x.initialize_all(1);
//    y.initialize_all(1);
//    z.initialize_all(1);
//
//
//    /* Investment binaries */
//
//    var<Real> Pv_cap("Pv_cap",pos_); /**< Real variable indicating the extra capacity of PV to be installed on bus b */
//    auto pot_pv = indices(grid._potential_PV_gens);
//    ROMDST->add(Pv_cap.in(pot_pv));
//    //    var<Real> w_g("w_g",0,1); /**< Binary variable indicating if generator g is built on bus */
//    //    var<Real> w_b("w_b",0,1); /**< Binary variable indicating if battery b is built on bus */
//    //    var<Real> w_e("w_e",0,1); /**< Binary variable indicating if expansion is selected for edge e */
//    //    w_g._is_relaxed = true;
//    //    w_b._is_relaxed = true;
//    //    w_e._is_relaxed = true;
//    var<bool> w_g("w_g"); /**< Binary variable indicating if generator g is built on bus */
//    var<bool> w_b("w_b"); /**< Binary variable indicating if battery b is built on bus */
//    var<bool> w_e("w_e"); /**< Binary variable indicating if expansion is selected for edge e */
//    var<bool> w_pv("w_pv"); /**< Binary variable indicating if PV is installed on bus b */
//
//    auto pot_gen = indices(grid._potential_diesel_gens);
//    auto pot_batt = indices(grid._potential_battery_inverters);
//    auto pot_edges = indices(grid._potential_expansion);
//
//    ROMDST->add(w_g.in(pot_gen),w_b.in(pot_batt),w_e.in(pot_edges),w_pv.in(pot_pv));
//    w_g.initialize_all(1);
//    w_b.initialize_all(1);
//    w_e.initialize_all(1);
//    w_pv.initialize_all(1);
//
//
//
//    /* Diesel power generation variables */
//    var<Real> Pg("Pg", grid.pg_min, grid.pg_max);
//    var<Real> Qg ("Qg", grid.qg_min, grid.qg_max);
//    var<Real> Pg2("Pg2", pos_);/**< Square of Pg */
//    ROMDST->add(Pg.in(Gt),Qg.in(Gt),Pg2.in(Gt));
//    //    Pg.initialize_all(0.1);
//
//
//    /* Battery power generation variables */
//    var<Real> Pb("Pb", grid.pb_min, grid.pb_max);
//    var<Real> Qb ("Qb", grid.qb_min, grid.qb_max);
//    var<Real> Pb_("Pb_", grid.pb_min, grid.pb_max);/**< Active power generation before losses */
//    var<Real> Qb_("Qb_", grid.qb_min, grid.qb_max);/**< Reactive power generation before losses */
//    ROMDST->add(Pb.in(Bt), Qb.in(Bt), Pb_.in(Bt),Qb_.in(Bt));
//
//
//    /* PV power generation variables */
//    var<Real> Pv("Pv", pos_);
//    ROMDST->add(Pv.in(PVt));
//
//    /* Battery state of charge variables */
//    var<Real> Sc("Sc", grid.pb_min, grid.pb_max);
//    ROMDST->add(Sc.in(Bt));
//
//    /* Wind power generation variables */
//    var<Real> Pw("Pw", grid.pw_min, grid.pw_max);
//    //    grid.pw_max.print(true);
//    ROMDST->add(Pw.in(Wt));
//
//    /* Power flow variables */
//    var<Real> Pij("Pfrom", grid.S_max);
//    var<Real> Qij("Qfrom", grid.S_max);
//    var<Real> Pji("Pto", grid.S_max);
//    var<Real> Qji("Qto", grid.S_max);
//
//    ROMDST->add(Pij.in(Et),Qij.in(Et));
//    if (pmt!=LDISTF) {
//        ROMDST->add(Pji.in(Et),Qji.in(Et));
//    }
//
//    /** Voltage magnitude (squared) variables */
//    var<Real> v("v", grid.vmin, grid.vmax);
//    ROMDST->add(v.in(Nt));
//    v.initialize_all(1.0);
//
//    /** Power loss variables */
//    var<Real> loss("loss", pos_);
//    if (pmt==DISTF || pmt==CDISTF) {
//        ROMDST->add(loss.in(Nt));
//    }
//
//    /** OBJECTIVE FUNCTION */
//    //    func_ obj = product(grid.c1.in(Gt), Pg) + product(grid.c2.in(Gt), Pg2) + product(grid.c0.in(Gt), y) + product(grid.c0.in(Gt), z);
//    func_ obj = product(grid.c1, Pg) + product(grid.c2, Pg2) + product(grid.c0, y.in(Gt)) + product(grid.c0, z.in(Gt));
//    obj += product(grid.gen_capcost, w_g.in(pot_gen));
//    obj += product(grid.inverter_capcost, w_b.in(pot_batt));
//    obj += product(grid.expansion_capcost, w_e.in(pot_edges));
//    obj += product(grid.pv_capcost, w_pv.in(pot_pv));
//    obj += product(grid.pv_varcost, Pv_cap.in(pot_pv));
//    //    func_ obj;
//    //    for (auto i = 0; i< Gt.size();i++) {
//    //        obj+= grid.c1.eval(i)*Pg(Gt._indices->at(i));
//    //        obj+= grid.c2.eval(i)*Pg2(Gt._indices->at(i));
//    //        obj+= grid.c0.eval(i)*(y(Gt._indices->at(i))+ z(Gt._indices->at(i)));
//    //    }
//    //    for (auto i = 0; i< pot_gen.size();i++) {
//    //        obj+= grid.gen_capcost.eval(i)*w_g(pot_gen._indices->at(i));
//    //    }
//    //    for (auto i = 0; i< pot_batt.size();i++) {
//    //        obj+= grid.inverter_capcost.eval(i)*w_b(pot_batt._indices->at(i));
//    //    }
//    //    for (auto i = 0; i< pot_edges.size();i++) {
//    //        obj+= grid.expansion_capcost.eval(i)*w_e(pot_edges._indices->at(i));
//    //    }
//    ROMDST->min(obj);
//
//
//    /** CONSTRAINTS **/
//
//    /** FLOW CONSERVATION **/
//
//    /** KCL P and Q */
//    Constraint KCL_P("KCL_P");
//    Constraint KCL_Q("KCL_Q");
//    if (pmt==LDISTF) {
//        KCL_P  = sum(Pij.out_arcs()) - sum(Pij.in_arcs()) + grid.pl - sum(Pg.in_gens()) - sum(Pb.in_bats()) - sum(Pw.in_wind()) - sum(Pv.in_pv());
//        KCL_Q  = sum(Qij.out_arcs()) - sum(Qij.in_arcs()) + (sqrt(1-grid._power_factor*grid._power_factor)/grid._power_factor)*grid.pl - sum(Qg.in_gens()) - sum(Qb.in_bats());
//    }
//    else{
//        KCL_P  = sum(Pij.out_arcs()) + sum(Pji.in_arcs()) + grid.pl - sum(Pg.in_gens())- sum(Pb.in_bats()) - sum(Pw.in_wind()) - sum(Pv.in_pv());
//        KCL_Q  = sum(Qij.out_arcs()) + sum(Qji.in_arcs()) + (sqrt(1-grid._power_factor*grid._power_factor)/grid._power_factor)*grid.pl - sum(Qg.in_gens()) - sum(Qb.in_bats());
//    }
//    ROMDST->add(KCL_P.in(grid.nodes, T) == 0);
//    ROMDST->add(KCL_Q.in(grid.nodes, T) == 0);
//
//    /**  THERMAL LIMITS **/
//
//    /*  Thermal Limit Constraints */
//    Constraint Thermal_Limit_from("Thermal_Limit_from");
//    Thermal_Limit_from += power(Pij, 2) + power(Qij, 2);
//    Thermal_Limit_from -= power(grid.S_max, 2);
//    ROMDST->add(Thermal_Limit_from.in(grid._exist_arcs, T) <= 0);
//
//    if (pmt!=LDISTF) {
//        Constraint Thermal_Limit_to("Thermal_Limit_to");
//        Thermal_Limit_to += power(Pji, 2) + power(Qji, 2);
//        Thermal_Limit_to -= power(grid.S_max,2);
//        ROMDST->add(Thermal_Limit_to.in(grid._exist_arcs, T) <= 0);
//    }
//
//    /*  Thermal Limit Constraints for expansion edges */
//    Constraint Thermal_Limit_from_exp("Thermal_Limit_From_Exp");
//    Thermal_Limit_from_exp += power(Pij, 2) + power(Qij, 2);
//    Thermal_Limit_from_exp -= power(w_e,2)*power(grid.S_max, 2);
//    ROMDST->add(Thermal_Limit_from_exp.in(grid._potential_expansion, T) <= 0);
//
//
//    if (pmt!=LDISTF) {
//        /*  Thermal Limit Constraints for expansion edges */
//        Constraint Thermal_Limit_to_exp("Thermal_Limit_To_Exp");
//        Thermal_Limit_to_exp += power(Pji, 2) + power(Qji, 2);
//        Thermal_Limit_to_exp -= power(w_e,2)*power(grid.S_max, 2);
//        ROMDST->add(Thermal_Limit_to_exp.in(grid._potential_expansion, T) <= 0);
//    }
//
//
//    /**  POWER FLOW **/
//
//    /** Power Flow Constraints */
//    Constraint Flow_P_From("Flow_P_From");
//    Flow_P_From = v.from() - 2*(grid.r*Pij + grid.x*Qij) - v.to();
//    if (pmt!=LDISTF) {
//        Flow_P_From += loss;
//        Constraint Flow_P_To("Flow_P_To");
//        Flow_P_To = v.to() - 2*(grid.r*Pji + grid.x*Qji) - v.from() + loss;
//        ROMDST->add(Flow_P_To.in(grid._exist_arcs, T)==0);
//    }
//    ROMDST->add(Flow_P_From.in(grid._exist_arcs, T)==0);
//
//    Constraint Flow_P_Expansion_L("Flow_P_Expansion_L");
//    Flow_P_Expansion_L = v.from() - 2*(grid.r*Pij + grid.x*Qij) - v.to() - (1-w_e)*(grid.vmax - grid.vmin);
//    ROMDST->add(Flow_P_Expansion_L.in(grid._potential_expansion, T)<=0);
//
//    Constraint Flow_P_Expansion_U("Flow_P_Expansion_U");
//    Flow_P_Expansion_U = v.from() - 2*(grid.r*Pij + grid.x*Qij) - v.to() - (1-w_e)*(grid.vmin - grid.vmax);
//    ROMDST->add(Flow_P_Expansion_U.in(grid._potential_expansion, T)>=0);
//
//
//    /**  PV **/
//
//    /*  On/Off on Potential PV */
//    Constraint OnOffPV("OnOffPV");
//    OnOffPV += Pv - Pv_cap*grid.pv_out;
//    ROMDST->add(OnOffPV.in(PV_pot_t) <= 0);
//
//
//    /**  BATTERIES **/
//
//    /*  Apparent Power Limit on Potential Batteries */
//    Constraint Apparent_Limit_Batt_Pot("Apparent_Limit_Batt_Potential");
//    Apparent_Limit_Batt_Pot += power(Pb, 2) + power(Qb, 2);
//    Apparent_Limit_Batt_Pot -= power(w_b,2)*power(grid.pb_max, 2);
//    ROMDST->add(Apparent_Limit_Batt_Pot.in(pot_Bt) <= 0);
//
//    /*  Apparent Power Limit on Existing Batteries */
//    Constraint Apparent_Limit_Batt("Apparent_Limit_Batt_Existing");
//    Apparent_Limit_Batt += power(Pb, 2) + power(Qb, 2);
//    Apparent_Limit_Batt -= power(grid.pb_max, 2);
//    ROMDST->add(Apparent_Limit_Batt.in(exist_Bt) <= 0);
//
//
//    /*  State Of Charge */
//    Constraint State_Of_Charge("State_Of_Charge");
//    State_Of_Charge = Sc - Sc.prev() + Pb;
//    ROMDST->add(State_Of_Charge.in(Bt1) == 0);
//
//    /*  Efficiencies */
//    Constraint EfficiencyExist("BatteryEfficiencyExisting");
//    EfficiencyExist += Pb  - grid.eff_a*Pb_ - grid.eff_b;//TODO without time extending eff_a and eff_b
//    ROMDST->add(EfficiencyExist.in(indices(grid._eff_pieces,exist_Bt)) <= 0);
//
//    Constraint EfficiencyPot("BatteryEfficiencyPotential");
//    EfficiencyPot += Pb  - grid.eff_a*Pb_ - grid.eff_b*w_b;
//    ROMDST->add(EfficiencyPot.in(indices(grid._eff_pieces,pot_Bt)) <= 0);
//
//    /**  UNIT COMMITMENT **/
//
//    /*  On/Off status */
//    Constraint OnOff_maxP("OnOff_maxP");
//    OnOff_maxP += Pg - grid.pg_max*x;
//    ROMDST->add(OnOff_maxP.in(Gt) <= 0);
//
//    Constraint Perspective_OnOff("Perspective_OnOff");
//    Perspective_OnOff += power(Pg,2) - Pg2*x;
//    ROMDST->add(Perspective_OnOff.in(Gt) <= 0);
//
//    Constraint OnOff_maxQ("OnOff_maxQ");
//    OnOff_maxQ += Qg - grid.qg_max*x;
//    ROMDST->add(OnOff_maxQ.in(Gt) <= 0);
//
//    Constraint OnOff_build("OnOff_build");
//    OnOff_build += x - w_g;
//    ROMDST->add(OnOff_build.in(pot_Gt) <= 0);
//
//    Constraint OnOffStartup0("OnOffStartup0");
//    OnOffStartup0 += x - y;
//    ROMDST->add(OnOffStartup0.in(indices(grid._diesel_gens,T.start())) == 0);
//
//    //    Constraint OnOffStartupN("OnOffStartupN");
//    //    OnOffStartupN += y + z;
//    //    ROMDST->add(OnOffStartupN.in(indices(grid._diesel_gens,T.end())) == 0);
//
//    Constraint OnOffStartupShutdown("OnOffStartupShutdown");
//    OnOffStartupShutdown += x - x.prev() - y + z;
//    ROMDST->add(OnOffStartupShutdown.in(Gt1) == 0);
//
//    Constraint StartupShutdown("StartupShutdown");
//    StartupShutdown += y + z;
//    ROMDST->add(StartupShutdown.in(Gt) <= 1);
//
//
//    Constraint MinUpTime("MinUpTime");
//    MinUpTime += sum(x.min_time()) - grid.min_ut*y;
//    ROMDST->add(MinUpTime.in(grid._diesel_gens, T, grid.min_ut) >= 0);
//
//    Constraint MinDownTime("MinDownTime");
//    MinDownTime += sum(x.min_time()) - grid.min_dt*(1-z);
//    ROMDST->add(MinDownTime.in(grid._diesel_gens, T, grid.min_dt) <= 0);
//
//    Constraint RampDown("RampDown");
//    RampDown += Pg.prev() - Pg  - grid.pg_max*z - grid.pg_max*grid.ramp_down;
//    ROMDST->add(RampDown.in(Gt1) <= 0);
//
//    Constraint RampUp("RampUp");
//    RampUp += Pg - Pg.prev()  - grid.pg_max*y - grid.pg_max*grid.ramp_up;
//    ROMDST->add(RampUp.in(Gt1) <= 0);
//
//    for (auto n:grid.nodes) {
//        auto b = (Bus*)n;
//        for (auto i = 0; i < b->_pot_gen.size(); i++) {
//            auto gen = b->_pot_gen[i];
//            if(grid.min_diesel_invest.eval(gen->_name)==grid.max_diesel_invest.eval(gen->_name)){
//                Constraint FixedDieselInvest("FixedDieselInvest"+gen->_name);
//                FixedDieselInvest += w_g(gen->_name);
//                ROMDST->add(FixedDieselInvest == 1);
//                for (auto j = i+1; j < b->_pot_gen.size(); j++) {
//                    auto gen2 = b->_pot_gen[j];
//                    if (gen2->_gen_type==gen->_gen_type) {
//                        Constraint FixedDieselInvest("FixedDieselInvest"+gen2->_name);
//                        FixedDieselInvest += w_g(gen2->_name);
//                        ROMDST->add(FixedDieselInvest == 1);
//                    }
//                }
//            }
//            else {
//                Constraint MinDieselInvest("MinDieselInvest_"+b->_name+"_DG"+to_string(gen->_gen_type));
//                MinDieselInvest += w_g(gen->_name);
//                for (auto j = i+1; j < b->_pot_gen.size(); j++) {
//                    auto gen2 = b->_pot_gen[j];
//                    if (gen2->_gen_type==gen->_gen_type) {
//                        MinDieselInvest += w_g(gen2->_name);
//                    }
//                }
//                auto rhs = grid.min_diesel_invest.eval(gen->_name);
//                if (rhs>0) {
//                    ROMDST->add(MinDieselInvest >= rhs);
//                }
//            }
//        }
//        for (auto i = 0; i < b->_pot_bat.size(); i++) {
//            auto bat = b->_pot_bat[i];
//            if(grid.min_batt_invest.eval(bat->_name)==grid.max_batt_invest.eval(bat->_name)){
//                Constraint FixedBattInvest("FixedBattInvest"+bat->_name);
//                FixedBattInvest += w_b(bat->_name);
//                ROMDST->add(FixedBattInvest == 1);
//                for (auto j = i+1; j < b->_pot_bat.size(); j++) {
//                    auto bat2 = b->_pot_bat[j];
//                    if (bat2->_bat_type==bat->_bat_type) {
//                        Constraint FixedBattInvest("FixedBattInvest"+bat2->_name);
//                        FixedBattInvest += w_b(bat2->_name);
//                        ROMDST->add(FixedBattInvest == 1);
//                    }
//                }
//            }
//            else {
//                Constraint MinBattInvest("MinBattInvest_"+b->_name+"_DG"+to_string(bat->_bat_type));
//                MinBattInvest += w_b(bat->_name);
//                for (auto j = i+1; j < b->_pot_bat.size(); j++) {
//                    auto bat2 = b->_pot_bat[j];
//                    if (bat2->_bat_type==bat->_bat_type) {
//                        MinBattInvest += w_b(bat2->_name);
//                    }
//                }
//                auto rhs = grid.min_batt_invest.eval(bat->_name);
//                if (rhs>0) {
//                    ROMDST->add(MinBattInvest >= rhs);
//                }
//            }
//        }
//    }
//
//
//
//
//    //    ROMDST->print_expanded();
//
//    /* SOLVE */
//    double lb = 0, ub = 0;
//    int return_status = -1;
//    auto solver_time_start = get_wall_time();
//    if (use_cplex) {
//        solver ACUC(ROMDST,cplex);
//        return_status = ACUC.run(output, relax = false, tol = 1e-6);
//        if (return_status==100) {
//            lb = ROMDST->_obj_val;
//            ub = ROMDST->_obj_val;
//        }
//    }
//    else if (use_gurobi) {
//        solver ACUC(ROMDST,gurobi);
//        return_status = ACUC.run(output, relax = false, tol = 1e-6);
//        if (return_status==100) {
//            lb = ROMDST->_obj_val;
//            ub = ROMDST->_obj_val;
//        }
//    }
//    else {
//        solver ACUC(ROMDST,ipopt);
//        return_status = ACUC.run(output, relax = false, tol = 1e-6);
//        //        ROMDST->print_solution();
//        if (return_status != 100) {
//            clog << "Cannot compute lower bound!\n";
//        }
//        else {
//            lb = ROMDST->_obj_val;
//            DebugOn("Lower Bound = " << lb << endl);
//            auto old_obj = ROMDST->_obj;
//            ROMDST->add_round_solution_obj();
//            return_status = ACUC.run(output, relax = false,     tol = 1e-6);
//            if (return_status != 100) {
//                clog << "Cannot compute lower bound!\n";
//            }
//            else {
//                //                ROMDST->print_solution();
//                ROMDST->round_solution();
//                if(ROMDST->is_feasible(tol=1e-4)){
//                    clog << "Feasible integer solution found!\n";
//                }
//                else{
//                    clog << "Current integer solution is infeasible!\n";
//                    return_status = -1;
//                }
//                ub = old_obj.eval();
//            }
//        }
//    }
//    solver_time_end = get_wall_time();
//    total_time_end = get_wall_time();
//    solve_time = solver_time_end - solver_time_start;
//    total_time = total_time_end - total_time_start;
//    if (return_status==100) {
//        ROMDST->print_solution();
//    }
//    DebugOn("Lower Bound = " << lb << endl);
//    DebugOn("Upper Bound = " << ub << endl);
//    DebugOn("Optimality gap = " << 100*(ub-lb)/ub << "%" << endl);
//    DebugOn("Solve time = " << solve_time << endl);
//    DebugOn("Total time = " << total_time << endl);
//    string out = "DATA_OPF, " + grid._name + ", " + to_string(Nbus) + ", " + to_string(Nline) +", " + to_string(ub) + ", " + to_string(lb) + ", " + to_string(solve_time);
//    if(return_status==100){
//        if(ROMDST->is_convex()){
//            out += ", GlobalOpimal, ";
//        }
//        else{
//            out += ", LocalOptimal, ";
//        }
//    }
//    else {
//        out += ", NoSolution, ";
//    }
//    out += to_string(total_time);
//
//    DebugOn(out <<endl);
//    return 0;
//}

int main (int argc, char * argv[])
{
    string fname = "../../data_sets/Power/LANL_INPUT.xlsx", mtype = "ACPOL";
    string solver_str="ipopt";
    int output = 0;
    bool relax = false, use_cplex = false, use_gurobi = false;
    double tol = 1e-6;
    string mehrotra = "no", log_level="0", nb_hours="1";
    
    /** create a OptionParser with options */
    op::OptionParser opt;
    opt.add_option("h", "help", "shows option help"); // no default value means boolean options, which default value is false
    opt.add_option("f", "file", "Input file name (def. ../data_sets/Power/nesta_case5_pjm.m)", fname );
    opt.add_option("l", "log", "Log level (def. 0)", log_level );
    opt.add_option("t", "time", "time in hours (def. 1)", nb_hours );
    opt.add_option("m", "model", "power flow model: ACPOL/ACRECT/DISTFLOW/LINDISTFLOW (def. ACPOL)", mtype );
    opt.add_option("s", "solver", "Solvers: ipopt/cplex/gurobi, default = ipopt", solver_str);
    
    /** parse the options and verify that all went well. If not, errors and help will be shown */
    bool correct_parsing = opt.parse_options(argc, argv);
    
    if(!correct_parsing){
        return EXIT_FAILURE;
    }
    
    fname = opt["f"];
    mtype = opt["m"];
    solver_str = opt["s"];
    if (solver_str.compare("gurobi")==0) {
        use_gurobi = true;
    }
    else if(solver_str.compare("cplex")==0) {
        use_cplex = true;
    }
    output = op::str2int(opt["l"]);
    auto max_nb_hours = op::str2int(opt["t"]);
    bool has_help = op::str2bool(opt["h"]);
    /** show help */
    if(has_help) {
        opt.show_help();
        exit(0);
    }
    double solver_time_end, total_time_end, solve_time, cont_solve_start, cont_solve_end, cont_solve_time, model_build_time = -1, total_time = -1;
    double total_time_start = get_wall_time();
    PowerNet grid;
    PowerModelType pmt = LDISTF;
    if(mtype.compare("DISTF")==0) pmt = DISTF;
    else if(mtype.compare("CDISTF")==0) pmt = CDISTF;
    if (pmt==LDISTF) {
        DebugOn("Using Linear Distflow model\n");
    }
    else if(pmt==DISTF) {
        DebugOn("Using Nonconvex Distflow model\n");
    }
    else if(pmt==CDISTF) {
        DebugOn("Using Convex Distflow model\n");
    }
    
//    Model m("mtest");
//    var<> x("x", pos_);
//    var<> y("y", pos_);
//    auto myids = indices("bus1");
//    m.add(x.in(myids), y.in(myids));
//    m.min(power(y,2) + y);
//    Constraint lbc("lbc");
//    lbc += y - x;
//    m.add(lbc >= 0);
//    Constraint lbx("lbx");
//    lbx += x;
//    m.add(lbx >= 3);
//    m.print_expanded();
//    auto s = solver(m,ipopt);
//    s.run(5, relax = false, tol = 1e-6);
//    m.print_solution();
//    s.run(5, relax = false, tol = 1e-6);
//    return 0;
    
    auto stat = grid.readDERCAM(fname);
    if (stat==-1) {
        cerr << "Error reading Excel File, Exising" << endl;
        return -1;
    }
    
    /* Grid Parameters */
    auto bus_pairs = grid.get_bus_pairs();
    auto Ngen = grid.gens.size();
    auto Nbat = grid._battery_inverters.size();
    auto Nline = grid.get_nb_active_arcs();
    auto Nbus = grid.get_nb_active_nodes();
    DebugOn("nb generators = " << Ngen << endl);
    DebugOn("nb of potential generators = " << grid._potential_diesel_gens.size() << endl);
    DebugOn("nb batteries = " << Nbat << endl);
    DebugOn("nb of potential batteries = " << grid._potential_battery_inverters.size() << endl);
    DebugOn("nb PV = " << grid._all_PV_gens.size() << endl);
    DebugOn("nb of potential PV = " << grid._potential_PV_gens.size() << endl);
    DebugOn("nb of existing PV = " << grid._existing_PV_gens.size() << endl);
    DebugOn("nb installed lines = " << Nline << endl);
    DebugOn("number of buses = " << Nbus << endl);
    
    auto ROMDST = grid.build_ROMDST(pmt,output,tol,max_nb_hours);
    
    
    
//        ROMDST->print_expanded();
    //    return 0;
    
    /* SOLVE */
    double lb = 0, ub = 0;
    int return_status = -1;
    auto solver_time_start = get_wall_time();
    if (use_cplex) {
        solver ACUC(*ROMDST,cplex);
        return_status = ACUC.run(output, relax = true, tol = 1e-6, 0.01);
        if (return_status==100) {
            lb = ROMDST->_obj_val;
            ub = ROMDST->_obj_val;
        }
    }
    else if (use_gurobi) {
        solver ACUC(*ROMDST,gurobi);
        return_status = ACUC.run(output, relax = false, tol = 1e-6, 0.01);
        if (return_status==100) {
            lb = ROMDST->_obj_val;
            ub = ROMDST->_obj_val;
        }
    }
    else {
        solver ACUC(*ROMDST,ipopt);
        return_status = ACUC.run(output, relax = false, tol = 1e-6);
        //        ROMDST->print_solution();
        if (return_status != 100) {
            clog << "Cannot compute lower bound!\n";
        }
        else {
            lb = ROMDST->_obj_val;
            DebugOn("Lower Bound = " << lb << endl);
            ROMDST->print_solution();
//            DebugOn("Rounding current solution..." << endl);
//            ROMDST->round_solution();
//            if(ROMDST->is_feasible(tol=1e-5)){
            if(false){
                clog << "Feasible integer solution found!\n";
                ub = lb;
                return_status = 100;
            }
            else{
                clog << "Rounded integer solution is not feasible!\n Trying to enforce integrality in the objective\n";
                auto old_obj = ROMDST->_obj;
                ROMDST->add_round_solution_obj();
                //                ROMDST->add_integrality();
                //                ROMDST->add_round_solution_cuts();
//                ROMDST->_first_run = true;
//                solver ACUC_CPX(*ROMDST,ipopt);
//                return_status = ACUC_CPX.run(output, relax = false, tol = 1e-6);
                return_status = ACUC.run(output, relax = false, tol = 1e-6);
                if (return_status != 100) {
                    clog << "Cannot compute lower bound!\n";
                }
                else {
                    //                ROMDST->print_solution();
                    ROMDST->round_solution();
                    if(ROMDST->is_feasible(tol=1e-4)){
                        clog << "Feasible integer solution found!\n";
                        ub = old_obj.eval();
                    }
                    else{
                        clog << "Current integer solution is infeasible!\n Trying to solve with pure integer objective\n";
                        ROMDST->add_round_solution_obj(false);
                        //                        ROMDST->_first_run = true;
                        //                        solver ACUC_CPX(*ROMDST,cplex);
                        return_status = ACUC.run(output, relax = false, tol = 1e-6);
                        if (return_status != 100) {
                            clog << "Cannot compute lower bound!\n";
                        }
                        else {
                            ROMDST->round_solution();
                            if(ROMDST->is_feasible(tol=1e-4)){
                                clog << "Feasible integer solution found!\n";
                                ub = old_obj.eval();
                            }
                            else{
                                return_status = -1;
                                clog << "Current integer solution is infeasible!\n";
                            }
                        }
                    }
                }
            }
        }
    }
    solver_time_end = get_wall_time();
    solve_time = solver_time_end - solver_time_start;
    DebugOn("Lower Bound = " << lb << endl);
    DebugOn("Upper Bound = " << ub << endl);
    DebugOn("Optimality gap = " << 100*(ub-lb)/ub << "%" << endl);
    DebugOn("Solve time = " << solve_time << endl);
    DebugOn("Total time = " << total_time << endl);
    string out = "DATA_ROMDST, lb = " + to_string(lb) + ", ub = " + to_string(ub) + ", solve_time = " + to_string(solve_time);
    if(return_status==100){
        if(ROMDST->is_convex() && (ub-lb)/ub < 1e-4){
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
//        return 0;
    auto nb_threads = 3;
    double start_built_time = get_wall_time();
    cont_solve_start = get_wall_time();
    vector<shared_ptr<Model>> conting_mods;
    if(return_status==100){
        bool all_feasible = false, added_all = false;
        unsigned nb_added = 0, nb_total = 0, nb_exist = grid._existing_diesel_gens.size(), nb_new = 0;
        grid.fix_investment();
        auto vals = grid.w_g.get_vals();
        clog << "Building models for new generators." << endl;
        for (auto i = 0; i<vals->size(); i++) {
            if (vals->at(i)) {//This generator was built
                clog << "Deactivating Newly Built Generator: " << grid._potential_diesel_gens[i]->_name << endl;
                grid._potential_diesel_gens[i]->_active = false;
                if (i>0) {
                    grid._potential_diesel_gens[i-1]->_active = true;
                }
                auto contingency_model = grid.build_ROMDST_contingency(grid._potential_diesel_gens[i]->_name);
//                contingency_model->print_expanded();
                conting_mods.push_back(contingency_model);
                nb_new++;
            }
        }
        
        clog << "Building models for existing generators." << endl;
        for (auto i = 0; i<nb_exist; i++) {
            clog << "Deactivating Existing Generator: " << grid._existing_diesel_gens[i]->_name << endl;
            grid._existing_diesel_gens[i]->_active = false;
            if (i>0) {
                grid._existing_diesel_gens[i-1]->_active = true;
            }
            auto contingency_model = grid.build_ROMDST_contingency(grid._existing_diesel_gens[i]->_name);
//            contingency_model->print_expanded();
//            return 0;
            conting_mods.push_back(contingency_model);
        }
        nb_total = nb_new + nb_exist;
        double end_build_time = get_wall_time();
        model_build_time = end_build_time - start_built_time;
        DebugOn("Total time for building continency models = " << model_build_time << endl);
//        return 0;
        while (!all_feasible && !added_all) {
            all_feasible = true;
            nb_total = 0;
            clog << "Running Feasibility Models in Parallel" << endl;
            run_parallel(conting_mods,ipopt,tol=1e-6,nb_threads=5);
            clog << "Checking N-1 Scenarios" << endl;
            /* Check Feasibility */
            for (auto &mod:conting_mods) {
                auto pshed_max = (var<>*)mod->get_var("P_shed_max");
                auto pshed = (var<>*)mod->get_var("P_shed");
                
//                DebugOn(pshed->to_str(true) << endl);
//                DebugOn(pshed_max->to_str(true) << endl);
//                auto pb = (var<>*)mod->get_var("Pb");
//                DebugOn(pb->to_str(true) << endl);
//                auto pb_ = (var<>*)mod->get_var("Pb_");
//                DebugOn(pb_->to_str(true) << endl);
//                auto sc = (var<>*)mod->get_var("Sc");
//                DebugOn(sc->to_str(true) << endl);
//                mod->get_constraint("BatteryEfficiency")->print_expanded();
//                mod->get_constraint("KCL_P")->print_expanded();
//                mod->get_constraint("KCL_Q")->print_expanded();
//                mod->get_constraint("State_Of_Charge")->print_expanded();
                
                if (mod->_status!=100) {
                    clog << "ERROR WHEN SOLVING N-1 CONTINGENCY WITH GEN:" << mod->get_name() << endl;
                    return -1;
                }
                
                auto max_shed = pshed_max->eval()*100;
                if (max_shed>2) {
                    auto Pshed_cstr = mod->get_constraint("max_load_shed");
                    for (auto inst = 0; inst<Pshed_cstr->_nb_instances; inst++) {
                        if (pshed->eval(inst)>1e-4 && Pshed_cstr->is_active(inst,1e-5)) {
                            DebugOn("Max load shed active constraint: ");
                            //                        Pshed_cstr->print(inst);
                            DebugOn("P_shed[" << pshed->get_rev_indices()->at(inst) << "] = " << pshed->eval(inst) << endl);
                        }
                    }
                    clog << "VIOLATED N-1 CONTINGENCY WITH GEN:" << mod->get_name() << endl;
                    clog << "Percentage of load shed = " << max_shed << "%" << endl;
//                    auto pshed = (var<>*)mod->get_var("P_shed");
//                    DebugOn(pshed->to_str(true) << endl);
//                    auto Pshed_cstr = mod->get_constraint("max_load_shed");
//                    for (auto inst = 0; inst<Pshed_cstr->get_nb_instances(); inst++) {
//                        if (Pshed_cstr->is_active(inst)) {
//                            DebugOn("Max load shed active constraint: " << Pshed_cstr->to_str(inst) << endl);
//                        }
//                    }
                    return_status = -1;
//                    return -1;
                    /* Add cuts */
                }
                else{
                    clog << "N-1 CONTINGENCY SATISFIED FOR GEN:" << mod->get_name() << endl;
                }
            }
            
            added_all = (nb_total==nb_added);
        }

    }
    cont_solve_end = get_wall_time();
    cont_solve_time = cont_solve_end - cont_solve_start;
    out = "DATA_ROMDST, lb = " + to_string(lb) + ", ub = " + to_string(ub) + ", gap = " + to_string(100*(ub-lb)/ub) + "%, base case solve_time = " + to_string(solve_time) + ", contingency cases solve_time = " + to_string(cont_solve_time) + " (inlcuding model build time = " + to_string(model_build_time) + ") ";
    if(return_status==100){
        clog << "Current Solution Satisfies N-1 Constraints." << endl;
        if(ROMDST->is_convex() && (ub-lb)/ub < 1e-4){
            out += ", GlobalOpimal, ";
        }
        else{
            out += ", LocalOptimal, ";
        }
        ROMDST->print_solution();
    }
    else {
        clog << "WARNING: Current Solution Violates N-1 Constraints." << endl;
        out += ", NoSolution, ";
    }
    total_time_end = get_wall_time();
    total_time = total_time_end - total_time_start;
    out += "total_time = " + to_string(total_time);
    DebugOn(out <<endl);
    return 0;
}
