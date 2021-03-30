#pragma once

/** 
 * @file    FiberSim_options.h
 * @brief   header file for FiberSim_options
 * @author  Ken Campbell
 */

// Definitions for JSON parsing
#ifndef _RAPIDJSON_DOCUMENT
#define _RAPIDJSON_DOCUMENT
#include "rapidjson/document.h"
#endif

#include "stdio.h"

class FiberSim_options
{
public:
    /**
     * Constructor
     */
    FiberSim_options(char JSON_options_file_string[]);

    /**
     * Destructor
     */
    ~FiberSim_options(void);

    // Variables
   
    double max_rate;                    /**< double defining the maximum rate allowed in calculations */

    double x_pos_rel_tol;               /**< double defining the relative tolerance for calculating
                                             x positions */
   
    int dump_precision;                 /**< integer defining the precision of doubles dumped in the half-
                                             sarcomere status files */

    char log_relative_to[_MAX_PATH];    /**< char array used to direct paths */

    char log_folder[_MAX_PATH];         /**< Folder to hold files about the program status:
                                             intended for debugging and testing purposes
                                            "none" means do not write files */

    char log_file_string[_MAX_PATH];    /**< Log file holding information about the program execution
                                             intended mainly for debugging and testing purposes */

    FILE* log_file;                     /**< Pointer to a log file */

    int log_mode;                       /**< Integer defining whether or not to dump program status
                                             0 means do not log
                                             1 means log
                                             Value is set by the presence of log_folder */

    char status_relative_to[_MAX_PATH]; /**< char array used to direct paths */

    char status_folder[_MAX_PATH];      /**< Folder to hold status files
                                             "none" means do not write files */

    char time_steps_string[_MAX_PATH];  /**< String definning which time_steps to dump */

    // Functions

    /**
     * a function that sets FiberSim_options parameters from a JSON file
     * @param json_file_string the filename for the JSON file
     */
    void set_FiberSim_options_from_JSON_file_string(char JSON_file_string[]);


    /**
     * a function that writes model options to file
     */
    void write_FiberSim_options_to_file(void);
};