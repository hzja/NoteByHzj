Pandas<br />在用pandas进行数据处理时，同一个操作经常会重复很多次，由于这些常见的场景涉及到不同类型的数据集，因此本文更加侧重于展示和解释这些代码可以用于完成哪些工作，以便读者更加方便地使用它们。<br />在下面的代码片段中，数据清洗代码被封装在了一些函数中，代码的目的十分直观。可以直接使用这些代码，无需将它们嵌入到需要进行少量参数修改的函数中。
<a name="RGUK7"></a>
## 1、删除多列数据
```python
def drop_multiple_col(col_names_list, df): 
    '''
    AIM    -> Drop multiple columns based on their column names 

    INPUT  -> List of column names, df

    OUTPUT -> updated df with dropped columns 
    ------
    '''
    df.drop(col_names_list, axis=1, inplace=True)
    return df
```
有时，并不是所有列的数据都对数据分析工作有用。因此，`df.drop`可以方便地删掉选定的列。
<a name="iqBrW"></a>
## 2、转换 Dtypes
```python
def change_dtypes(col_int, col_float, df): 
    '''
    AIM    -> Changing dtypes to save memory

    INPUT  -> List of column names (int, float), df

    OUTPUT -> updated df with smaller memory  
    ------
    '''
    df[col_int] = df[col_int].astype('int32')
    df[col_float] = df[col_float].astype('float32')
```
当面对更大的数据集时，需要对「dtypes」进行转换，从而节省内存。如果有兴趣学习如何使用Pandas来处理大数据，强烈推荐阅读「Why and How to Use Pandas with Large Data」这篇文章（[https://towardsdatascience.com/why-and-how-to-use-pandas-with-large-data-9594dda2ea4c](https://towardsdatascience.com/why-and-how-to-use-pandas-with-large-data-9594dda2ea4c)）。
<a name="tgqKF"></a>
## 3、将分类变量转换为数值变量
```python
def convert_cat2num(df):
    # Convert categorical variable to numerical variable
    num_encode = {'col_1' : {'YES':1, 'NO':0},
                  'col_2'  : {'WON':1, 'LOSE':0, 'DRAW':0}}  
    df.replace(num_encode, inplace=True)  
```
有一些机器学习模型要求变量是以数值形式存在的。这时，就需要将分类变量转换成数值变量然后再将它们作为模型的输入。对于数据可视化任务来说，建议大家保留分类变量，从而让可视化结果有更明确的解释，便于理解。
<a name="VHo1A"></a>
## 4、检查缺失的数据
```python
def check_missing_data(df):
    # check for any missing data in the df (display in descending order)
    return df.isnull().sum().sort_values(ascending=False)
```
如果想要检查每一列中有多少缺失的数据，这可能是最快的方法。这种方法可以更清楚地知道哪些列有更多的缺失数据，以便决定接下来在数据清洗和数据分析工作中应该采取怎样的行动。
<a name="UJf29"></a>
## 5、删除列中的字符串
```python
def remove_col_str(df):
    # remove a portion of string in a dataframe column - col_1
    df['col_1'].replace('\n', '', regex=True, inplace=True)

    # remove all the characters after &# (including &#) for column - col_1
    df['col_1'].replace(' &#.*', '', regex=True, inplace=True)
```
有时可能会看到一行新的字符，或在字符串列中看到一些奇怪的符号。可以很容易地使用 `df['col_1'].replace` 来处理该问题，其中「col_1」是数据帧 df 中的一列。
<a name="TDcxZ"></a>
## 6、删除列中的空格
```python
def remove_col_white_space(df):
    # remove white space at the beginning of string 
    df[col] = df[col].str.lstrip()
```
当数据十分混乱时，很多意想不到的情况都会发生。在字符串的开头有一些空格是很常见的。因此，当想要删除列中字符串开头的空格时，这种方法很实用。
<a name="ynh2P"></a>
## 7、将两列字符串数据（在一定条件下）拼接起来
```python
def concat_col_str_condition(df):
    # concat 2 columns with strings if the last 3 letters of the first column are 'pil'
    mask = df['col_1'].str.endswith('pil', na=False)
    col_new = df[mask]['col_1'] + df[mask]['col_2']
    col_new.replace('pil', ' ', regex=True, inplace=True)  # replace the 'pil' with emtpy space
```
当希望在一定条件下将两列字符串数据组合在一起时，这种方法很有用。例如，希望当第一列以某些特定的字母结尾时，将第一列和第二列数据拼接在一起。根据需要，还可以在拼接工作完成后将结尾的字母删除掉。
<a name="VpR6j"></a>
## 8、转换时间戳（从字符串类型转换为日期「DateTime」格式）
```python
def convert_str_datetime(df): 
	'''
	AIM    -> Convert datetime(String) to datetime(format we want)
	
	INPUT  -> df
	
	OUTPUT -> updated df with new datetime format 
	------
	'''
    df.insert(loc=2, column='timestamp', value=pd.to_datetime(df.transdate, format='%Y-%m-%d %H:%M:%S.%f'))
```
在处理时间序列数据时，可能会遇到字符串格式的时间戳列。这意味着可能不得不将字符串格式的数据转换为根据需求指定的日期「datetime」格式，以便使用这些数据进行有意义的分析和展示。
