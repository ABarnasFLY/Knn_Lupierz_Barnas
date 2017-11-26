file = open('t10k-images.idx3-ubyte' , 'rb')

i = []

file.read(46 +2*28)

n = 20

for x in range(28*28*n):
	i.append(file.read(1))

for x in range(28*n):
  for y in range(28):
    d = int.from_bytes(i[x*28+y], byteorder='big')
    if d > 50:
      print('@' , end = ' ')
    else:
      print('~' , end = ' ')
  print(' ')
file.close()
