import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

dataset_name = ["com-youtube.ungraph.txt",   # DS1
                "as20000102.txt",            # DS2
                "as-skitter.txt",            # DS3
                "Gowalla_edges.txt",         # DS4
                "musae_chameleon_edges.csv", # DS5
                "roadNet-CA.txt",            # DS6
                "roadNet-PA.txt",            # DS7
                "roadNet-TX.txt",            # DS8
                "Email-Enron.txt",           # DS9
                "CA-CondMat.txt"]            # DS10

def run(name):
  data_set = pd.read_csv(f'degree_{name[:-4]}.csv')

  x = data_set['node']
  y = data_set['degree']

  plt.hist(y, color='green',bins=98)  # density=False would make counts
  plt.ylabel('Count Degree')
  plt.xlabel('Degree')
  plt.title(f'dataset "{name[:-4]}"')
  plt.yscale('symlog')
  # plt.xscale('symlog')
  # plt.show()
  plt.savefig(f'img/degree_{name[:-4]}.png', format='png')

for name in dataset_name:
  run(name)