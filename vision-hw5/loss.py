import torch
import torch.nn.functional as F

def MSELoss4Classification(x, y):
    pred = F.softmax(x, dim=-1)
    y_onehot = torch.zeros_like(x)
    y_onehot[torch.arange(len(y)), y] = 1
    loss = F.mse_loss(pred, y_onehot)
    return loss
