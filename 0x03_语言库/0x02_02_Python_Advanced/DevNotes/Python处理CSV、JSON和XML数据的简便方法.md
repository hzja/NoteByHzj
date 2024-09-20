Python<br />Python的卓越灵活性和易用性使其成为最受欢迎的编程语言之一，尤其是对于数据处理和机器学习方面来说，其强大的数据处理库和算法库使得python成为入门数据科学的首选语言。在日常使用中，CSV，JSON和XML三种数据格式占据主导地位。下面针对三种数据格式来分享其快速处理的方法。
<a name="uRJxG"></a>
## CSV数据
CSV是存储数据的最常用方法。在Kaggle比赛的大部分数据都是以这种方式存储的。可以使用内置的Python csv库来读取和写入CSV。通常，会将数据读入列表。<br />看看下面的代码。当运行`csv.reader()`所有CSV数据变得可访问时。该`csvreader.next()`函数从CSV中读取一行; 每次调用它，它都会移动到下一行。也可以使用for循环遍历csv的每一行`for row in csvreader` 。确保每行中的列数相同，否则，在处理列表列表时，最终可能会遇到一些错误。
```python
import csv 

filename = "my_data.csv"

fields = [] 
rows = []   
# Reading csv file 
with open(filename, 'r') as csvfile: 
    # Creating a csv reader object 
    csvreader = csv.reader(csvfile) 

    # Extracting field names in the first row 
    fields = csvreader.next() 

    # Extracting each data row one by one 
    for row in csvreader: 
        rows.append(row)  
# Printing out the first 5 rows 
for row in rows[:5]: 
    print(row)
```
在Python中写入CSV同样容易。在单个列表中设置字段名称，并在列表列表中设置数据。这次创建一个`writer()`对象并使用它将数据写入文件，与读取时的方法基本一样。
```python
import csv 

# Field names 
fields = ['Name', 'Goals', 'Assists', 'Shots'] 

# Rows of data in the csv file 
rows = [ ['Emily', '12', '18', '112'], 
         ['Katie', '8', '24', '96'], 
         ['John', '16', '9', '101'], 
         ['Mike', '3', '14', '82']]

filename = "soccer.csv"

# Writing to csv file 
with open(filename, 'w+') as csvfile: 
    # Creating a csv writer object 
    csvwriter = csv.writer(csvfile) 

    # Writing the fields 
    csvwriter.writerow(fields) 

    # Writing the data rows 
    csvwriter.writerows(rows)
```
可以使用Pandas将CSV转换为快速单行的字典列表。将数据格式化为字典列表后，使用该`dicttoxml`库将其转换为XML格式。还将其保存为JSON文件！
```python
import pandas as pd
from dicttoxml import dicttoxml
import json

# Building our dataframe
data = {'Name': ['Emily', 'Katie', 'John', 'Mike'],
        'Goals': [12, 8, 16, 3],
        'Assists': [18, 24, 9, 14],
        'Shots': [112, 96, 101, 82]
        }

df = pd.DataFrame(data, columns=data.keys())

# Converting the dataframe to a dictionary
# Then save it to file
data_dict = df.to_dict(orient="records")
with open('output.json', "w+") as f:
    json.dump(data_dict, f, indent=4)

# Converting the dataframe to XML
# Then save it to file
xml_data = dicttoxml(data_dict).decode()
with open("output.xml", "w+") as f:
    f.write(xml_data)
```
<a name="cmv5W"></a>
## JSON数据
JSON提供了一种简洁且易于阅读的格式，它保持了字典式结构。就像CSV一样，Python有一个内置的JSON模块，使阅读和写作变得非常简单！以字典的形式读取CSV时，然后将该字典格式数据写入文件。
```python
import json
import pandas as pd

# Read the data from file
# We now have a Python dictionary
with open('data.json') as f:
    data_listofdict = json.load(f)

# We can do the same thing with pandas
data_df = pd.read_json('data.json', orient='records')

# We can write a dictionary to JSON like so
# Use 'indent' and 'sort_keys' to make the JSON
# file look nice
with open('new_data.json', 'w+') as json_file:
    json.dump(data_listofdict, json_file, indent=4, sort_keys=True)

# And again the same thing with pandas
export = data_df.to_json('new_data.json', orient='records')
```
正如之前看到的，一旦获得了数据，就可以通过`pandas`或使用内置的Python CSV模块轻松转换为CSV。转换为XML时，可以使用`dicttoxml`库。具体代码如下：
```python
import json
import pandas as pd
import csv

# Read the data from file
# We now have a Python dictionary
with open('data.json') as f:
    data_listofdict = json.load(f)

# Writing a list of dicts to CSV
keys = data_listofdict[0].keys()
with open('saved_data.csv', 'wb') as output_file:
    dict_writer = csv.DictWriter(output_file, keys)
    dict_writer.writeheader()
    dict_writer.writerows(data_listofdict)
```
<a name="wEUcP"></a>
## XML数据
XML与CSV和JSON有点不同。CSV和JSON由于其既简单又快速，可以方便阅读，编写和解释。而XML占用更多的内存空间，传送和储存需要更大的带宽，更多存储空间和更久的运行时间。但是XML也有一些基于JSON和CSV的额外功能：可以使用命名空间来构建和共享结构标准，更好地传承，以及使用XML、DTD等数据表示的行业标准化方法。<br />要读入XML数据，使用Python的内置XML模块和子模`ElementTree`。可以使用`xmltodict`库将`ElementTree`对象转换为字典。一旦有了字典，就可以转换为CSV，JSON或Pandas Dataframe！具体代码如下：
```python
import xml.etree.ElementTree as ET
import xmltodict
import json

tree = ET.parse('output.xml')
xml_data = tree.getroot()
xmlstr = ET.tostring(xml_data, encoding='utf8', method='xml')
data_dict = dict(xmltodict.parse(xmlstr))
print(data_dict)
with open('new_data_2.json', 'w+') as json_file:
    json.dump(data_dict, json_file, indent=4, sort_keys=True)
```
