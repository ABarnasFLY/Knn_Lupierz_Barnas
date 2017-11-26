file = open('test_images.byte' , 'rb')
file_labels = open('test_labels.byte', 'rb')


i = []
images = []
number = 20;
file.read(46 +2*28)
file_labels.read(8)
n = 20

for p in range(number):
  for x in range(28*28):
    i.append(file.read(1))
  i.append(int.from_bytes(file_labels.read(1), byteorder='big'))
  images.append(i)
  i =[]
for p in range(number):
  for x in range(28):
    for y in range(28):
      d = int.from_bytes(images[p][x*28+y], byteorder='big')
      if d > 50:
        print('@' , end = ' ')
      else:
        print('~' , end = ' ')
    print(' ')
  print(images[p][-1])
file.close()
