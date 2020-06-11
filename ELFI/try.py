f=open("progress.txt","r+")

SectionsCount=7
SectionsCount=hex( int(SectionsCount, 16) )

SentDataBlocks = 6
lines=f.readlines()
lines[0] = str(int(SectionsCount,16)) +" "+ str(SentDataBlocks) 
lines[1]="Erase_Done"
f.seek(0)
for i in range(2):
  f.write(lines[i]) 
f.close()