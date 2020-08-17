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
# I got the following result.
#
#   weight decay | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^0 |     0.3371 |     0.8990 |    0.9049
#          10^-1 |     0.2932 |     0.9031 |    0.9089
#          10^-2 |     0.2896 |     0.9034 |    0.9092
#          10^-3 |     0.2893 |     0.9034 |    0.9091
#          10^-4 |     0.2893 |     0.9034 |    0.9091
#          10^-5 |     0.2893 |     0.9034 |    0.9091
#
# Here lower weight decay tends to be better.


# 2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?
# I got the following result.
#
#     activation | final loss | train acc. | test acc.
# -----------------------------------------------------
#      LOSTISTIC |     0.3917 |     0.8893 |    0.8949
#           RELU |     0.2980 |     0.9131 |    0.9162
#          LRELU |     0.2985 |     0.9134 |    0.9164
#
# RELU and LRELU are significantly better than LOGISTIC for the fixed learning rate, whereas there seems no big difference between RELU and LRELU.


# 2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?
# I got the following result.
#
# ********************** LOGISTIC *********************
#  learning rate | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^1 |     1.3944 |     0.4247 |    0.4248
#           10^0 |     0.1501 |     0.9541 |    0.9485
#          10^-1 |     0.1722 |     0.9449 |    0.9445
#          10^-2 |     0.3917 |     0.8893 |    0.8949
#
# ************************ RELU ***********************
#  learning rate | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^0 |        nan |     0.0992 |    0.1009
#          10^-1 |     3.0858 |     0.4036 |    0.4035
#          10^-2 |     0.2980 |     0.9131 |    0.9162
#          10^-3 |     0.6582 |     0.8430 |    0.8518
#
# *********************** LRELU ***********************
#  learning rate | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^0 |        nan |     0.0991 |    0.1009
#          10^-1 |     0.4912 |     0.8578 |    0.8650
#          10^-2 |     0.2985 |     0.9134 |    0.9164
#          10^-3 |     0.6562 |     0.8441 |    0.8522
#
# For LOGISTIC, lr=1 seems the best. For RELU and LRELU, lr=0.01 seems the best.


# NOTE: From here, we use LOGISTICS and set learning rate to 0.1.


# 2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?
# I got the following result.
#
#   weight decay | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^0 |     0.4784 |     0.8884 |    0.8936
#          10^-1 |     0.2072 |     0.9359 |    0.9359
#          10^-2 |     0.1747 |     0.9443 |    0.9434
#          10^-3 |     0.1724 |     0.9448 |    0.9444
#              0 |     0.1722 |     0.9449 |    0.9445
#
# Weight decay does not help. This is because our model is not very large. Indeed, testing accuracy is always as good as training accuracy, which means the model is rather underfitting.


# 2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?
# I got the following result.
#
#   weight decay | final loss | train acc. | test acc.
# -----------------------------------------------------
#           10^0 |     0.7695 |     0.8258 |    0.8339
#          10^-1 |     0.2723 |     0.9490 |    0.9495
#          10^-2 |     0.1501 |     0.9728 |    0.9647
#          10^-3 |     0.1458 |     0.9743 |    0.9652
#          10^-4 |     0.1453 |     0.9743 |    0.9650
#
# Weight decay does not really help although the model is slightly overfitting this time. Maybe we need bigger models to get it to work.


# 3.2.1 How well does your network perform on the CIFAR dataset?


