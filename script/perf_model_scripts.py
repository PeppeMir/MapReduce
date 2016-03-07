
# coding: utf-8

# In[93]:

import numpy, pandas as pd, matplotlib.pyplot as plt, plotly.plotly as py
from plotly.graph_objs import *

def plot(mtrx, cols):
    mtrx[cols].plot(figsize=(14,5), xticks=matrix['NUM_THREAD'])
    plt.axvline(60,c='red')
    plt.axvline(120,c='red')
    plt.axvline(180,c='red')
    plt.grid()
    
def plotly_plot(xval, yval_list, yval_labels):
    data = [ ]
    for yval, label in zip(yval_list, yval_labels):
        trace = Scatter(
        x = xval.tolist(), 
        y = yval.tolist(),
        mode="lines+markers",
        name=label)  # , filename="path"
        data.append(trace)
    return data   


# In[94]:

#import plotly.tools as tls
#tls.set_credentials_file(username='GiuseppeMiraglia', api_key='p2lmmrnhuf')


# In[95]:

############################################################################################
################################ PARAMETERS INITIALIZATION #################################
############################################################################################

# hand measured parameters values (milliseconds)       
key_extract_time = 0.00023
usr_map_time = 144.41
usr_reduce_time = 149.178

# read sequential file
input_seq_file="SEQ_coscos_VAL=100_SIZE=5000.csv"
seq_matrix = pd.read_csv(input_seq_file, sep=',')

# read the parallel file
input_file = "coscos_VAL=100_SIZE=5000.csv"
matrix = pd.read_csv(input_file, sep=',')

# create a data frame for the output
tot_output_file = "results/output_TOTAL.csv"
map_output_file = "results/output_MAP.csv"
reduce_output_file = "results/output_REDUCE.csv"

model_matrix_df = pd.DataFrame()


# In[96]:

#############################################################################
#################### SEQUENTIAL MODEL TOTAL TIME ############################
#############################################################################

time_readfile_seq = seq_matrix['FILE_READING_TIME']
time_map_seq = seq_matrix['NUM_RECORD'] * (key_extract_time + usr_map_time) 
time_sort_seq = seq_matrix['SORT_TIME']
time_reduce_seq = seq_matrix['INTERM_KEYS_NUM'] * usr_reduce_time

seq_value = (time_readfile_seq + time_map_seq + time_sort_seq + time_reduce_seq)[0]


# In[97]:

# group the matrix by thread number
matrix_gr = matrix.groupby('NUM_THREAD')

# apply average on each group
matrix_mean = pd.DataFrame([g.mean() for n, g in matrix_gr], index=[n for n, g in matrix_gr])

# add column in front
matrix_mean.index.rename('nTreads',inplace=True)


# In[98]:

#############################################################################
####################### PARALLEL MODEL TOTAL TIME ###########################
#############################################################################

time_readfile = matrix_mean['FILE_READING_TIME'].mean()
#print time_readfile, "\n"

time_maptaskgen = matrix_mean['MAP_TASK_GEN_TIME'].mean()
#print time_maptaskgen, "\n"

time_map = (matrix_mean['NUM_RECORD'] / matrix_mean['NUM_THREAD']) * (key_extract_time + usr_map_time)
#print time_map, "\n"

time_localsort = matrix_mean['LOCAL_SORT_AVGTIME'].mean()
#print time_localsort, "\n"

time_kway = matrix_mean['SORT_TIME'].mean()
#print time_kway, "\n"

time_reducetaskgen = matrix_mean['REDUCE_TASK_GEN_TIME'].mean()
#print time_reducetaskgen, "\n"

time_reduce = ((matrix_mean['INTERM_KEYS_NUM'] / matrix_mean['NUM_THREAD'])             + matrix_mean['NUM_THREAD']) * usr_reduce_time
#print time_reduce, "\n"

# compute total time
tot_model_time = time_readfile + time_maptaskgen + time_map + time_localsort + time_kway +     time_reducetaskgen + time_reduce

model_matrix_df['TOT_MODEL_TIME'] = tot_model_time
model_matrix_df['TOT_EFFECTIVE_TIME'] = matrix_mean['TOTAL_TIME']

#model_matrix_df['TOT_MODEL_TIME'].plot(figsize=(14,5))
#model_matrix_df['TOT_EFFECTIVE_TIME'].plot(figsize=(14,5))


# In[99]:

############################################################
############### MODEL TOTAL SCALABILITY ####################
############################################################

# model Tpar(1)
base_scalab_value = model_matrix_df['TOT_MODEL_TIME'].iloc[0]        

# model scalability(n) = Tpar(1) / Tpar(n)
model_matrix_df['MODEL_SCALABILITY'] = base_scalab_value /  model_matrix_df['TOT_MODEL_TIME']

# plot
#model_matrix_df['MODEL_SCALABILITY'].plot(figsize=(14,5))


# In[100]:

############################################################
################# MODEL TOTAL SPEEDUP ######################
############################################################

# model speedup(n) = Tseq / Tpar(n)
model_matrix_df['MODEL_SPEEDUP'] = seq_value / model_matrix_df['TOT_MODEL_TIME']

# plot
#model_matrix_df['MODEL_SPEEDUP'].plot(figsize=(14,5))


# In[101]:

############################################################
################ MODEL TOTAL EFFICIENCY ####################
############################################################

# model efficiency(n) = speedup(n) / n
model_matrix_df['MODEL_EFFICIENCY'] = model_matrix_df['MODEL_SPEEDUP'] / matrix_mean['NUM_THREAD']

# plot
#model_matrix_df['MODEL_EFFICIENCY'].plot(figsize=(14,5),ylim=(0))


# In[102]:

################################################################
############### EFFECTIVE TOTAL SCALABILITY ####################
################################################################

# effective Tpar(1)
base_eff_scalab_value = matrix_mean['TOTAL_TIME'].iloc[0] 

# effective scalability(n) = Tpar(1) / Tpar(n)
model_matrix_df['EFFECTIVE_SCALABILITY'] = base_eff_scalab_value / matrix_mean['TOTAL_TIME']

# plot
#model_matrix_df['EFFECTIVE_SCALABILITY'].plot(figsize=(14,5))


# In[103]:

################################################################
################### EFFECTIVE TOTAL SPEEDUP ####################
################################################################

# effective Tseq
effective_seq_value = seq_matrix['TOTAL_TIME']

# effective speedup(n) = Tseq / Tpar(n)
model_matrix_df['EFFECTIVE_SPEEDUP'] = seq_value / matrix_mean['TOTAL_TIME']

# plot
#model_matrix_df['EFFECTIVE_SPEEDUP'].plot(figsize=(14,5))


# In[104]:

################################################################
################ EFFECTIVE TOTAL EFFICIENCY ####################
################################################################

# effective efficiency(n) = speedup(n) / n
model_matrix_df['EFFECTIVE_EFFICIENCY'] = model_matrix_df['EFFECTIVE_SPEEDUP'] / matrix_mean['NUM_THREAD']

# plot
#model_matrix_df['EFFECTIVE_EFFICIENCY'].plot(figsize=(14,5))


# In[105]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ model_matrix_df['MODEL_SCALABILITY'], model_matrix_df['EFFECTIVE_SCALABILITY'] ],
                     [ 'MODEL_SCALABILITY', 'EFFECTIVE_SCALABILITY' ]))'''

plot(model_matrix_df, ['MODEL_SCALABILITY', 'EFFECTIVE_SCALABILITY'])


# In[106]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ model_matrix_df['MODEL_SPEEDUP'], model_matrix_df['EFFECTIVE_SPEEDUP'] ],
                     [ 'MODEL_SPEEDUP', 'EFFECTIVE_SPEEDUP' ]))'''

plot(model_matrix_df, ['MODEL_SPEEDUP', 'EFFECTIVE_SPEEDUP'])


# In[107]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ model_matrix_df['MODEL_EFFICIENCY'], model_matrix_df['EFFECTIVE_EFFICIENCY'] ],
                     [ 'MODEL_EFFICIENCY', 'EFFECTIVE_EFFICIENCY' ]))'''

plot(model_matrix_df, ['MODEL_EFFICIENCY', 'EFFECTIVE_EFFICIENCY'])


# In[108]:

########### MAP ONLY ##########

map_matrix = pd.DataFrame()
map_matrix['MODEL_MAP'] = time_map
map_matrix['EFFECTIVE_MAP'] = (matrix_mean['MAPPERS_TIME'] - matrix_mean['LOCAL_SORT_AVGTIME'])


# In[109]:

####################################################
############### MAP SCALABILITY ####################
####################################################

map_matrix['MODEL_MAP_SCALAB'] =  map_matrix['MODEL_MAP'].iloc[0] / map_matrix['MODEL_MAP']
map_matrix['EFFECTIVE_MAP_SCALAB'] = map_matrix['EFFECTIVE_MAP'].iloc[0] / map_matrix['EFFECTIVE_MAP']

# plot
#map_matrix['EFFECTIVE_MAP_SCALAB'].plot(figsize=(14,5))


# In[110]:

####################################################
################# MAP SPEEDUP ######################
####################################################

map_matrix['MODEL_MAP_SPEEDUP'] =  time_map_seq.iloc[0] / map_matrix['MODEL_MAP']
map_matrix['EFFECTIVE_MAP_SPEEDUP'] = seq_matrix['MAP_TIME'].iloc[0] / map_matrix['EFFECTIVE_MAP']

#map_matrix['EFFECTIVE_MAP_SPEEDUP'].plot(figsize=(14,5))


# In[111]:

#######################################################
################# MAP EFFICIENCY ######################
#######################################################

map_matrix['MODEL_MAP_EFFICIENCY'] = map_matrix['MODEL_MAP_SPEEDUP'] / matrix_mean['NUM_THREAD']
map_matrix['EFFECTIVE_MAP_EFFICIENCY'] = map_matrix['EFFECTIVE_MAP_SPEEDUP'] / matrix_mean['NUM_THREAD']

#map_matrix['EFFECTIVE_MAP_EFFICIENCY'].plot(figsize=(14,5))


# In[112]:

# MAP comparison plots
'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ map_matrix['EFFECTIVE_MAP_SCALAB'], map_matrix['MODEL_MAP_SCALAB'] ],
                     [ 'EFFECTIVE_MAP_SCALAB', 'MODEL_MAP_SCALAB' ]))'''

plot(map_matrix, ['EFFECTIVE_MAP_SCALAB', 'MODEL_MAP_SCALAB'])


# In[113]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ map_matrix['EFFECTIVE_MAP_EFFICIENCY'], map_matrix['MODEL_MAP_EFFICIENCY'] ],
                     [ 'EFFECTIVE_MAP_EFFICIENCY', 'MODEL_MAP_EFFICIENCY' ]))'''

plot(map_matrix, ['EFFECTIVE_MAP_EFFICIENCY', 'MODEL_MAP_EFFICIENCY'])


# In[114]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ map_matrix['EFFECTIVE_MAP_SPEEDUP'], map_matrix['MODEL_MAP_SPEEDUP'] ],
                     [ 'EFFECTIVE_MAP_SPEEDUP', 'MODEL_MAP_SPEEDUP' ]))'''

plot(map_matrix, ['EFFECTIVE_MAP_SPEEDUP', 'MODEL_MAP_SPEEDUP'])


# In[115]:

########### REDUCE ONLY ##########

reduce_matrix = pd.DataFrame()
reduce_matrix['MODEL_REDUCE'] = time_reduce
reduce_matrix['EFFECTIVE_REDUCE'] = matrix_mean['REDUCERS_TIME'] 


# In[116]:

#######################################################
############### REDUCE SCALABILITY ####################
#######################################################

reduce_matrix['MODEL_REDUCE_SCALAB'] =  reduce_matrix['MODEL_REDUCE'].iloc[0] / reduce_matrix['MODEL_REDUCE']
reduce_matrix['EFFECTIVE_REDUCE_SCALAB'] = reduce_matrix['EFFECTIVE_REDUCE'].iloc[0] / reduce_matrix['EFFECTIVE_REDUCE']

#reduce_matrix['EFFECTIVE_REDUCE_SCALAB'].plot(figsize=(14,5))


# In[117]:

#######################################################
################# REDUCE SPEEDUP ######################
#######################################################

reduce_matrix['MODEL_REDUCE_SPEEDUP'] =  time_reduce_seq.iloc[0] / reduce_matrix['MODEL_REDUCE']
reduce_matrix['EFFECTIVE_REDUCE_SPEEDUP'] = seq_matrix['REDUCE_TIME'].iloc[0] / reduce_matrix['EFFECTIVE_REDUCE']

#reduce_matrix['EFFECTIVE_REDUCE_SPEEDUP'].plot(figsize=(14,5))


# In[118]:

######################################@###################
################# REDUCE EFFICIENCY ######################
##########################################################

reduce_matrix['MODEL_REDUCE_EFFICIENCY'] = reduce_matrix['MODEL_REDUCE_SPEEDUP'] / matrix_mean['NUM_THREAD']
reduce_matrix['EFFECTIVE_REDUCE_EFFICIENCY'] = reduce_matrix['EFFECTIVE_REDUCE_SPEEDUP'] / matrix_mean['NUM_THREAD']

#reduce_matrix['EFFECTIVE_REDUCE_EFFICIENCY'].plot(figsize=(14,5))


# In[119]:

# REDUCE comparison plots
'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ reduce_matrix['EFFECTIVE_REDUCE_SCALAB'], reduce_matrix['MODEL_REDUCE_SCALAB'] ],
                     [ 'EFFECTIVE_REDUCE_SCALAB', 'MODEL_REDUCE_SCALAB' ]))'''

plot(reduce_matrix, ['EFFECTIVE_REDUCE_SCALAB', 'MODEL_REDUCE_SCALAB'])


# In[120]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ reduce_matrix['EFFECTIVE_REDUCE_SPEEDUP'], reduce_matrix['MODEL_REDUCE_SPEEDUP'] ],
                     [ 'EFFECTIVE_REDUCE_SPEEDUP', 'MODEL_REDUCE_SPEEDUP' ]))'''

plot(reduce_matrix, ['EFFECTIVE_REDUCE_SPEEDUP', 'MODEL_REDUCE_SPEEDUP'])


# In[121]:

'''py.iplot(plotly_plot(matrix_mean['NUM_THREAD'], 
                     [ reduce_matrix['EFFECTIVE_REDUCE_EFFICIENCY'], reduce_matrix['MODEL_REDUCE_EFFICIENCY'] ],
                     [ 'EFFECTIVE_REDUCE_EFFICIENCY', 'MODEL_REDUCE_EFFICIENCY' ]))'''

plot(reduce_matrix, ['EFFECTIVE_REDUCE_EFFICIENCY', 'MODEL_REDUCE_EFFICIENCY'])


# In[122]:

# write the matrix into the output file
model_matrix_df.to_csv(tot_output_file, index=True)
map_matrix.to_csv(map_output_file, index=True)
reduce_matrix.to_csv(reduce_output_file, index=True)

