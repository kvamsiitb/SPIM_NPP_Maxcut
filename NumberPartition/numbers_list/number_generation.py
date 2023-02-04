#!/usr/bin/python
# -*- coding: utf-8 -*-

# In[1]: polarization 45

### IMPORTING REQUISITE PACKAGES


import numpy as np
import math


numbers_shape = 100
numbers = np.random.uniform(0, 1, (numbers_shape, numbers_shape))
numbers = (numbers*1000).astype(int)
print(numbers)
#

