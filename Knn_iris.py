import csv
with open('iris.data.txt') as csvfile:
  lines = csv.reader(csvfile)
  for row in lines:
    print(', '.join(row))