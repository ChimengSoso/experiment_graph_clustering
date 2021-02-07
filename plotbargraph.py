import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data_set = pd.read_csv('out_all_dataset.csv')

labels = [f'DS{id}' for id in range(len(data_set))]
# labels = [f'{name[:-4]}' for name in data_set['datasetname']]

scan = data_set['SCAN']
pscan = data_set['PSCAN']
chi = data_set['Our Algorithm']

x = np.arange(len(labels))  # the label locations
width = 0.2  # the width of the bars

fontsz = 16

fig, ax = plt.subplots()
rects1 = ax.bar(x - width, scan, width, label='SCAN',color='limegreen', edgecolor='black', hatch='-')
rects2 = ax.bar(x, pscan, width, label='pSCAN',color='tomato', edgecolor='black', hatch='x')
rects3 = ax.bar(x + width, chi, width, label='Our Algorithm',color='dodgerblue', edgecolor='black', hatch='.')

ax.set_ylabel('Processing Time (s)', fontsize=fontsz)
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=0, fontsize=fontsz/1.5)
ax.legend()

# ax.xaxis_date()     # interpret the x-axis values as dates
# fig.autofmt_xdate() # make space for and rotate the x-axis tick labels

plt.show()