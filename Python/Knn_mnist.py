import random
import math
import operator
import time
import numpy as np
from scipy.spatial import distance

def loadDataset(f_test_image, f_test_labels, f_training_image, f_training_labels, trainingSet=[], train_labels=[], testSet = [], test_labels =[]):
  file_ts_imag = open(f_test_image , 'rb')
  file_ts_label = open(f_test_labels, 'rb')
  file_tr_imag = open(f_training_image , 'rb')
  file_tr_label = open(f_training_labels, 'rb')
  iter = 0;
  end = False;
  i = []
  file_ts_imag.read(46+2*28)
  file_ts_label.read(8)
  file_tr_imag.read(46+2*28)
  file_tr_label.read(8)
  while True:
    i = list(file_ts_imag.read(784))
    if not i:
      print(repr(len(testSet)))
      break
    test_labels.append(int.from_bytes(file_ts_label.read(1), byteorder='big'))
    n = np.array(i)
    testSet.append(i)
    iter += 1
    print(iter)
    i = []
  iter = 0
  while True:
    i = list(file_tr_imag.read(784))
    if not i:
      print(repr(len(trainingSet)))
      break
    train_labels.append(int.from_bytes(file_tr_label.read(1), byteorder='big'))
    n = np.array(i)
    trainingSet.append(n)
    iter += 1
    print(iter)
    i = []
	  
  
def loadTestDataset(filename_image, filename_labels, split, number,  trainingSet=[], train_labels=[], testSet = [], test_labels =[]):
  file = open(filename_image , 'rb')
  file_labels = open(filename_labels, 'rb')

  i = []
  images = []
  #read some kind of header
  file.read(46 +2*28)
  file_labels.read(8)
  labels = []
  for p in range(number):
    for x in range(28*28):
      i.append(int.from_bytes(file.read(1), byteorder = 'big'))
    labels.append(int.from_bytes(file_labels.read(1), byteorder='big'))
    images.append(i)
    i =[]
	
  for x in range(len(images)):
    if random.random() < split:
      trainingSet.append(np.array(images[x]))
      train_labels.append(labels[x])
    else:
      testSet.append(np.array(images[x]))
      test_labels.append(labels[x])


def euclideanDistance(instance1, instance2, length):
  distance = 0
  for x in range(length):
    distance += pow((instance1[x] - instance2[x]),2)
  return math.sqrt(distance)

def getNeighbors(k, trainingSet, testInstance, train_labels ):
  distances=[]
  start = time.process_time()
  for x in range(len(trainingSet)-1):
    dist = np.linalg.norm(testInstance - trainingSet[x])
    distances.append((train_labels[x], dist))
  distances.sort(key=operator.itemgetter(1))
  print(time.process_time() - start)
  neighbors = []
  for x in range(k):
    neighbors.append(distances[x][0])
  return neighbors

def getResponse(neighbors):
  classVotes = {}
  for x in range(len(neighbors)):
    response = neighbors[x]
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
  train_labels = []
  test_labels = []
  split = 0.67
  number = 9000
  loadDataset('../Data/test_images.byte', '../Data/test_labels.byte', '../Data/train_images.byte', '../Data/train_labels.byte', trainingSet, train_labels, testSet, test_labels)
  #loadTestDataset('test_images.byte','test_labels.byte', split, number, trainingSet, train_labels, testSet, test_labels)
  print('Train set: ' + repr(len(trainingSet)))
  print('Test set: ' + repr(len(testSet)))
  hits = 0
  iter = 0
  predictions = []
  k = 3
  for x in range(len(testSet)):
    neighbors = getNeighbors(k, trainingSet,testSet[x], train_labels)
    result = getResponse(neighbors)
    predictions.append(result)
    print('> predicted = ' + repr(result) + ', actual = ' + repr(test_labels[x]))
    if result == test_labels[x]:
      hits += 1
    print(repr((float(hits)/float(x+1)) * 100) + '%')
  print('Accuracy : ' + repr((float(hits)/float(x+1)) + '%'))
  
main()