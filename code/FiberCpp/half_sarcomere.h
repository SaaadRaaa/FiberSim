#pragma once

/**
 * @file    half_sarcomere.h
 * @brief   header file for the half_sarcomere class
 * @author  Ken Campbell
 */

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "global_definitions.h"
#include "transition.h"

#include "gsl_vector.h"
#include "gsl_spmatrix.h"
#include "gsl_rng.h"

class FiberSim_model;
class FiberSim_options;
class FiberSim_data;
class FiberSim_protocol;

class muscle;

class kinetic_scheme;

class thick_filament;
class thin_filament;

class half_sarcomere
{
public:
    // Variables

    FiberSim_model* p_fs_model;     /**< Pointer to the FiberSim_model */

    FiberSim_options* p_fs_options; /**< Pointer to the FiberSim_model */

    FiberSim_protocol* p_fs_protocol;
                                    /**< Pointer to a FiberSim_protocol */

    FiberSim_data* p_fs_data;       /**< Pointer to a FiberSim_data structure */

    muscle* p_parent_m;             /**< Pointer to the parent muscle */

    int hs_id;                      /**< integer identifier for the half-sarcomere */

    // Simulation variables
    int step_counter;               /**< integer that starts at 0 and advances each time
                                         implement_time_step is called */

    // Macroscopic state variables
    
    double time_s;                  /**< double holding hs time in s */

    double hs_length;               /**< double holding half-sarcomere length in nm */
    
    double hs_force;                /**< double holding half-sarcomere force */

    double f_activated;             /**< double holding the proportion of binding sites
                                         that are activated */

    double pCa;                     /**< double holding intracellular pCa */

    // Thick filament structure

    thick_filament* p_mf[MAX_NO_OF_THICK_FILAMENTS];
                                    /**< pointer to an array of thick_filament objects */
    
    int m_n;                        /**< integer defining the number of thick filaments
                                         in the half-sarcomere */

    int m_nodes_per_thick_filament; /**< integer defining the number of nodes (positions)
                                         on each thick filament */

    int m_cbs_per_thick_filament;   /**< integer defining the number of cbs
                                         on each thick filament */

    int m_cbs_per_node;             /**< integer defining the number of cbs
                                         per node */

    double m_inter_crown_rest_length;
                                    /**< double defining the rest-length of the spring
                                         between crowns in nm */

    double m_lambda;                /**< double defining the barezone length in nm */

    double m_mean_fil_length;       /**< double holding the mean m filament length */

    gsl_vector* m_y;                /**< gsl_vector holding y_coordinates of
                                         thick filaments */

    gsl_vector* m_z;                /**< gsl_vector holding z_coordinates of
                                         thick filaments */

    // Thin filament structure

    thin_filament* p_af[MAX_NO_OF_THIN_FILAMENTS];
                                    /**< pointer to an array of thin_filament objects */
    
    int a_n;                        /**< integer defining the number of thin filaments
                                         in the half-sarcomere */

    int a_regulatory_units_per_strand;
                                    /**< integer defining the number of regulatory
                                         units per strand */

    int a_bs_per_unit;              /**< integer defining the number of binding sites
                                         per unit */

    int a_strands_per_filament;     /**< integer defining the number of regulatory
                                         strands per filament */
    
    int a_nodes_per_thin_filament;  /**< integer defining the number of nodes (positions)
                                         on each thin filament */

    int a_bs_per_thin_filament;     /**< integer defining the number of binding sites
                                         on each thin filament */

    int a_bs_per_node;              /**< integer defining the number of binding sites
                                         per node */

    double a_inter_bs_rest_length;  /**< double defining the rest-length of the
                                         spring between binding-sites in nm */

    double a_mean_fil_length;       /**< double holding the mean a filament length */

    gsl_vector* a_y;                /**< gsl_vector holding y_coordinates of
                                         thin filaments */

    gsl_vector* a_z;                /**< gsl_vector holding z_coordinates of
                                         thin filaments */

    short int** nearest_actin_matrix;
                                    /**< short int matrix holding the indices for
                                         the a filaments surrounding each m filament */

    // Titin structure

    int t_attach_a_node;                /**< int defining the thin node at which titin
                                             attaches */

    int t_attach_m_node;                /**< int defining the thick node at which titin
                                             attaches */

    // Lattice calculations

    int hs_total_nodes;             /**< integer holding the total number of nodes
                                         in the hs lattice */

    gsl_spmatrix* k_matrix_sparse_triplet;
                                    /**< gsl_sparse_matrix in triplet format for
                                         k_matrix used in position calculations */

    gsl_vector* f_vector;           /**< gsl_vector for right-hand side of
                                         position calculations */

    gsl_vector* x_vector;           /**< gsl_vector holding node positions */

    gsl_vector* f0_vector;           /**< gsl_vector holding right-hand side of kx=f
                                          with-out cross-bridges or titin */

    gsl_vector* g_vector;           /**< gsl_vector holding right-hand side of kx=f
                                         for titin and cross-bridges that
                                         depends on x */

    gsl_vector* df_vector;          /**< gsl_vector holding right-hand side of kx=f
                                         for titin and cross-bridges that
                                         does not depend on x */
    
    gsl_vector* tri_d_vector;       /**< gsl_vector holding main diagonal of k_matrix
                                         for iterative solve */
    gsl_vector* tri_e_vector;       /**< gsl_vector above main diagonal of k_matrix
                                         for iterative solve */
    gsl_vector* tri_f_vector;       /**< gsl_vector above main diagonal of k_matrix
                                             for iterative solve */

    // State populations

    int a_no_of_bs_states;          /**< integer holding the number of bs states */
    gsl_vector* a_pops;             /**< gsl_vector holding a_bs state populations */

    int m_no_of_cb_states;          /**< integer holding the number of cb states */
    gsl_vector* m_pops;             /**< gsl_vector holding m_cb state populations */

    int c_no_of_pc_states;          /**< integer holding the number of MyBPC states */
    gsl_vector* c_pops;             /**< gsl_vector holding m_pc state_populations */

    // Model parameters
    
    // Actin
    double a_k_stiff;               /**< double defining stiffness of thin filament
                                         spring in N m^-1 */
    double a_k_on;
    double a_k_off;
    double a_k_coop;

    // Myosin
    double m_k_stiff;               /**< double defining stiffness of thick filament
                                         spring in N m^-1 */

    double m_k_cb;                  /**< double defining stiffness of cross-bridge
                                         link in N m^-1 */

    kinetic_scheme* p_m_scheme; 
                                    /**< pointer to a kinetic scheme for myosin */

    // Titin
    double t_k_stiff;               /**< double defining stiffness of titin filament
                                         spring in N m^-1 */

    double t_slack_length;          /**< double defining slack length of titin filament
                                         in nm */

    // Myosin binding protein C
    kinetic_scheme* p_c_scheme;     /**< pointer to a kinetic scheme for MyBPC */

    int c_no_of_pcs;                /**< integer defining the number of MyBPC per filament */

    double c_k_stiff;               /** double defining the stiffness of a mybpc link */

    // Random numbers
    gsl_rng* rand_generator;        /**< pointer to a random number generator */

    // Functions

     /**
     * Constructor
     * + initialized with:
         + pointer to a model
         + pointer to options
         + pointer to the parent muscle
         + an id number
     */
    half_sarcomere(
        FiberSim_model* set_p_fs_model,
        FiberSim_options* set_p_fs_options,
        FiberSim_protocol* set_p_fs_protocol,
        muscle* p_parent_m,
        int set_hs_id);

    /**
    * Destructor
    */
    ~half_sarcomere(void);

    /**
    * initialise_filament_y_z_coordinates(int m_n)
    * sets the m_y, m_z, a_y, a_z coordinates for the thick and thin filaments
    * and deduces the mirror filaments
    * param m_n an integer setting the number of thick filaments, must be a perfect square
    * @return void
    */
    void initialise_filament_y_z_coordinates(int m_n);

    /**
    * initialize_nearest_actin_matrix(void)
    * sets the 6 nearest_actins for each m_filament
    * @return void
    */
    void initialise_nearest_actin_matrix(void);

    /**
     * + writes a file in JSON format summarizing the half_sarcomere status
     *   + for each thick filament
     *     + cb_x
     *     + cb_angle
     *     + cb_state
     *     + cb_isoform
     *     + cb_bound_to_a_f
     *     + cb_bound_to_a_n
     *     + cb_nearest_a_f
     *     + cb_nearest_a_n
     */
    void write_hs_status_to_file(char output_file_string[]);

    /**
    * returns the node index for a given filament, bs/cb combination
    * @param char representing filament_type
            'm' for myosin, 'a' for actin, 'c' for MyBP-C
    * @param int for the filament_index, 0 to (m_n-1) for myosin,
    *       0 to (a_n-1) for actin
    * @param int for the cross-bridge or binding-site index
    * @return int holding the index number for the cross-bridge or binding site
     */
    int node_index(char filament_type, int filament_index, int n_index);

    /**
    * builds the bare version (without cross-bridges or titin) of the
    * f0_vector which is the right hand side of kx = f
    * @return void
    */
    void initialise_f0_vector(void);

    /**
    * Initialises tri-diagonal vectors which define the positions of
    * the binding sites actins in the absence of cross-bridges or titin
    * @return void
    */
    void initialize_tridiagonal_vectors(void);

    /**
    * Calculates the g-vector which is the part of f in kx = f
    * which is due to cross-bridges and titin and depends on x
    * @return void
    */
    void calculate_g_vector(gsl_vector* x_trial);

    /**
    * Calculates the g-vector which is the part of f in kx = f
    * which is due to cross-bridges and titin and does not depend on x
    * thus the part due titin rest length and cb extensions
    * @return void
    */
    void calculate_df_vector(gsl_vector* x_trial);

    /**
    * calculates the x vector holding positions for filament nodes
    * method based on an iterative solve worked out with Qiang Ye
    * and based (Ken thinks) on a preconditioned conjugate gradient method
    * @return int number of iterations in iterative solver
    */
    size_t calculate_x_positions(void);

    /**
    * void unpack_x_vector(void)
    * unpacks the x vector updating the bs_x and cb_x positions in the
    * p_af and p_mf objects
    * @return void
    */
    void unpack_x_vector(void);

    /**
    * void update_f0_vector(double delta_hsl)
    * updates the f0_vector
    * @param delta_hsl a double defining the change in half-sarcomere length
    * @return void
    */
    void update_f0_vector(double delta_hsl);

    /**
    * void implement_time_step(double time_step, double delta_hsl, double sim_mode, double pCa)
    * @param time_step double defining the simulation time_step in s
    * @param delta_pCa double defining the half-sarcomere length change in nm
    * @param sim_mode double defining the protocol mode
    * @param pCa double defining the pCa level
    * @return no_of_iterations size_t for calculate_x_positions
    */
    size_t implement_time_step(double time_step, double delta_hsl, double sim_mode, double pCa);

    /**
    * double calculate_force(void)
    * @return double holding the average force in a thick filament
    */
    double calculate_force(void);

    /**
    * uses Brent's root-finding method to get a delta_hsl that produces
    * the target force
    * @param target_force a double with the target force
    * @return a double with the delta_hsl required to get target_force
    */
    double calculate_delta_hsl_for_force(double target_force);

    static double test_force_wrapper(double delta_hsl, void* params);

    /**
    *
    */
    double test_force_for_delta_hsl(double delta_hsl, void *params);

    /**
    * void calculate_a_pops(void)
    * updates a vector holding the proportion of binding sites in each state
    * @return void
    */
    void calculate_a_pops(void);

    /**
    * void calculate_m_pops(void)
    * updates a vector holding the proportion of cross-bridges in each state
    * @return void
    */
    void calculate_m_pops(void);

    /**
    * void calculate_c_pops(void)
    * updates a vector holding the proportion of MyBPC in each state
    * @return void
    */
    void calculate_c_pops(void);


    /**
    * void calculate_mean_filament_lengths(void)
    * sets the mean_a_fil_length and mean_m_fil_length 
    * @return void
    */
    void calculate_mean_filament_lengths(void);

    /**
    * void set_cb_nearest_a_f(void)
    * loops through thick filaments setting the nearest a_f for each cb in p_mf
    * @return void
    */
    void set_cb_nearest_a_f(void);

    /**
    * void set_cb_nearest_a_n(void)
    * loops through thick filaments setting the nearest a_n for each cb in p_mf
    * @return void
    */
    void set_cb_nearest_a_n(void);

    /**
    * void set_pc_nearest_a_f(void)
    * loops through thick filaments setting the nearest a_f for each pc in p_mf
    * @return void
    */
    void set_pc_nearest_a_f(void);

    /**
    * void set_pc_nearest_a_n(void)
    * loops through thick filaments setting the nearest a_n for each pc in p_mf
    * @return void
    */
    void set_pc_nearest_a_n(void);


    /**
    * void thin_filament_kinetics(double time_step)
    * switches regulatory units on and off
    * @return void
    */
    void thin_filament_kinetics(double time_step, double Ca_conc);

    /**
    * void thick_filament_kinetics(double time_step)
    * handles both myosin abd MyBP-C transitions
    * @return void
    */
    void thick_filament_kinetics(double time_step);

    /**
    * void myosin_kinetics(double time_step)
    * handles myosin transitions
    * @return void
    */
    void myosin_kinetics(double time_step);

    /**
    * void myybpc_kinetics(double time_step)
    * handles mybpc transitions
    * @return void
    */
    void mybpc_kinetics(double time_step);

    /**
    * void handle_lattice_event(char mol_type, char event_type, int thick_f, int thick_n, int thin_f, int thin_n)
    * adjusts k_matrix and f_vector when myosin or mybp-c attach or detach between filaments
    * @param mol_type char 'm' for myosin, or 'c' for myosin binding protein-C
    * @param p_trans pointer to the transition
    * @param thick_f int index for the thick filament
    * @param thick_n int index for the thick head
    * @param thin_f int index for the thin filament
    * @param thin_n int index for the thin binding site
    * @return void
    */
    void handle_lattice_event(char mol_type, transition* p_trans, int thick_f, int thick_n, int thin_f, int thin_n);

    // Debugging functions
    
    /**
    * void write_sparse_matrix_to_file(gsl_spmatrix* p_matrix, char output_file_string[])
    * @return void
    */
    void write_gsl_spmatrix_to_file(gsl_spmatrix* p_matrix, char output_file_string[]);

    /**
    * void write_gsl_vector_to_file(gsl_vector* p_vector, char output_file_string[])
    * @return void
    */
    void write_gsl_vector_to_file(gsl_vector* p_vector, char output_file_string[]);
};