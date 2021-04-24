import serial  #pyserial
import matplotlib.pyplot as plt
import numpy as np
import csv
import datetime

# TODO: add button input to arduino (or python? keyboardinterrupt?) to start measuring

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
            

            x.append(i)
            weight_arr.append(float(data.decode()))

            if (i > 0):
                rate = weight_arr[i] - weight_arr[i-1]
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
    
    temp = np.average(rate_arr)
    print(f'Average rate of weight loss = {temp} lb/(time unit)')


        # append to a text file!
