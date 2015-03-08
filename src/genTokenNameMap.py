
filein = open("Lexemes.txt", 'r')
fileout = open("LexemeNameMap.hpp", 'w')

lines = filein.readlines()
filein.close()
for line in lines:
  name = line[:-2]  #strip comma and newline
  tmp = "{" + name + ", \"" + name + "\"},\n"
  fileout.write(tmp)
fileout.close()
