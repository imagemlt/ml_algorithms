import os
from PIL import Image
import time
from numpy import array,exp
from random import random


class ANN:
	def __init__(self,trainPath,verifyPath,testPath):
		self.trainPath = trainPath
		self.verifyPath = verifyPath
		self.testPath = testPath
	
	def sigmoid(self,z):
		return 1.0 / (1.0 + exp(-z))
		
	def build(self,inNum,hidNum,outNum):
		# node number
		self.inNum = inNum + 1
		self.hidNum = hidNum + 1
		self.outNum = outNum
		# node outputs
		self.hidOutput = [0.0]*self.hidNum
		self.hidOutput[0] = 1.0
		self.outOutput = [0.0]*self.outNum
		# node outputs error
		self.hidOutputError = [0.0]*self.hidNum
		self.outOutputError = [0.0]*self.outNum
		# weights matrix
		self.wIn2Hid = [[0.0]*self.hidNum for i in range(self.inNum)]
		self.wHid2Out = [[random()/10]*self.outNum for j in range(self.hidNum)]
		
	def feedForward(self):
		for j in range(1,self.hidNum):
			sum = 0.0
			for i in range(self.inNum):
				sum += self.inOutput[i]*self.wIn2Hid[i][j]
			self.hidOutput[j] = self.sigmoid(sum)
		
		for k in range(self.outNum):
			sum = 0.0
			for j in range(self.hidNum):
				sum += self.hidOutput[j] * self.wHid2Out[j][k]
			self.outOutput[k] = self.sigmoid(sum)
			
	def backPropagate(self,N=0.3):
		# calculate errors for output
		for k in range(self.outNum):
			error = self.targets[k]-self.outOutput[k]
			self.outOutputError[k] = self.outOutput[k]*(1-self.outOutput[k])*error
		
		# calculate errors for hidden layer
		for j in range(self.hidNum):
			error = 0.0
			for k in range(self.outNum):
				error += self.outOutputError[k]*self.wHid2Out[j][k]
			self.hidOutputError[j] = self.hidOutput[j]*(1-self.hidOutput[j])*error
		
		# update hid-output weights
		for j in range(self.hidNum):
			for k in range(self.outNum):
				change = N*self.outOutputError[k]*self.hidOutput[j]
				#print(change)
				self.wHid2Out[j][k] += change
				
		# update input-hid weights
		for i in range(self.inNum):
			for j in range(1,self.hidNum):
				change = N*self.hidOutputError[j]*self.inOutput[i]
				self.wIn2Hid[i][j] += change
	
	def readPgm(self,file):
		im = Image.open(file)
		pixels = array(im.getdata())
		pixels = pixels.reshape(1,-1)/255.0
		pixels = pixels.tolist()[0]
		pixels.insert(0,1.0)
		return pixels
	
	def train(self,loop=500):
		diremap = {'straight':0,'right':1,'left':2,'up':3}
		best = 0.0
		tc = 0
		for l in range(loop):
			files=open(self.trainPath).readlines()
			for filename in files:
				filename=filename.strip()
				subname=filename.split('/')[-1]
				dire= subname.split('_')[1]
				self.targets=[0.1]*self.outNum
				self.targets[diremap[dire]] = 0.9
				self.inOutput = self.readPgm(filename)
				self.feedForward()
				self.backPropagate()
				tc+=1

				if tc % 60 == 0:
					v=self.verify()
					if v>best:
						best = v
						self.bestWIn2Hid = self.wIn2Hid
						self.bestWHid2Out = self.wHid2Out
					print(v,best)
		
		self.wIn2Hid = self.bestWIn2Hid
		self.wHid2Out = self.bestWHid2Out
	
	def verify(self):
		right = 0
		wrong = 0
		diremap = {'straight':0,'right':1,'left':2,'up':3}
		files=open(self.verifyPath).readlines()
		for file in files:
			filename=file.strip()
			subname=filename.split('/')[-1]
			dire=subname.split('_')[1]
			rightanswer = diremap[dire]

			self.inOutput = self.readPgm(filename)
			self.feedForward()
			answer = array(self.outOutput).argmax()
			#print(answer)
			if answer == rightanswer:
				right += 1
			else:
				wrong += 1
		return 1.0*right/(right+wrong)
		
	def test(self):
		right = 0
		wrong = 0
		diremap = {'straight':0,'right':1,'left':2,'up':3}
		files=open(self.testPath).readlines()
		for file in files:
			filename=file.strip()
			subname=filename.split('/')[-1]
			dire=subname.split('_')[1]
			rightanswer = diremap[dire]

			self.inOutput = self.readPgm(filename)
			self.feedForward()
			answer = array(self.outOutput).argmax()
			#print(answer);
			if answer == rightanswer:
				right += 1
			else:
				wrong += 1

		p = 1.0*right/(right+wrong)
		print('Test',p)
		
	
		
				
if __name__=='__main__':
	trainPath = '../train/all_train.list'
	verifyPath = '../train/all_test1.list'
	testPath = '../train/all_test2.list'
	
	
	TIMEFORMAT='%Y-%m-%d %X'
	print(time.strftime(TIMEFORMAT,time.localtime(time.time())))
	myANN = ANN(trainPath,verifyPath,testPath)
	myANN.build(960,3,4)
	myANN.train(loop=100)
	myANN.test()
	print(time.strftime(TIMEFORMAT,time.localtime(time.time())))
