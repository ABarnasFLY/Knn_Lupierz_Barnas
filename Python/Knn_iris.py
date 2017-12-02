import csv
with open('../Data/iris.data.txt') as csvfile:
  lines = csv.reader(csvfile)
  for row in lines:
    print(', '.join(row))