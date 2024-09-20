PyTorchXGBoost<br />在大规模数据集进行读取进行训练的过程中，迭代读取数据集是一个非常合适的选择，在Pytorch中支持迭代读取的方式。接下来将介绍XGBoost的迭代读取的方式。
<a name="SFlya"></a>
## **内存数据读取**
```python
class IterLoadForDMatrix(xgb.core.DataIter):
    def __init__(self, df=None, features=None, target=None, batch_size=256 * 1024):
        self.features = features
        self.target = target
        self.df = df
        self.batch_size = batch_size
        self.batches = int(np.ceil(len(df) / self.batch_size))
        self.it = 0  # set iterator to 0
        super().__init__()

    def reset(self):
        '''Reset the iterator'''
        self.it = 0

    def next(self, input_data):
        '''Yield next batch of data.'''
        if self.it == self.batches:
            return 0  # Return 0 when there's no more batch.

        a = self.it * self.batch_size
        b = min((self.it + 1) * self.batch_size, len(self.df))
        dt = pd.DataFrame(self.df.iloc[a:b])
        input_data(data=dt[self.features], label=dt[self.target])  # , weight=dt['weight'])
        self.it += 1
        return 1
```
调用方法（此种方式比较适合GPU训练）：
```python
Xy_train = IterLoadForDMatrix(train.loc[train_idx], FEATURES, 'target')
dtrain = xgb.DeviceQuantileDMatrix(Xy_train, max_bin=256)
```
参考文档：[https://xgboost.readthedocs.io/en/latest/python/examples/quantile_data_iterator.html](https://xgboost.readthedocs.io/en/latest/python/examples/quantile_data_iterator.html)
<a name="yxNy8"></a>
## **外部数据迭代读取**
```python
class Iterator(xgboost.DataIter):
    def __init__(self, svm_file_paths: List[str]):
        self._file_paths = svm_file_paths
        self._it = 0
        super().__init__(cache_prefix=os.path.join(".", "cache"))

    def next(self, input_data: Callable):
        if self._it == len(self._file_paths):
            # return 0 to let XGBoost know this is the end of iteration
            return 0

        X, y = load_svmlight_file(self._file_paths[self._it])
        input_data(X, y)
        self._it += 1
        return 1

    def reset(self):
        """Reset the iterator to its beginning"""
        self._it = 0
```
调用方法（此种方式比较适合CPU训练）：
```python
it = Iterator(["file_0.svm", "file_1.svm", "file_2.svm"])
Xy = xgboost.DMatrix(it)

# Other tree methods including ``hist`` and ``gpu_hist`` also work, but has some caveats
# as noted in following sections.
booster = xgboost.train({"tree_method": "approx"}, Xy)
```
参考文档：[https://xgboost.readthedocs.io/en/stable/tutorials/external_memory.html](https://xgboost.readthedocs.io/en/stable/tutorials/external_memory.html)
