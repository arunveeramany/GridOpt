//
//  Net.h
//
//  Created by Guanglei Wang on 16/06/2014.
//

#ifndef Net_h
#define Net_h

#include <map>
#include <math.h>
#include "Bus.h"
#include "Gen.h"
#include "Line.h"
#include <gravity/Net.h>
#include <gravity/model.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

using namespace gravity;


/** @brief Seasons type */
typedef enum { summer_, winter_, spring_, autumn_} SeasonType;


/** @brief Wind Farm Data */
class WindData{
public:
    double _cap;
    double _cap_cost;
    double _fixed_cost;
    double _var_cost;
    unsigned _lifetime;
};

/** @brief PV Gens Data */
class PVData{
public:
    double _cap;
    double _cap_cost;
    double _fixed_cost;
    double _var_cost;
    unsigned _lifetime;
};

/** @brief A Month has a name, belongs to a given season and has stats on the number of week/weekends and peak days*/
class Month{
public:
    string _name;
    SeasonType _season;
    unsigned _nb_week_days = 0;
    unsigned _nb_weekend_days = 0;
    unsigned _nb_peak_days = 0;
    double _wind_average[24];
    double _solar_average[24];
    double _wind_variance[24];
    
    
    Month(string name, SeasonType season, unsigned nb_week_days, unsigned nb_weekend_days, unsigned nb_peak_days):_name(name), _season(season), _nb_week_days(nb_week_days), _nb_weekend_days(nb_weekend_days), _nb_peak_days(nb_peak_days){};
    void print() const{
        cout << _name << " : " << "in season " << _season << ", nb_week_days = " << _nb_week_days<< ", nb_weekend_days = " << _nb_weekend_days << ", nb_peak_days = " << _nb_peak_days << endl;
        cout << "Hourly Average Wind Data: ";
        for (unsigned t=0; t<24; t++) {
            cout << _wind_average[t];
            if (t<23) {
                cout << ", ";
            }
        }
        cout << ";" << endl;
        cout << "Hourly Variance in Wind Data: ";
        for (unsigned t=0; t<24; t++) {
            cout << _wind_variance[t];
            if (t<23) {
                cout << ", ";
            }
        }
        cout << ";" << endl;
    };
};


typedef enum { ACPOL, ACRECT, DISTF, CDISTF, LDISTF, QC, QC_SDP, OTS, DF, SOCP, SDP, DC, QC_OTS_L, QC_OTS_N, QC_OTS_O, SOCP_OTS, GRB_TEST } PowerModelType;
// A PowerNet extends a standard network by incorporating additional parameters related to power systems.
class PowerNet: public Net {

public:
    bool add_3d_nlin = true;
    string ref_bus;
    double bMVA; /**< Base MVA */
    double bV; /**< Base Voltage */
    double _power_factor = 1, PeakPVEfficiency = 0.15;
    double m_theta_lb = 0, m_theta_ub = 0; /**< BigM values for phase angles */
    double vmin = 0, vmax = 0; /**< Global values for voltage bounds */
    size_t nb_nodes = 0, nb_branches = 0, nb_gens = 0;
    param<Real> cb_f, cb_v;/**< Battery fixed nd variable costs */
    param<Real> inverter_capcost, gen_capcost, expansion_capcost, pv_capcost, pv_varcost;/**< Inverter, Generators, Expansion and PV capital costs */
    
    param<Real> pg_min, pg_max, qg_min, qg_max, pg_s, qg_s; /**< Upper and lower bounds on generation along with nominal values (default set points)*/
    param<Real> pb_min, pb_max, qb_min, qb_max; /**< Upper and lower bounds on battery generation */
    param<Real> pv_min, pv_max, qv_min, qv_max; /**< Upper and lower bounds on PV generation */    
    param<Real> pw_min, pw_max, qw_min, qw_max; /**< Upper and lower bounds on wind generation */
    param<Real> pv_out; /**< Normalized PV generation on bus */
    param<Real> c0, c1, c2; /**< Generation costs */
    param<Real> p_factor; /**< Participation factor for generators */
    param<Real> ramp_up, ramp_down; /**< Generation ramp up/down params */
    param<Real> gen_eff; /**< Diesel generation efficiency */
    param<int> min_ut, min_dt; /**< Minimum Uptime and Downtime for Generators */
    int max_ident_units; /**< Maximum number of identical units */
    param<int> min_diesel_invest, max_diesel_invest; /**< Minimum and Maximum number of Diesel generation investment at a given bus */
    param<int> min_batt_invest, max_batt_invest; /**< Minimum and Maximum number of Battery investment at a given bus */    
    param<Real> th_min, th_max, tan_th_min, tan_th_max, cphi, sphi, cos_d; /**< Upper and lower bounds on phase angles. tan is for the tangent of the angles */
    param<Real> v_min, v_max, v_s; /**< Voltage bounds and nominal values (default set points) */
    param<Real> v_diff_max; /**< Voltage bounds difference upper bound */
    param<Real> w_min, w_max; /**< Voltage bounds in lifted W space */
    param<Real> pl, ql, pl_ratio; /**< Load vectors */
    param<Real> gs, bs; /**< Transformer params */
    param<Real> eff_a, eff_b; /**< Efficiency params */
    unsigned _nb_eff_pieces = 1;/**< Number of pieces for the efficiency curves */
    unsigned _nb_conting = 0;/**< Number of contingencies for N-1 */
    indices _eff_pieces;/**< Set of indices for pieces for the efficiency curves */
    param<Real> tbound_max_tan, tbound_min_tan;  /** tan (th_min), tan(th_max) **/
    param<Real> r,x,g, b, ch, tr, as, S_max, wr_min, wr_max, wi_min, wi_max; /**< Power lines parameters, resp., impedance, line charging, and thermal limits. w params are for lifted variavles in W space */
    param<Real> g_ff, g_ft, g_tt, g_tf, b_ff, b_ft, b_tf, b_tt, Y, Y_t, Y_charge, Y_charge_t; /**< Transformers phase shifters parameters, e.g., g_ft = (-a->b*a->cc - a->g*a->dd)/(pow(a->cc,2)+pow(a->dd,2)) where a->cc = a->tr*cos(a->as) and a->dd = a->tr*sin(a->as);*/
    
    /** Set of all diesel generators */
    std::vector<Gen*> gens;
    
    /** Mapping the generator name to the generator pointer */
    std::map<std::string, Gen*> genMap;
    
    /** Set of all diesel generators in DEROPT */
    std::vector<DieselGen> _all_diesel_gens;
    
    /** Set of potential diesel generators */
    std::vector<Gen*> _potential_diesel_gens;
    
    /** Set of existing diesel generators */
    std::vector<Gen*> _existing_diesel_gens;
    
    /** Set of generators contingencies */
    std::vector<Gen*> _conting_gens;
    
    /** Set of line contingencies */
    std::vector<Arc*> _conting_lines;
    
    
    /** Set of all PV generators */
    std::vector<PV*> _all_PV_gens;
    
    /** Set of potential PV generators */
    std::vector<PV*> _potential_PV_gens;
    
    /** Set of existing PV generators */
    std::vector<PV*> _existing_PV_gens;
    
    /** Set of existing + potential diesel generators */
//    std::vector<DieselGen*> _diesel_gens;
    
    /** Set of existing + potential diesel generators */
    std::vector<WindGen*> _wind_gens;
    
    /** Set of all battery inverters */
    std::vector<BatteryInverter> _all_battery_inverters;
    
    /** Set of potential battery inverters */
    std::vector<BatteryInverter*> _potential_battery_inverters;
    
    /** Set of existing battery inverters */
    std::vector<BatteryInverter*> _existing_battery_inverters;
    
    /** Set of potential + existing battery inverters */
    std::vector<BatteryInverter*> _battery_inverters;
    
    
    /** Set of potential edge expansions */
    std::vector<Line*> _potential_expansion;
    
    /** Data on months */
    std::vector<Month> _months_data;
    
    /** Data on wind farm */
    WindData _wind_data;
    
    /** Data on PV gens */
    PVData _PV_data;

    
    /** Indices Sets */
    indices hours; /**< Hours */
    //    indices months = time("jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"); /**< Months */
    indices months = time("apr", "aug", "dec"); /**< Months */
    //    indices months = time("jan");
    indices typical_days = time("week","peak","weekend");
    //    indices typical_days = time("week");
    indices T, Nt, Nt_load, Nt_no_load, Et, Gt, exist_Gt, exist_Bt, exist_Et, pot_Et, pot_Gt, pot_Bt, Bt, Bt1, Gt1, Wt, PVt, PV_pot_t, pot_gen, pot_batt, pot_edges ;
    indices Et_opt, Gt_opt, Bt_opt, Bt1_opt, Wt_opt, PVt_opt;
    
    
    /** Investment Binary Variables */
    var<bool> w_g, w_b, w_e, w_pv;
    var<Real> Pv_cap; /**< Real variable indicating the extra capacity of PV to be installed */
    param<Real> Pv_cap_; /**< Real variable indicating the extra capacity of PV that has been installed */
    var<Real> Pg_; /**< Real variable indicating the power generation levels on committed generators */
    var<Real> Qg_; /**< Real variable indicating the power generation levels on committed generators */
    param<Real> v_base; /**< Voltage magnitude on buses */
    param<Real> theta_base; /**< Phase angle on buses */
    param<Real> p_from_base;/**< Params storing power flows in the base case */
    param<Real> p_to_base;/**< Params storing power flows in the base case */
    param<Real> q_from_base;/**< Params storing power flows in the base case */
    param<Real> q_to_base;/**< Params storing power flows in the base case */
    param<Real> Pg_base, Qg_base; /**< Real and Reactive params storing the power generation levels on committed generators in the base case */
    vector<param<Real>> Qg_cont;/**< Params storing the reactive power generation levels on committed generators in the contingency cases */
    vector<param<Real>> v_cont;/**< Params storing voltage magnitudes in the contingency cases */
    vector<param<Real>> theta_cont;/**< Params storing phase angles in the contingency cases */
    vector<param<Real>> p_from;/**< Params storing power flows in the contingency cases */
    vector<param<Real>> p_to;/**< Params storing power flows in the contingency cases */
    vector<param<Real>> q_from;/**< Params storing power flows in the contingency cases */
    vector<param<Real>> q_to;/**< Params storing power flows in the contingency cases */
    vector<param<Real>> _delta;/**< Params storing power generation delta for contingency cases */
    /** Constructors */
    PowerNet();
    ~PowerNet();
    
    /** Power grid data parser from Matpower*/
    int readgrid(const char* fname);
    
    /** Power grid data parser from GAMS*/
    int readGAMS(const string& fname);
    
    /** Power grid data parser from DERCAM*/
    int readDERCAM(const string& fname);
    
    /** Accessors */
    string get_ref_bus();
    unsigned get_nb_active_gens() const;
    unsigned get_nb_active_bus_pairs() const;
    unsigned get_nb_active_arcs() const;
    unsigned get_nb_active_nodes() const;
    void time_expand(unsigned T); /* < Time expansion of the grid parameters */
    void time_expand(const indices& T); /* < Time expansion of the grid parameters */
    void update_net();
    
    void save_base_case_sol(const string& fname);
    void save_all_sol(const string& fname);
    /** Power Models */
    
    vector<shared_ptr<Model>> conting_mods;/* < Contingency models */
    
    void update_status(unique_ptr<Model> model);
    
    void fix_investment();
    unique_ptr<Model> build_ACOPF(PowerModelType model=ACPOL, int output=0, double tol=1e-6);
    unique_ptr<Model> build_ACOPF_N_1(PowerModelType model=ACPOL, int output=0, double tol=1e-6);
    double solve_acopf(PowerModelType model=ACPOL, int output=0, double tol=1e-6);
    unique_ptr<Model> build_ROMDST(PowerModelType model=LDISTF, int output=5, double tol=1e-6, int nb_hours = 24);
    unique_ptr<Model> build_SCOPF(PowerModelType model=LDISTF, int output=5, double tol=1e-6);
    shared_ptr<Model> build_ROMDST_contingency(const string& name, PowerModelType model=LDISTF, int output=5, double tol=1e-6, int nb_hours = 24);
    shared_ptr<Model> build_SCOPF_gen_contingency(int cont, const string& name, PowerModelType model=ACPOL, int output=5, double tol=1e-6);
    shared_ptr<Model> build_SCOPF_line_contingency(int cont, const string& name, PowerModelType model=ACPOL, int output=5, double tol=1e-6);
    
};

#endif
