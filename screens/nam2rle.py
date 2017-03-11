
import sys

filename = sys.argv[1]
screenFile = open (filename+".txt", "r") 
data=screenFile.readlines()
nametableFile = open (filename+".nam", "wb")
rleHeaderFile = open (filename+".h", "w")

values = []
for i in range (len(data)):
	line = data[i]
	items=line.split(" ")
	'''print(len(items))'''
	for j in range (32):
		values.append(items[j])
print('step 1 OK', len(values))
#Generate nametable
nametablebytes = []
for i in range(len(values)): 
	print(i, values[i][2:])
	aByte = bytes.fromhex(values[i][2:])


	nametablebytes.append(aByte)
	nametableFile.write(aByte)
print('step 2 OK')

#Generate RLE
headerLen = 1
#
rleContent = ""
print("len(nametable)",len(nametablebytes))
aByte = nametablebytes[0]
j=0 
k=0
for i in range (0, len(nametablebytes)):
	print(i,j, aByte)
	if (nametablebytes[i]!=aByte):
		if (j==1):
			rleContent += "0x"+aByte.hex()+"," 
			headerLen +=1
			j=0
			k+=1
		else:
			print("Rupture multiple sur",nametablebytes[i],aByte,"repeat",j)
			rleContent += "\n"
			while (j>255):
				rleContent += "0x"+aByte.hex()+","
				rleContent += "0x01,0xfe,\n"
				headerLen +=3
				j -= 255
				k+=255
			if (j==1):
				rleContent += "0x"+aByte.hex()+","
				headerLen +=1
				j=0
				k+=1
			else:
				rleContent += "0x"+aByte.hex()+","
				rleContent += "0x01,"
				if (j-1) < 15:
					print("ici1",j)
					rleContent += "0x0"+"{}".format(j-1)+",\n"
					print("0x0"+"{}".format(j-1)+",\n")
				elif (j-1 == 15):
					print("-------------------->ici1",j)
					rleContent += "0x0f"+",\n"
				else:
					print("ici2",j)
					rleContent += "{}".format(hex(j-1))+",\n"
					print("{}".format(hex(j-1))+",\n")
				headerLen +=3
				k+=j
				j=0


			
		aByte = nametablebytes[i]
	j+=1

##########
print("fin du for",j,k,aByte,headerLen)

if (k!=1024):
	j=1024-k
	print("RELIQUAT", aByte)
	while (j>255):

		rleContent += "0x"+aByte.hex()+","
		rleContent += "0x01,0xfe,\n"
		headerLen +=3
		k -= 255
	if (j==1):
		rleContent += "0x"+aByte.hex()+","
		headerLen +=1
	else:
		rleContent += "0x"+aByte.hex()+","
		rleContent += "0x01,"
		if (j-1) < 15:
			rleContent += "0x0"+"{}".format(j-1)+",\n"
		else:
			rleContent += "{}".format(hex(j-1))+",\n"
		headerLen +=3
###########
headerLen+=2
print("FIN", j)


#
rleHeaderFile.write("const unsigned char {}_rle[{}]=\n".format(filename, headerLen))
rleHeaderFile.write("{\n");
rleHeaderFile.write("0x01,\n")

rleHeaderFile.write("{}".format(rleContent));

rleHeaderFile.write("\n0x01,0x00\n")
rleHeaderFile.write("};\n");
#"const unsigned char gameover_nam[{0}]={};".format(0=headerLen)

screenFile.close()
nametableFile.close()
rleHeaderFile.close()
