import torch
import torchvision
import torchvision.transforms as transforms


class CifarLoader(object):
	"""docstring for CifarLoader"""
	def __init__(self, args):
		super(CifarLoader, self).__init__()
		transform = transforms.Compose(
		    [
		     # TODO: Add data augmentations here
		     transforms.ToTensor(),
		     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
		     ])

		transform_test = transforms.Compose([
		    transforms.ToTensor(),
		    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
		])

		use_cuda = args.cuda and torch.cuda.is_available()
		kwargs = {'num_workers': 1, 'pin_memory': True} if use_cuda else {}

		trainset = torchvision.datasets.CIFAR10(root='./data', train=True,
		                                        download=True, transform=transform)
		self.trainloader = torch.utils.data.DataLoader(trainset, batch_size=args.batchSize,
		                                          shuffle=True, **kwargs)

		testset = torchvision.datasets.CIFAR10(root='./data', train=False,
		                                       download=True, transform=transform_test)
		self.testloader = torch.utils.data.DataLoader(testset, batch_size=args.batchSize,
		                                         shuffle=False, **kwargs)

		self.classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')
