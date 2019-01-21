import math
row = []
with open("file.txt", 'r') as file:#assuming file name is file.txt
    for line in file:
        innerList = [elt.strip() for elt in line.split(" ")]
        row.append(innerList)

k = 1.2
b = 0.75
DocFreqWord1 = 0 #number of documents that contain word1
DocFreqWord2 = 0 #number of documents that contain word2
inverseDocFrequency = 0

N = len(row)

documentLength = []
x = 0
y = 1
for i in range(N):
    documentLength.append(int(row[x][y]))
    x += 1

L = sum(documentLength) / N #average length of all documents

word1Frequency = []
word2Frequency = []

x = 0
y = 2

for i in range(N):
    if int(row[x][y] > 1):
        DocFreqWord1 += 1
    word1Frequency.append(int(row[x][y]))
    x += 1

x = 0
y = 3

for i in range(N):
    if int(row[x][y] > 1):
        DocFreqWord2 += 1
    word2Frequency.append(int(row[x][y]))
    x += 1

def bm25(val1, val2):
    termFreqW1 = val1 #number of times word appears in document
    docLength = val2
    
    inverseDocFrequency = (N - DocFreqWord1 + 0.5) / (DocFreqWord1 + 0.5)
    math.log(inverseDocFrequency)
    
    tfi = (termFreqW1 * (k + 1)) / (termFreqW1 + k * (1 - b + b * (docLength / L)))
    
    result = inverseDocFrequency * tfi
    
    return result

def bm25a(val1, val2):
    termFreqW2 = val1 #number of times word appears in document
    docLength = val2
    
    inverseDocFrequency = (N - DocFreqWord2 + 0.5) / (DocFreqWord2 + 0.5)
    math.log(inverseDocFrequency)
    
    tfi = (termFreqW2 * (k + 1)) / (termFreqW2 + k * (1 - b + b * (docLength / L)))
    
    result = inverseDocFrequency * tfi
    
    return result

print(" ")

count = N
a = 0
doc = 1
while(count > 0):
    termFreqW1 = word1Frequency[a]#times word1 appears in document a
    termFreqW2 = word2Frequency[a]#times word2 appears in document a
    d = documentLength[a]
    
    bm25Word1 = bm25(termFreqW1, d)
    bm25Word2 = bm25a(termFreqW2, d)
    
    totalbm25 = bm25Word1 + bm25Word2
    
    print("BM25 score for Document: ", doc, "is: ", totalbm25)
    
    a += 1
    doc+= 1
    count -= 1