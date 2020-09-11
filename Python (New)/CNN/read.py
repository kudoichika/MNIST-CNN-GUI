import gzip
import numpy as np
import pandas as pd

f = gzip.open('train-images-idx3-ubyte.gz','r')
g = gzip.open('train-labels-idx1-ubyte.gz', 'r')
f.read(16)
g.read(8)
bufferX = f.read(60000 * 784)
bufferY = g.read(60000)
dataX = np.frombuffer(bufferX, dtype = np.uint8).astype(np.float32)
dataY = np.frombuffer(bufferY, dtype = np.uint8).astype(np.float32)

dataX = dataX.reshape((60000, 784))
data = np.append(dataY.reshape(dataY.shape[0],-1), dataX, axis = 1)

idx = np.arange(60000)
cols = ['labels']
for i in range(784):
	cols.append('px'+str(i))

df = pd.DataFrame(data, index = idx, columns = cols)
df.to_csv('train.csv')
