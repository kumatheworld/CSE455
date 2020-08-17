from uwimg import *

def softmax_model(inputs, outputs):
    l = [make_layer(inputs, outputs, SOFTMAX)]
    return make_model(l)

def neural_net(inputs, outputs):
    print(inputs)
    l = [   make_layer(inputs, 32, LOGISTIC),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

print("loading data...")
train = load_classification_data("mnist.train", "mnist.labels", 1)
test  = load_classification_data("mnist.test", "mnist.labels", 1)
print("done")
print

print("training model...")
batch = 128
iters = 1000
rate = .01
momentum = .9
decay = .0

m = softmax_model(train.X.cols, train.y.cols)
train_model(m, train, batch, iters, rate, momentum, decay)
print("done")
print

print("evaluating model...")
print("training accuracy: %f", accuracy_model(m, train))
print("test accuracy:     %f", accuracy_model(m, test))


## Questions ##
# NOTE: During the following experiments, the hyperparameters including activation functions are fixed to the default values unless they are explicitly mentioned in each question.

# 2.2.1 Why might we be interested in both training accuracy and testing accuracy? What do these two numbers tell us about our current model?
# Training accuracy tells us how well a model fits the training data. By looking at training accuracy, we can figure out how good a model can potentially be on the given task. On the other hand, testing accuracy tells us how well a model can perform on unseen data. In most cases we are interested in how good predictions our model can make on real world data, which tend to be unavailable during training time.


# 2.2.2 Try varying the model parameter for learning rate to different powers of 10 (i.e. 10^1, 10^0, 10^-1, 10^-2, 10^-3) and training the model. What patterns do you see and how does the choice of learning rate affect both the loss during training and the final model accuracy?
# I got the following result.
#
#  learning rate | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^1 |        nan |     0.0992 |    0.1009
#           10^0 |     0.5915 |     0.8506 |    0.8463
#          10^-1 |     0.2085 |     0.9207 |    0.9171
#          10^-2 |     0.2893 |     0.9034 |    0.9091
#          10^-3 |     0.5371 |     0.8590 |    0.8669
#
# Overall, testing accuracy is positively correlated with training accuracy and negatively correlated with final loss. When learning rate is too high, the loss value fluctuates a lot and does not become as low as it should be, sometimes ending up producing NaNs. On the other hand, when learning rate is too low, loss does not decrease as much. For our model, lr=10^-1 or 10^-2 seems the best.


# 2.2.3 Try varying the parameter for weight decay to different powers of 10: (10^0, 10^-1, 10^-2, 10^-3, 10^-4, 10^-5). How does weight decay affect the final model training and test accuracy?


# 2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?


# 2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?


# 2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?


# 2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?


# 3.2.1 How well does your network perform on the CIFAR dataset?


