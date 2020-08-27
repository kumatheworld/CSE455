import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os
import datetime
import pdb
import time
import torchvision.models as torchmodels
from loss import MSELoss4Classification

class BaseModel(nn.Module):
    def __init__(self):
        super(BaseModel, self).__init__()
        if not os.path.exists('logs'):
            os.makedirs('logs')
        ts = time.time()
        st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d_%H:%M:%S_log.txt')
        self.logFile = open('logs/' + st, 'w')

        self.input_size = 3 * 32 * 32
        self.output_size = 10

    def log(self, str):
        print(str)
        self.logFile.write(str + '\n')

    def criterion(self, loss):
        if loss == 'cross_entropy':
            return nn.CrossEntropyLoss()
        elif loss == 'mse':
            return MSELoss4Classification
        else:
            raise Exception(f"Loss function '{loss}' not implemented!")

    def optimizer(self):
        return optim.SGD(self.parameters(), lr=0.001)

    def adjust_learning_rate(self, optimizer, epoch, args):
        lr = args.lr  # TODO: Implement decreasing learning rate's rules
        lr *= 0.9 ** (epoch // 50)
        for param_group in optimizer.param_groups:
            param_group['lr'] = lr



class LazyNet(BaseModel):
    def __init__(self):
        super(LazyNet, self).__init__()
        # TODO: Define model here
        self.layer = nn.Sequential(
            nn.Flatten(),
            nn.Linear(self.input_size, self.output_size)
        )

    def forward(self, x):
        # TODO: Implement forward pass for LazyNet
        y1 = self.layer(x)
        return y1


class BoringNet(BaseModel):
    def __init__(self):
        super(BoringNet, self).__init__()
        # TODO: Define model here
        self.hidden_sizes = [120, 84]
        self.layers = nn.Sequential(
            nn.Flatten(),
            nn.Linear(self.input_size, self.hidden_sizes[0]),
            nn.ReLU(inplace=True),
            nn.Linear(self.hidden_sizes[0], self.hidden_sizes[1]),
            nn.ReLU(inplace=True),
            nn.Linear(self.hidden_sizes[1], self.output_size)
        )

    def forward(self, x):
        # TODO: Implement forward pass for BoringNet
        y = self.layers(x)
        return y


class CoolNet(BaseModel):
    def __init__(self):
        super(CoolNet, self).__init__()
        # TODO: Define model here
        self.channel_sizes = [32, 64]
        self.feature_size = 32 // 4 - 3
        self.hidden_size = 128
        self.layers = nn.Sequential(
            nn.Conv2d(3, self.channel_sizes[0], 5),
            nn.BatchNorm2d(self.channel_sizes[0]),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(2, 2),
            nn.Conv2d(self.channel_sizes[0], self.channel_sizes[1], 5),
            nn.BatchNorm2d(self.channel_sizes[1]),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(2, 2),
            nn.Flatten(),
            nn.Linear(self.channel_sizes[1] * self.feature_size * self.feature_size, self.hidden_size),
            nn.BatchNorm1d(self.hidden_size),
            nn.ReLU(inplace=True),
            nn.Linear(self.hidden_size, self.output_size)
        )

    def forward(self, x):
        # TODO: Implement forward pass for CoolNet
        y = self.layers(x)
        return y
