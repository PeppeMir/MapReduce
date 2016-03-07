import sys
import numpy as np
import pandas as pd
from random import randint

if len(sys.argv) != 3:
	print 'Error: expected N_TOT and N_DISTINCT'
	exit(1)

N_TOT = int(sys.argv[1])
N_DISTINCT = int(sys.argv[2])

out_file = "../Code/FileGen/generated/numbers.txt"
pd.DataFrame(N_DISTINCT * np.random.rand(N_TOT)).astype(int).to_csv(out_file, index=False, header=False)
