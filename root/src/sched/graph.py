import pandas as pd
import matplotlib.pyplot as plt

d1 = pd.read_csv('./1core-1process.csv')
d2 = pd.read_csv('./1core-2process.csv')
d3 = pd.read_csv('./1core-3process.csv')

plt.plot(d1)
plt.plot(d2)
plt.plot(d3)
