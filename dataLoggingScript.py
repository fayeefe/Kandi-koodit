import csv
data = []
with open("filename.csv", "r", newline='') as f:
    reader = csv.reader(f)
    for row in f:
        data.append(row)
        print("test")
print(data)

with open('Testfile.csv', 'w', newline='') as csvFile:
    # Fieldnames = ['Value', 'time']
    writer = csv.writer(csvFile)
    writer.writerows(data)
