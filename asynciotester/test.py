import os, sys
import multiprocessing
import subprocess
import random

def worker(filepath):
	"""worker function"""
	p = subprocess.Popen(['./asynciotester', filepath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdout,stderr = p.communicate()

	result = stdout.strip()

	if 'ERROR' in result:
		print "*" * 100

	print result

	if p.returncode != 0:
		print 'Error processing'

def main(folderPath, count):

	filenames = os.listdir(folderPath)
	filepaths = []
	for name in filenames:
		path = os.path.join(folderPath, name)
		if not name.startswith('.') and os.path.isfile(path):
			filepaths.append(path)

	random.shuffle(filepaths)

	count = len(filepaths) if count > len(filepaths) else count

	jobs = []
	for i,filepath in enumerate(filepaths[:count]):
		p = multiprocessing.Process(target=worker, args=(filepath,))
		jobs.append(p)
		p.start()

if __name__ == '__main__':
	main(sys.argv[1], int(sys.argv[2]))