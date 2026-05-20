# python

from __future__ import absolute_import
from __future__ import print_function
import glob
import re

finder = re.compile(r"<!\[CDATA\[(.*?)\]\]>")
outfile = open(r"language.h", "w")

i = 1

for filename in glob.glob(r"config/*.xml"):
  xml = open(filename).read()
  for text in finder.findall(xml):
    print("#define TASKTEXT%i QObject::tr(\"%s\")" % (i, text.replace('"', r'\"')), file=outfile)
    i+=1
