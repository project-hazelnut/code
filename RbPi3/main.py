import os, time
import thread
import numpy as np
import csv
import time


# set up power measurement with ina219 module
from ina219 import INA219

ina = INA219(shunt_ohms=0.1,
             max_expected_amps = 0.2,
             address=0x40)

ina.configure(voltage_range=ina.RANGE_32V,
              gain=ina.GAIN_AUTO,
              bus_adc=ina.ADC_128SAMP,
              shunt_adc=ina.ADC_128SAMP)

power_measurements = []

# power measurement delay in seconds
POWER_MEASUREMENT_DELAY = 1
MEASUREMENT = "test1"

def main():
    start_time = last_iteration_time = time.time()
    elapsed_time = 0
    
    while elapsed_time < 3600:
        power_reading = ina.power() / 1000
        
        print([MEASUREMENT, power_reading])
        
        try:
            if not os.path.exists("./results/"):
                os.makedirs("./results/")
            
            if not os.path.isfile("./results/"+ MEASUREMENT +".csv"):
                with open("./results/"+ MEASUREMENT +".csv", "w") as results_file:
                    writer = csv.writer(results_file)
                    writer.writerow(["measurement", "average_power"])
                    writer.writerow([MEASUREMENT, power_reading])
                    results_file.close()
            else:
                with open("./results/"+ MEASUREMENT +".csv", "w") as results_file:
                    writer = csv.writer(results_file)
                    writer.writerow([MEASUREMENT, power_reading])
                    results_file.close()
        except IOError:
            print("Input output error whilst writing to file. Please check.")
            exit()
        
        time.sleep(POWER_MEASUREMENT_DELAY)
        elapsed_time = time.time() - start_time
        last_iteration_time = time.time()

if __name__ == "__main__":
    main()   



