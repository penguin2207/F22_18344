import os
import commands
from csv import reader
import csv

AMAT = 100.0
zero_config = ((0,0,0,0),(0,0,0,0))

pinFile = 'pinOut_262144.csv'
spread = 6

AMATS_0 = [100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,]
AREAS = [100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,100.0,]
best_configs_0 = [zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config,zero_config]

with open(pinFile, 'r') as pin_obj:
    pin_reader = reader(pin_obj, skipinitialspace=True, delimiter=',')
    for pin_row in pin_reader:
        pin_pol_L2 = int(pin_row[10])
        pin_size_L2 = int(pin_row[11])
        pin_assoc_L2 = int(pin_row[12])
        pin_set_L2 = int(pin_row[13])
        pin_block_size_L2 = int(pin_row[14])
        pin_miss_rate_L2 = pin_row[18]
        if(pin_miss_rate_L2 == "nan"):
          continue
        pin_miss_rate_L2 = float(pin_row[18])
        pin_hit_rate_L2 = 1-pin_miss_rate_L2

        pin_pol_L1 = int(pin_row[0])
        pin_size_L1 = int(pin_row[1])
        pin_assoc_L1 = int(pin_row[2])
        pin_set_L1 = int(pin_row[3])
        pin_block_size_L1 = int(pin_row[4])
        pin_miss_rate_L1 = float(pin_row[8])
        pin_hit_rate_L1 = 1-(pin_miss_rate_L1)

        L1_found = False
        L2_found = False

        L1_hit_val = 0
        L1_miss_val = 0
        L1_area_val = 0
        L2_hit_val = 0
        L2_miss_val = 0
        L2_area_val = 0
        

        with open('destinyLog.csv', 'r') as destiny_obj:
          destiny_reader = reader(destiny_obj)
          for dest_row in destiny_reader:
            dest_miss_rate = dest_row[5]
            if(dest_miss_rate == "nfigur"):
              continue
            dest_size = int(dest_row[0])
            dest_assoc = int(dest_row[1])
            dest_block_size = int(dest_row[2])
            dest_area_val = float(dest_row[3])
            dest_hit_val = float(dest_row[4])
            dest_miss_val = float(dest_row[5])
            if(dest_size == pin_size_L1):
              if(dest_assoc == pin_assoc_L1):
                if(dest_block_size == pin_assoc_L1):
                  L1_found = True
                  L1_hit_val = dest_hit_val
                  L1_miss_val = dest_miss_val
                  L1_area_val = dest_area_val
            if(dest_size == pin_size_L2):
              if(dest_assoc == pin_assoc_L2):
                if(dest_block_size == pin_assoc_L2):
                  L2_found = True
                  L2_hit_val = dest_hit_val
                  L2_miss_val = dest_miss_val
                  L2_area_val = dest_area_val

        if(L2_found and L1_found):    
          AMAT_new = L1_hit_val * pin_hit_rate_L1 + pin_miss_rate_L1*(L1_miss_val + pin_hit_rate_L2*L2_hit_val + pin_miss_rate_L2*(L2_miss_val + 9.5))
          ind = 0
          for val in AMATS_0:
            if(AMAT_new<val):
              old_AMAT = AMATS_0[ind]
              old_config = best_configs_0[ind]
              old_area = AREAS[ind]
              newInd = ind
              while newInd < 500:
                old_AMAT2=AMATS_0[newInd+1]
                old_config2=best_configs_0[newInd+1]
                old_area2 = AREAS[newInd+1]
                if(old_AMAT<old_AMAT2):
                  AMATS_0[newInd+1]=old_AMAT
                  best_configs_0[newInd+1]=old_config
                  AREAS[newInd+1]=old_area
                old_AMAT = old_AMAT2
                old_config=old_config2
                old_area=old_area2
                newInd=newInd+1
              AMATS_0[ind]=AMAT_new
              AREAS[ind]=L1_area_val + L2_area_val
              best_configs_0[ind] = ((pin_assoc_L1, pin_block_size_L1, pin_size_L1, pin_pol_L1),(pin_assoc_L2, pin_block_size_L2, pin_size_L2, pin_pol_L2))
              break
            ind=ind+1

for x in range(0, 500, spread):
  with open('iterationLog.csv', mode='a') as iteration_file:
            iteration_writer = csv.writer(iteration_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            iteration_writer.writerow([best_configs_0[x], AMATS_0[x], AREAS[x]])

