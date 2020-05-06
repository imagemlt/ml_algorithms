from PIL import Image
from numpy import array

def readPgm(file):
	im = Image.open(file)
	pixels = array(im.getdata())
	pixels = pixels.reshape(1,-1)/255.0
	pixels = pixels.tolist()[0]
	pixels.insert(0,1.0)
	return pixels

trainpath="../train/all_train.list"
testpath="../train/all_test1.list"
verifypath="../train/all_test2.list"

trainfiles=open(trainpath).readlines()
testfiles=open(testpath).readlines()
verifyfiles=open(verifypath).readlines()

for file in trainfiles:
    filename=file.strip()
    subname=filename.split('/')[-1]
    print(subname)
    f=open('train/'+subname+".data",'w')
    for pixel in readPgm(filename):
        f.write(str(pixel)+" ")
    f.close()

for file in testfiles:
    filename=file.strip()
    subname=filename.split('/')[-1]
    print(subname)
    f=open('test/'+subname+".data",'w')
    for pixel in readPgm(filename):
        f.write(str(pixel)+" ")
    f.close()

for file in verifyfiles:
    filename=file.strip()
    subname=filename.split('/')[-1]
    print(subname)
    f=open('verify/'+subname+".data",'w')
    for pixel in readPgm(filename):
        f.write(str(pixel)+" ")
    f.close() 