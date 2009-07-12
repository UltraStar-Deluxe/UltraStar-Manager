# python

import glob
import re

finder = re.compile("<!\[CDATA\[(.*?)\]\]>")
outfile = file(r"language.h", "w")

i = 1
   
for filename in glob.glob(r"config\*.xml"):
  xml = file(filename).read()
  for text in finder.findall(xml):
    print >> outfile, "#define TASKTEXT%i QObject::tr(\"%s\")" % (i, text.replace('"', r'\"'))
    i+=1
