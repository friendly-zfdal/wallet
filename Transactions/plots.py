
import matplotlib.pyplot as plt

file=open('./graphics.txt')

line=file.readline()
counter=0
expanse=0
income=0
categories=[]
percents=[]
while line:

    counter=counter+1
    if counter==1:
        income=float(line)
        line=file.readline()
        continue
    if counter==2:
        expense=float(line)
        line=file.readline()
        continue

    if line=='\n':
        continue
    categories.append(line.split(':')[0].split('(')[0])
    percents.append(float(line.split(':')[1]))

    line=file.readline()

file.close()

fig, ax=plt.subplots()
ax.pie(percents, labels=categories)
ax.axis("equal")

fig, ax=plt.subplots()
ax.bar(['income','expense'],[income,expense])
plt.show()


    