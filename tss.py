import numpy as np 
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from scipy.stats import gaussian_kde


def parseFile(fileName):
	fh = open(fileName, 'r') #Open file with input name
	data = fh.readlines()
	fh.close
	# Initialize lists
	HR = [] 
	t = []
	for line in data: # Pass through all scanned data
		if line.find("<ns3:hr>") != -1: # If a heart rate tag is found
			startHR = line.find("<ns3:hr>") # Find start and end to split the line
			stopHR = line.find("</ns3:hr>")
			HR.append(float(line[startHR+8:stopHR])) # Extract only HR number. 8 is the length of the HR tag. <ns3:hr>
		elif line.find("<time>") != -1: # Do the same thing for time
			startT = line.find("<time>")
			stopT = line.find("</time>")
			t.append(float(line[startT+17:stopT-11])*3600+float(line[startT+20:stopT-8])*60+float(line[startT+23:stopT-5]))  #This line extracts the hours, minutes and seconds. They are all converted to seconds and appended to the time list
	t[:] = [i - t[0] for i in t]
	t.pop(0) #Delete first element of time which corresponds to activity start time. 
	return(HR,t)

def getZones():
	zones = [] # Initialize list
	fh = open('userData','r') # Read user data of format "RHR,MaxHR"
	data = fh.readlines()
	fh.close()
	HR = data[0].split(",")
	for i in range(0,6):  # Calculate each zone according to (Max HR - RHR) * [zone percentage] + RHR
		zones.append((float(HR[1]) - float(HR[0]))*((i+5.0)/10.0) + float(HR[0]))
	return(zones, (float(HR[1]) - float(HR[0])), float(HR[0]))

def getTimeInZones(HR, t, zones):
	tInZones = [0, 0, 0, 0, 0]  # Initialize at zero
	for i in range(0,len(HR)-1):
		if HR[i] < zones[0]:
			tInZones[0] += 1
		elif HR[i] < zones[2]:
			tInZones[1] += 1
		elif HR[i] < zones[3]:
			tInZones[2] += 1
		elif HR[i] < zones[4]:
			tInZones[3] += 1
		else:
			tInZones[4] += 1
	return tInZones

def calcTrimp(HR, t, HRR, RHR):
	trimp = 0
	for i in range(int(min(HR)), int(max(HR))):
		count = HR.count(i)
		Hr = ((i)- RHR) / HRR
		trimp += float(count) / 60.0 * Hr * .64 * np.exp(1.92 * Hr)
	return trimp

def generatePlot(HR, t, zones, tInZones):
	plt.figure()
	plt.plot(t, HR)
	plt.xlabel('Time (s)')
	plt.ylabel('HR (bpm)')
	plt.title('Heart Rate Over Time')
	currentAxis = plt.gca()
	currentAxis.add_patch(Rectangle((0, (zones[4])), t[len(t)-1], zones[5] - zones[4], alpha=.15, label='Z5', facecolor='#cc0000'))
	currentAxis.add_patch(Rectangle((0, (zones[3])), t[len(t)-1], zones[4] - zones[3], alpha=.15, label='Z4', facecolor='#cc8400'))
	currentAxis.add_patch(Rectangle((0, (zones[2])), t[len(t)-1], zones[3] - zones[2], alpha=.15, label='Z3', facecolor='#1ecc00'))
	currentAxis.add_patch(Rectangle((0, (zones[1])), t[len(t)-1], zones[2] - zones[1], alpha=.15, label='Z2', facecolor='#6dc9ff'))
	currentAxis.add_patch(Rectangle((0, (zones[0])), t[len(t)-1], zones[1] - zones[0], alpha=.15, label='Z1', facecolor='#d142f4'))

	plt.legend(loc=4)


	plt.figure()
	#z1, z2, z3, z4, z5 = plt.barh(np.arange(1,6),tInZones)
	labels = 'Z1', 'Z2', 'Z3', 'Z4', 'Z5'
	colors = ['#d142f4', '#6dc9ff', '#1ecc00', '#cc8400', '#cc0000']
	plt.pie(tInZones, labels = labels, explode = (.05,.05,.05,.05,.05), colors = colors)
	plt.title('Time in Zones')
	#z5.set_facecolor('#cc0000')
	#z4.set_facecolor('#cc8400')
	#z3.set_facecolor('#1ecc00')
	#z2.set_facecolor('#6dc9ff')
	#z1.set_facecolor('#d142f4')


	iqr = np.subtract(*np.percentile(HR, [75, 25])) # interquartile range
	nbins = int((max(HR)-min(HR))/(2.0*iqr/(len(HR)**(1.0/3.0)))) #Freedman-Diaconis rule for number of bins in histogram


	plt.figure()
	kde = gaussian_kde(HR)
	x = np.linspace(min(HR)-10, max(HR)+10, 500)
	pdf = kde.evaluate(x)
	currentAxis = plt.gca()
	currentAxis.add_patch(Rectangle(((zones[4]), 0), zones[5] - zones[4], max(pdf)*1.5, alpha=.15, label='Z5', facecolor='#cc0000'))
	currentAxis.add_patch(Rectangle(((zones[3]), 0), zones[4] - zones[3], max(pdf)*1.5, alpha=.15, label='Z4', facecolor='#cc8400'))
	currentAxis.add_patch(Rectangle(((zones[2]), 0), zones[3] - zones[2], max(pdf)*1.5, alpha=.15, label='Z3', facecolor='#1ecc00'))
	currentAxis.add_patch(Rectangle(((zones[1]), 0), zones[2] - zones[1], max(pdf)*1.5, alpha=.15, label='Z2', facecolor='#6dc9ff'))
	currentAxis.add_patch(Rectangle(((zones[0]), 0), zones[1] - zones[0], max(pdf)*1.5, alpha=.15, label='Z1', facecolor='#d142f4'))
	currentAxis.plot(x, pdf)
	plt.hist(HR,normed=1, bins = nbins)
	plt.legend(loc=2)
	plt.title('Heart Rate Histogram and PDF')
	plt.ylim((0,max(pdf)*1.5))
	plt.show()


#fileName = raw_input("Enter file name:")
fileName = "test.gpx"
[HR, t] = parseFile(fileName)
zones, HRR, RHR = getZones()
tInZones = getTimeInZones(HR, t, zones)
trimp = calcTrimp(HR, t, HRR, RHR)
print trimp

#generatePlot(HR, t, zones, tInZones)