import os
import shutil
import sys

version_str = '  -D Version='  
file_in = 'platformio.ini'
file_out = 'platformio.ini.bak'
version = sys.argv[1]

with open(file_in, "rt") as fin:
    with open(file_out, "wt") as fout:
        for line in fin:
            if version_str in line:
              fout.write('{}\\"{}\\"\n'.format(version_str, version))
            else:
              fout.write(line)

shutil.move(file_out, file_in)