#from subprocess import call
import os

#N_frames = 1000

for (dirpath, dirnames, filenames) in os.walk(os.getcwd()):
	for i in filenames:
		if i[-4:] == ".dat":


			i_s = i.split(".")[0]
			filename = i
			outname = i_s + ".jpg"

			call_txt = "gnuplot -e \"in='{0}'; out='{1}'; title='t={2}'\" gnuplotter.gp".format(filename, outname, i_s)

			#print(call_txt)
			os.system(call_txt)

	
	
