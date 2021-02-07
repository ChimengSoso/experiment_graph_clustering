import pandas as pd
# import matplotlib
# matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt
import numpy as np


def run_mu(namefile):
  data = pd.read_csv(namefile)

  d = np.array(data)

  lw = 2
  mksz = 10
  fontsz = 16

  fig, ax = plt.subplots()

  data_x = data['mu'][:4]

  plt.plot(data_x, data['SCAN'][:4], 'g^-.', linewidth=lw,markersize=mksz,label='SCAN')
  plt.plot(data_x, data['pSCAN'][:4], 'rs:', linewidth=lw, markersize=mksz,label='pSCAN')
  plt.plot(data_x, data['Our algorithm'][:4], 'bo--',linewidth=lw,markersize=mksz,label='Our algorithm')
  plt.legend()

  plt.ylabel('Processing Time (s)', fontsize=fontsz)
  # plt.title(f'Dataset "{namefile[4:-4]}"', fontsize=fontsz)
  plt.gca().set_xlabel(r'$\mu$', fontsize=fontsz)
  ax.set_xticks(data_x)
  # fig.tight_layout()
  # plt.show()

  plt.savefig(f'img/mu-{namefile[4:-4]}.png', format='png')
  plt.close()

def run_eps(namefile):
  data = pd.read_csv(namefile)

  d = np.array(data)

  lw = 2
  mksz = 10
  fontsz = 16

  plt.plot(data['epsilon'][4:], data['SCAN'][4:], 'g^-.', linewidth=lw,markersize=mksz,label='SCAN')
  plt.plot(data['epsilon'][4:], data['pSCAN'][4:], 'rs:', linewidth=lw, markersize=mksz,label='pSCAN')
  plt.plot(data['epsilon'][4:], data['Our algorithm'][4:], 'bo--',linewidth=lw,markersize=mksz,label='Our algorithm')
  plt.legend()

  plt.ylabel('Processing Time (s)', fontsize=fontsz)
  # plt.title(f'Dataset "{namefile[4:-4]}"', fontsize=fontsz)
  plt.gca().set_xlabel(r'$\epsilon$', fontsize=fontsz)
  # plt.show()

  plt.savefig(f'img/eps-{namefile[4:-4]}.png', format='png')
  plt.close()

dataset_name = ['as-skitter', 'CA-CondMat', 'com-youtube.ungraph',
                'Email-Enron', 'Gowalla_edges', 'roadNet-CA']

for name in dataset_name:
  namefile = 'out_' + name + '.csv'
  run_mu(namefile)

for name in dataset_name:
  namefile = 'out_' + name + '.csv'
  run_eps(namefile)
