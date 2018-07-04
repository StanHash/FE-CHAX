import sys, csv, os
import nightmare, pfind

def show_exception_and_exit(exc_type, exc_value, tb):
	import traceback
	
	traceback.print_exception(exc_type, exc_value, tb)
	sys.exit(-1)

def getarglength(nmmentry):
	"""takes the nmm entry object and returns the appropriate EA marker"""
	
	if (nmmentry.length == 4) & (nmmentry.offset % 4 == 0):
		return "WORD "
	
	elif (nmmentry.length == 2) & (nmmentry.offset % 2 == 0):
		return "SHORT "
	
	else:
		return "BYTE "

def processed_lines(csvName, romName):
	"""Takes a csv and generates corresponding event lines. Requires a nmm with the same name in the same folder."""

	nmmName = csvName.replace(".csv", ".nmm")
	macroName = "_C2EA_{0}_ENTRY".format(csvName.split('.')[0].replace(os.path.sep, "_"))

	nmm = nightmare.NightmareTable(nmmName)

	codeCurrent = '' # because we start with BYTE
	macroArgs = []
	macroDef = ''

	for i in range(nmm.colNum):
		argString = "_ARG{0:03d}".format(i)
		macroArgs.append(argString)

		codeArg = getarglength(nmm.columns[i]) # this gets the appropriate length string.

		if codeArg!=codeCurrent: # only append if needed else just add arg.
			if codeCurrent != '': # this should only be on the first line
				macroDef += ';'
			
			# assuming codeArg does not equal codeCurrent
			macroDef += (codeArg + argString + ' ')
			codeCurrent = codeArg
		
		else:
			macroDef += (argString + ' ')

	yield '#define {0}({1}) "{2}"\n'.format(macroName, ",".join(macroArgs), macroDef)

	with open(csvName, 'r') as myfile:
		table = csv.reader(myfile)

		offsetCell = next(table)[0]

		inline = False
		repoint = False

		tableName = ''

		if offsetCell.strip()[0:6]=="INLINE":
			tableName = offsetCell[6:].strip()

			inline = True

			if tableName[0:5] == "ALONE":
				tableName = tableName[3:].strip()
				repoint = True

			yield '{0}:\n'.format(tableName)

		else:
			yield "PUSH\n"
			yield "ORG {0}\n".format(offsetCell)
		
		for row in table:
			line = "{0}(".format(macroName)

			for entry, data in zip(nmm.columns, row[1:]):
				thisentry = ''
				# output.extend(int(data, 0).to_bytes(entry.length, 'little', signed=entry.signed))
				
				if data == '':
					sys.exit("ERROR: `{0}` contains a blank cell!".format(csvName))
				
				try:
					arglen = getarglength(entry)
					
					if (arglen == "WORD ") | (arglen == "SHORT "):
						line += data + ','
					
					else:
						dt = int(data, 0).to_bytes(entry.length, 'little', signed = entry.signed) # this is a string i guess
						
						for byte in dt:
							thisentry += (hex(byte) + ' ')
						
						line += thisentry[:-1] + ','
				
				except ValueError: # if it's not a number, just add it directly
					line += (data + ',')
			
			yield line[:-1] + ')\n'
		
		if inline and repoint:
			yield "PUSH\n"

			for offset in pfind.pointer_iter(romName, 0x8000000 | nmm.offset):
				yield "ORG {0}\nPOIN {1}\n".format(hex(offset), tableName)

		if not repoint:
			yield "POP\n"

def main():
	sys.excepthook = show_exception_and_exit
	
	if len(sys.argv) < 4:
		sys.exit("Usage: {0} <CSV File> <Output Event File> <Reference ROM>".format(sys.argv[0]))

	if not os.path.exists(sys.argv[1]):
		sys.exit("File `{0}` doesn't exist".format(sys.argv[1]))

	if not os.path.exists(sys.argv[3]):
		sys.exit("File `{0}` doesn't exist".format(sys.argv[3]))

	with open(sys.argv[2], 'w') as f:
		f.writelines(processed_lines(sys.argv[1], sys.argv[3]))

if __name__ == '__main__':
	main()
