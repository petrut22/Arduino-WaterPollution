import serial
import time
import numpy
import matplotlib.pyplot as plt
from drawnow import *


levelVec = []
ntuVec = []

port = "COM7"
bluetooth = serial.Serial(port, 9600)
plt.ion()
cnt = 0
xMax = 0

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
	while(bluetooth.inWaiting() == 0):
		pass
	arduinoString = bluetooth.readline()
	line = arduinoString.decode()
	finalString = line.strip()
	dataArray = finalString.split(", ")

	levelValue = float(dataArray[1])
	voltageValue = float(dataArray[0])
	print(levelValue)
	print(voltageValue)
	levelVec.append(levelValue)
	ntuVec.append(voltageValue)
	drawnow(makeFig)
	plt.pause(.000001)
	cnt = cnt + 1
	xMax = xMax + 1
	if (cnt > 50):
		ntuVec.pop(0)
		levelVec.pop(0)



