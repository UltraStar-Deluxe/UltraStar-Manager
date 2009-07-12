# python

import glob
import re

finder = re.compile("<!\[CDATA\[(.*?)\]\]>")
outfile_audiotag = file(r"audiotag\language.h", "w")
outfile_rename = file(r"rename\language.h", "w")

i = 1
   
for filename in glob.glob(r"audiotag\config\*.xml"):
    xml = file(filename).read()
    for text in finder.findall(xml):
		print >> outfile_audiotag, "#define TASKTEXT%i QObject::tr(\"%s\")" % (i, text.replace('"', r'\"'))
        i+=1

i = 1

for filename in glob.glob(r"rename\config\*.xml"):
	xml = file(filename).read()
	for text in finder.findall(xml):
		print >> outfile_rename, "#define TASKTEXT%i QObject::tr(\"%s\")" % (i, text.replace('"', r'\"'))
		i+=1
