import matplotlib.pyplot as plt
import pdb
import argparse
import os.path

def argParser():
	parser = argparse.ArgumentParser(description='PyTorch Plot Progress')
	parser.add_argument('--file_name', default='')
	return parser.parse_args()


def main():
	args = argParser()
	train_accuracy=[]
	test_accuracy=[]
	train_loss=[]
	with open(args.file_name) as f:
		for line in f:
			if 'Final Summary' in line:
				train_loss.append(float(line[:-1].split(' ')[-1]))
			elif 'Train Accuracy of the network' in line:
				train_accuracy.append(float(line[:-1].split(' ')[-2]))
			elif 'Test Accuracy of the network' in line:
				test_accuracy.append(float(line[:-1].split(' ')[-2]))

	figures_dir = 'report/figures'
	log_id = os.path.splitext(os.path.basename(args.file_name))[0]

	plt.plot(train_accuracy, label='train')
	plt.plot(test_accuracy, label='test')
	plt.ylim(0, 100)
	plt.legend()
	plt.title('Accuracy')
	plt.savefig(os.path.join(figures_dir, f'accuracies_{log_id}.png'))

	plt.clf()
	plt.plot(train_loss)
	plt.ylim(0, 2.3)
	plt.title('Training loss')
	plt.savefig(os.path.join(figures_dir, f'loss_{log_id}.png'))



if __name__ == '__main__':
	main()