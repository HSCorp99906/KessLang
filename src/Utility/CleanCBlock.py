import sys

cleanedFile = None

with open(sys.argv[1] + "/" + sys.argv[2], 'r') as src:
    cleanedFile = src.read().replace("______END______;", "")

src.close()

cleanedFile = cleanedFile.replace("__EMPTY__", "")

with open(sys.argv[1] + "/" + sys.argv[2], "w") as src:
    src.write(cleanedFile)
