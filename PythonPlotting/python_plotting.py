#file python_plotting.py
#author Eric Torres
#date 28 Apr 2021

# user must set this value to get proper readings!
time_difference = 1/60  # running at 1 minute, converting to hours

import serial  #pyserial
import matplotlib.pyplot as plt
import numpy as np
import csv
import datetime

plt.ion()
fig=plt.figure()


i=0
x=list()
rate_arr = [0]
weight_arr = list()
y=list()

ser = serial.Serial('COM19', 9600)
ser.close()
ser.open()
with open('weight_data.csv', 'w') as fobj:
    fields = ['time', 'weight', 'rate']
    writer = csv.DictWriter(fobj, fieldnames=fields)
    writer.writeheader()

    while True:

        try:
            data = ser.readline()
            # print(data.decode())
            
            try:
                weight_arr.append(float(data.decode()))
                x.append(i)
            except ValueError:
                i -= 1
                print(data.decode())
            # print(type(data))
            # x.append(i)
            # if type(data) == float:
            #     weight_arr.append(float(data.decode()))

            if (i > 0):
                # if(i == 1):
                #     first_time = datetime.datetime.now()
                # elif(i == 2):
                #     second_time = datetime.datetime.now()
                #     diff_time = second_time - first_time

                rate = (weight_arr[i] - weight_arr[i-1]) / time_difference
                rate_arr.append(rate)

                # print(x)
                # print(rate_arr)

                plt.subplot(1,2,1)
                plt.plot(x, weight_arr)
                plt.title('Weight vs. Time')

                plt.subplot(1,2,2)
                plt.plot(x, rate_arr)
                plt.title('Weight Loss Rate vs. Time')
                plt.show()
                plt.pause(0.0001)

                writer.writerow({'time' : datetime.datetime.now(), 'weight' : weight_arr[-1], 'rate' : rate_arr[-1]})

            # plt.plot(x, rate_arr)

            # data2 = ser.readline()
            # print(data2.decode())

            # x.append(i)
            # y.append(float(data.decode()))

            # plt.plot(x, y)
            i += 1
            # x.append(i)
            # plt.plot(x, rate_arr)
            # plt.show()
            # plt.pause(0.0001)  # Note this correction

        except KeyboardInterrupt:
            break
    
    avg = np.average(rate_arr)
    print(f'Average rate of weight loss = {avg} lb/hr')


        # append to a text file!
