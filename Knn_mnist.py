import random
import math
import operator

def loadDataset(filename_image, filename_labels, split, number, trainingSet=[], testSet=[]):
  #need to be implemented
  file = open(filename_image , 'rb')
  file_labels = open(filename_labels, 'rb')

  i = []
  images = []
  #read some kind of header
  file.read(46 +2*28)
  file_labels.read(8)
  
  for p in range(number):
    for x in range(28*28):
      i.append(int.from_bytes(file.read(1), byteorder = 'big'))
    i.append(int.from_bytes(file_labels.read(1), byteorder='big'))
    images.append(i)
    i =[]
	
  for x in range(len(images)):
    if random.random() < split:
      trainingSet.append(images[x])
    else:
      testSet.append(images[x])


def euclideanDistance(instance1, instance2, length):
  distance = 0
  for x in range(length):
    distance += pow((instance1[x] - instance2[x]),2)
  return math.sqrt(distance)

def getNeighbors(trainingSet, testInstance, k):
  distances=[]
  length = len(testInstance)-1
  for x in range(len(trainingSet)):
    dist = euclideanDistance(testInstance, trainingSet[x], length)
    distances.append((trainingSet[x], dist))
  distances.sort(key=operator.itemgetter(1))
  neighbors = []
  for x in range(k):
    neighbors.append(distances[x][0])
  return neighbors

def getResponse(neighbors):
  classVotes = {}
  for x in range(len(neighbors)):
    response = neighbors[x][-1]
    if response in classVotes:
      classVotes[response] += 1
    else:
      classVotes[response] = 1
  sortedVotes = sorted(classVotes.items(), key=operator.itemgetter(1), reverse = True)
  return sortedVotes[0][0]

def getAccuracy(trueSet, predictions):
  correct = 0
  for x in range(len(trueSet)):
    print(repr(predictions[x]) + ' ' + repr(trueSet[x][-1]))
    if trueSet[x][-1] == predictions[x]:
      correct += 1
  return (correct/float(len(trueSet))) * 100.0
  
def main():
  trainingSet = []
  testSet = []
  split = 0.67
  number = 1000
  loadDataset('test_images.byte','test_labels.byte', split, number, trainingSet, testSet)
  print('Train set: ' + repr(len(trainingSet)))
  print('Test set: ' + repr(len(testSet)))
  
  predictions = []
  k = 3
  for x in range(len(testSet)):
    neighbors = getNeighbors(trainingSet,testSet[x],k)
    result = getResponse(neighbors)
    predictions.append(result)
    print('> predicted = ' + repr(result) + ', actual = ' + repr(testSet[x][-1]))
  accuracy = getAccuracy(testSet, predictions)
  print('Accuracy : ' + repr(accuracy) + '%')
  
main()