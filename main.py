import serial
import time
import numpy
import matplotlib.pyplot as plt
from drawnow import *


levelVec = []
ntuVec = []

#Listening port
port = "COM5"
bluetooth = serial.Serial(port, 9600)
plt.ion()
cnt = 0
xMax = 0

#I'm trying to produce the graphic using the values received from bluetooth
#the graphic will contain 2 axes: one for values received from turbidity sensor
#and another for values received from water level sensor
def makeFig():
	plt.ylim(0, 5)
	plt.title("Streaming values of water")
	plt.grid(True)
	plt.ylabel('Voltage ')
	plt.plot(ntuVec, 'black', label ='Voltage')
	plt.hlines(y=4.2, xmin = 0, xmax = xMax,  color='green', linestyle='dashed', linewidth=2, label = 'Clean water')
	plt.legend(loc='upper left')
	plt2 = plt.twinx()
	plt.ylim(0, 800)
	plt2.plot(levelVec, 'blue', label = 'Water level')
	plt2.hlines(y=600, xmin=0, xmax=xMax, color='red', linestyle='dashed', linewidth=2, label='Water overflow')
	plt2.set_ylabel('Water level')
	plt2.ticklabel_format(useOffset=False)
	plt2.legend(loc='upper right')


while True:
	while bluetooth.inWaiting() == 0:
		pass
	#parsing the received data
	arduinoString = bluetooth.readline()
	line = arduinoString.decode()
	finalString = line.strip()
	dataArray = finalString.split(", ")
	levelValue = float(dataArray[1])
	voltageValue = float(dataArray[0])
	# print(levelValue)
	# print(voltageValue)
	levelVec.append(levelValue)
	ntuVec.append(voltageValue)
	drawnow(makeFig)
	plt.pause(.000001)
	cnt = cnt + 1
	xMax = xMax + 1
	if (cnt > 50):
		ntuVec.pop(0)
		levelVec.pop(0)



