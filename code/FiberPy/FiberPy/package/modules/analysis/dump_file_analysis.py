"""@package FiberSim_analysis
Documentation for the FiberSim_analysis module

More stuff here
"""

import json

import numpy as np

def extract_dump_data(dump_file_string):
    
    # Load the dump file as a dict
    
    with open(dump_file_string, 'r') as f:
        d = json.load(f)
        
    # Get the thick filaments
    d_thick = d['thick']
    
    # Make a dict to hold data
    thick = dict()
    for (i, f) in enumerate(d_thick):
        print(i)
        if (i==0):
            thick['m_no_of_states'] = f['m_no_of_states']
            thick['m_no_of_isotypes'] = f['m_no_of_isotypes']
            thick['states'] = np.zeros([thick['m_no_of_states'],
                                        thick['m_no_of_isotypes']])

        # Now count the states for each isotype
        cbs = np.asarray(f['cb_state'], dtype=int)
        cbi = np.asarray(f['cb_iso'], dtype=int)
        
        print(cbi)
        
        for iso in range(thick['m_no_of_isotypes']):
            for state in range(thick['m_no_of_states']):
                
                iso_matches_state = cbi[np.nonzero(cbs == (state+1))[0]]
                matches = np.nonzero(iso_matches_state == (iso+1))[0]
                
                thick['states'][state, iso] = thick['states'][state, iso] + \
                        len(matches)
    
    print(thick)
    
    return thick
