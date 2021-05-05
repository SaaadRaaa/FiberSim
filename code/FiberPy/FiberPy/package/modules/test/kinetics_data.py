# -*- coding: utf-8 -*-
"""
Created on Tue Apr 27 17:11:49 2021

@author: sako231
"""

import json
import os
import numpy as np

import path_definitions as pd

# Create stretch and node_force bins

X_MIN = -10
X_MAX = 10
X_STEP = 0.25

F_MIN = -0.5
F_MAX = 0.5
F_STEP = 0.25/20

NB_INTER = int((X_MAX - X_MIN)/X_STEP)


def get_m_kinetics(model_json_file):
    
    # Extract the myosin kinetics from the json model file 

    with open(model_json_file, 'r') as f:
        mod = json.load(f)
           
    m_kinetics = []
    
    for i, isotypes in enumerate(mod["m_kinetics"]): # Get data for each isotype
        
        idx = 0
        
        data_scheme = []
        
        for j, state in enumerate(isotypes["scheme"]): # Get kinetic scheme for each state
            
            state_data = {}
                        
            state_data["state_number"] = state["number"]
            state_data["state_type"] = state["type"]
            state_data["transition"] = []  # array for the different transitions
                       
            for k, trans in enumerate(state["transition"]): # Get transition data for each new state
                
                trans_data = {}
                
                trans_data["to"] = trans["new_state"]
                trans_data["index"] = idx
                trans_data["rate_type"] = trans["rate_type"]
                trans_data["rate_parameters"] = trans["rate_parameters"]
                
                idx += 1
                
                state_data["transition"].append(trans_data)
                
            data_scheme.append(state_data)
        
        m_kinetics.append(data_scheme)
        
    # Save the kinetics structure in a JSON file
    outfile = os.path.join(pd.OUTPUT_DIR, "m_kinetics.json")  
    with open(outfile, 'w') as f:
        json.dump(m_kinetics, f)
        
    return m_kinetics

def calculate_rate_from_m_kinetics(m_kinetics, model_json_file):
    
    # Extract the rate laws from the kinetic data
       
    stretch = np.arange(X_MIN,X_MAX, X_STEP)    
    node_force = np.arange(F_MIN,F_MAX, F_STEP)
    
    with open(model_json_file, 'r') as f:
        mod = json.load(f)

    k_cb = mod["m_parameters"]["m_k_cb"]

    for i, iso in enumerate(m_kinetics):
        
        rate_values = [stretch, node_force]
    
        for j, state in enumerate(iso):
            
            for new_state in state["transition"]:
                
                trans_type = new_state["rate_type"]
                trans_param = new_state["rate_parameters"]
                
                if trans_type == "constant":
                    
                    rate_trans = [trans_param[0] for x in stretch]
                    rate_values.append(rate_trans)
                                   
                elif trans_type == "gaussian":
                    
                    rate_trans = [trans_param[0]*np.exp(-0.5 * k_cb * np.power(x, 2)/(1e18 * 1.38e-23*310)) for x in stretch]
                    rate_values.append(rate_trans)
                                    
                elif trans_type == "poly":
                    
                    rate_trans = [trans_param[0] + trans_param[1] * np.power(x, trans_param[2]) for x in stretch]
                    rate_values.append(rate_trans)
                    
                elif trans_type == "force_dependent":
                    
                    rate_trans = [trans_param[0] * (1 + trans_param[1] * nf) for nf in node_force]
                    rate_values.append(rate_trans)
                    
                else:
                    raise RuntimeError(f"Transition of type {trans_type} is unknown")
                    
        filename = os.path.join(pd.OUTPUT_DIR, f"rate_equations_iso_{i}.txt")    
        np.savetxt(filename, np.transpose(rate_values))
        
def get_stretch_interval(stretch):
    
    # Get the stretch bins for calculating probabilities 
        
    no_interval = int(np.floor((stretch - X_MIN) / X_STEP))
    
    if no_interval >= NB_INTER:
        #print('interval out of bounds')
        no_interval = NB_INTER-1
        
    if no_interval <= 0:
        #print('interval out of bounds')
        no_interval = 0

    return no_interval