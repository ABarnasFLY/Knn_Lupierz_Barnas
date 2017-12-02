file = open('../Data/test_labels.byte', 'rb')
file.read(8)
for x in range(20):
  print (int.from_bytes(file.read(1), byteorder='big'))
  
  