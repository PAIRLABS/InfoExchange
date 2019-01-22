if __name__ == "__main__":
	with open('spinExport_box.csv','r') as fp:
		lines = fp.readlines()
		for l in lines:
			if l=='---\n':
				lines.remove(l)
		new_lines = []
		for l in lines:
			new_lines.append(l.split('"')[1])
		for i in range( len(new_lines)/5 ):
			print("{}, {}, {}, {}, {}".format(new_lines[5*i],new_lines[5*i+1],new_lines[5*i+2],new_lines[5*i+3],new_lines[5*i+4]))
	fp.close()