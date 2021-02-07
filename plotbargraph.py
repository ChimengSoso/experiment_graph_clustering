import matplotlib
import matplotlib.pyplot as plt
import numpy as np


labels = ['DS1', 'DS2', 'DS3', 'DS4', 'DS5', 'DS6', 'DS7', 'DS8', 'DS9', 'DS10']
men_means = [20, 34, 30, 35, 27, 10, 20, 30, 40, 50]
women_means = [25, 32, 34, 20, 25, 55, 88, 99, 44, 22]
third = [10, 15, 17, 23, 21, 11, 55, 33, 20, 10]

x = np.arange(len(labels))  # the label locations
width = 0.2  # the width of the bars


fontsz = 16


fig, ax = plt.subplots()
rects1 = ax.bar(x - width, men_means, width, label='SCAN',color='limegreen', edgecolor='black', hatch='-')
rects2 = ax.bar(x, women_means, width, label='pSCAN',color='tomato', edgecolor='black', hatch='x')
rects3 = ax.bar(x + width, third, width, label='Our Algorithm',color='dodgerblue', edgecolor='black', hatch='.')

# plt.figure(1, figsize=(18, 4))

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Processing Time (s)', fontsize=fontsz)
  # plt.ylabel('Processing Time (s)', fontsize=fontsz)
# ax.set_title('Scores by group and gender')
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=0, fontsize=fontsz)
ax.legend()

# plt.xscale('symlog')
# plt.yscale('symlog', linthresh=0.002)

def autolabel(rects):
  """Attach a text label above each bar in *rects*, displaying its height."""
  for rect in rects:
    height = rect.get_height()
    ax.annotate('{}'.format(height),
                xy=(rect.get_x() + rect.get_width() / 2, height),
                xytext=(0, 3),  # 3 points vertical offset
                textcoords="offset points",
                ha='center', va='bottom')


# autolabel(rects1)
# autolabel(rects2)
# autolabel(rects3)

# fig.tight_layout()

# xmin, xmax = plt.xlim()
# ymin, ymax = plt.ylim()

# scale_factor = 5

# plt.xlim(xmin * scale_factor, xmax * scale_factor)
# plt.yscale('log')
# plt.ylim(ymin * scale_factor, ymax * scale_factor)


# fig = matplotlib.pyplot.gcf()
# fig.set_size_inches(10.5, 5.5)
# fig.savefig('test2png.png', dpi=100)


plt.show()