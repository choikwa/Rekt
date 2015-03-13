
filein = open("Lexemes.txt", 'r')
idToNameFO = open("idToNameMap.txt", 'w')
nameToIdFO = open("nameToIdMap.txt", 'w')

lines = filein.readlines()
filein.close()
for line in lines:
  name = line[:-2]  #strip comma and newline
  tmp1 = "{" + name + ",\"" + name + "\"},\n"
  tmp2 = "{\"" + name + "\"," + name + "},\n"
  idToNameFO.write(tmp1)
  nameToIdFO.write(tmp2)
idToNameFO.close()
nameToIdFO.close()
