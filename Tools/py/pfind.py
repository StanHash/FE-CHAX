# Module providing pointer-finding facilities
# Used by pfinder & c2ea

def pointer_iter(romFileName, value):
	target = value.to_bytes(4, 'little')

	with open(romFileName, 'rb') as rom:
		offset = 0

		while True:
			word = rom.read(4)

			if word == b'':
				break
			
			if word == target:
				yield offset
			
			offset += 4
