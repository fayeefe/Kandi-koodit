import csv
originalData = []
VoltageData = []
thicknessData = []

feedRate = 0.3 #m/min

with open("filename.csv", "r", newline='') as f:
    reader = csv.reader(f)
    next(reader)
    for row in reader:
        data = []
        voltage = float(3.3/1023*int(row[0]))
        data.append(voltage)
        data.append(row[1])
        VoltageData.append(data)
        data.clear

        #Handles the thickness calculation !!!!!!!!!!!!!!the voltage values need to be calibrated with the machine!!!!!!!!!!!!!!!!!!!!!!
        #thickness = float(12.7/(V_max-V_min)*voltage+v_offset)
        #data.append(thickness)
        #data.append(row[1])
        #thicknessData.append(data)
        originalData.append(row)

# Print out the original values in a csv file (veeery unnecesary but a good template for file creation)
with open('Testfile.csv', 'w', newline='') as csvFile:
    fieldnames = ['Value', 'time']
    writer = csv.writer(csvFile)
    writer.writerow(fieldnames)
    writer.writerows(originalData)

# creates the voltage values (not super necessary but good for debugging)
with open('VoltageValues.csv', 'w', newline='') as Vcsv:
    fieldnames = ['voltage', 'time']
    writer = csv.writer(Vcsv)
    writer.writerow(fieldnames)
    writer.writerows(VoltageData)

# creates the voltage values (not super necessary but good for debugging)
with open('thicknessValues.csv', 'w', newline='') as Vcsv:
    fieldnames = ['thickness', 'time', 'length']
    writer = csv.writer(Vcsv)
    writer.writerow(fieldnames)
    writer.writerows(thicknessData)
