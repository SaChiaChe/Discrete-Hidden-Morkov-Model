# Discrete Hidden Morkov Model

Implementing discrete HMM.

## How to run

### Complie
Makefile is included, thus, simply type
```
make
```
Or you could do it in the hard way
```
gcc train.c -o train
gcc test.c -o test
```

### Training
Iteration is how much iterations you want, for this train data, the model converges at around 800 iterations.<br>
InitModelPath is the initial model, there is a intial model at TrainData/model_init.txt<br>
TrainDataPath is where the train data is, it is at TrainData/seq_model_0X.txt, there are 5 training datas, and should train 5 different models.<br>
OutputModelPath is where the final model should be saved at.
```
./train Iteration InitModelPath TrainDataPath OutputModelPath
```

### Testing
ModelListPath is where the model list is, and it should contain the path of the 5 trained models you want to test.<br>
TestDataPath is where the test data is, it is at TestData/testing_dataX.txt, the first testing data has answersm and the second doesn't.<br>
OutputResultPath is where the result should be at, the result should contain the best model and it's probalility for every inpt in a line.
```
./test ModelListPath TestDataPath OutputResultPath
```

### Accuracy
ResultFilePath is the file ouput by the previous test program.<br>
AnswerFilePath is where the answers are at, it is at TestData/testing_answer.txt
*If you choose weird file names, CalAcc.py might fail, since it it comparing the model names, but it is fairy easy to adjust, so it's up to you to de the work!*
```
python CalAcc.py ResultFilePath AnswerFilePath
```

## Other Stuff
/Models contains the models trained for 1000 iterations(Not sure thou)<br>
/Result contains a lot of results for models that ran for iterations ranging from 1 to 2000<br>
/TestArea contains all the models that ran for iterations ranging from 1 to 2000

## Built With

* Python 3.6.0 :: Anaconda custom (64-bit)

## Authors

* **SaKaTetsu** - *Initial work* - [SaKaTetsu](https://github.com/SaKaTetsu)