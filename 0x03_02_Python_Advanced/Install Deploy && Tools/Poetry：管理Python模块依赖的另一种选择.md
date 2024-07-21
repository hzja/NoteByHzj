在项目的开始阶段，使用pip或conda等依赖性管理工具可能就足够了。<br />然而，随着项目的扩大，依赖关系的数量也在增加。这可能会使项目的环境难以重现，并且在仅仅依靠pip或conda进行依赖性管理时难以有效地维护它。<br />此时 Poetry 应运而生，它是一个开源库，提供了一个强大的工具来创建和维护具有一致环境的Python项目。在这里将深入探讨Poetry的优势，并强调它与pip和conda的主要区别。
<a name="zd1RS"></a>
## 安装的便利性
一键轻松安装过程，能够快速采用并将软件包集成到代码库中，从而节省时间和精力。
<a name="AkeCI"></a>
### Conda
Conda的安装格式对于不同的软件包是不一致的。例如，要安装polars，需要运行：
```bash
conda install -c conda-forge polar
```
而要安装pandas，命令是：
```bash
conda install -c anaconda pandas
```
<a name="T29Vh"></a>
### Pip
Pip对每个软件包都有一致的安装格式：
```bash
pip install package-name
```
<a name="nbVUh"></a>
### Poetry
Poetry也都遵循相同的安装格式：
```bash
poetry add package-name
```
<a name="vfonO"></a>
## 可用的packages
软件包选择空间大，使得找到最适合需求的特定软件包和版本更容易。
<a name="j7nQd"></a>
### Conda
有些软件包，如 "snscrape"，不能通过conda安装。此外，某些版本，如Pandas 2.0，可能无法通过Conda安装。<br />虽然可以在conda的虚拟环境中使用pip来解决软件包的限制，但conda不能跟踪用pip安装的依赖关系。
```bash
$ conda list
# packages in environment at /Users/khuyentran/miniconda3/envs/test-conda:
#
# Name                    Version                   Build  Channel
```
<a name="tVZgI"></a>
### Pip
Pip可以安装来自Python Package Index (PyPI)和其他存储库的任何软件包。
<a name="hAC2D"></a>
### Poetry
Poetry还可以安装来自Python Package Index (PyPI)和其他存储库的软件包。
<a name="L00cb"></a>
## 依赖关系的数量
减少环境中的依赖性，可以简化开发过程。
<a name="o1pP8"></a>
### Conda
Conda提供完全的环境隔离，同时管理Python软件包和系统级的依赖关系。与其他软件包管理器相比，这可能会导致软件包太大，在安装和分发时可能会消耗更多的存储空间。
```bash
$ conda install pandas

$ conda list          

# packages in environment at /Users/khuyentran/miniconda3/envs/test-conda:
#
# Name              Version         Build           Channel             
blas                1.0             openblas                          
bottleneck          1.3.5           py311ha0d4635_0                    
bzip2               1.0.8           h620ffc9_4                        
ca-certificates     2023.05.30      hca03da5_0                        
libcxx              14.0.6          h848a8c0_0                        
libffi              3.4.4           hca03da5_0                        
libgfortran         5.0.0           11_3_0_hca03da5_28                 
libgfortran5        11.3.0          h009349e_28                       
libopenblas         0.3.21          h269037a_0                        
llvm-openmp         14.0.6          hc6e5704_0                        
ncurses             6.4             h313beb8_0                        
numexpr             2.8.4           py311h6dc990b_1                    
numpy               1.24.3          py311hb57d4eb_0                    
numpy-base          1.24.3          py311h1d85a46_0                    
openssl             3.0.8           h1a28f6b_0                        
pandas              1.5.3           py311h6956b77_0                    
pip                 23.0.1          py311hca03da5_0                    
python              3.11.3          hb885b13_1                        
python-dateutil     2.8.2           pyhd3eb1b0_0                      
pytz                2022.7          py311hca03da5_0                    
readline            8.2             h1a28f6b_0                        
setuptools          67.8.0          py311hca03da5_0                    
six                 1.16.0          pyhd3eb1b0_1                      
sqlite              3.41.2          h80987f9_0                        
tk                  8.6.12          hb8d0fd4_0                        
tzdata              2023c           h04d1e81_0                        
wheel               0.38.4          py311hca03da5_0                    
xz                  5.4.2           h80987f9_0                        
zlib                1.2.13          h5a0b063_0
```
<a name="DYELX"></a>
### Pip
Pip只安装软件包所需的依赖性。
```bash
$ pip install pandas

$ pip list
Package         Version
--------------- -------
numpy           1.24.3
pandas          2.0.2
pip             22.3.1
python-dateutil 2.8.2
pytz            2023.3
setuptools      65.5.0
six             1.16.0
tzdata          2023.3
```
<a name="qBkZv"></a>
### Poetry
Poetry也只安装软件包所需的依赖项。
```bash
$ poetry add pandas
$ poetry show
```
<a name="JeQYe"></a>
## 卸载Packages
卸载软件包及其依赖关系可以释放磁盘空间，防止不必要的杂乱，并优化存储资源的使用。
<a name="YRGNL"></a>
### Pip
Pip只删除指定的包，而不是它的依赖关系，可能会导致未使用的依赖关系长期积累。这可能导致增加存储空间的使用和潜在的冲突。
```bash
$ pip install pandas
$ pip uninstall pandas
$ pip list

Package         Version
--------------- -------
numpy           1.24.3
pip             22.0.4
python-dateutil 2.8.2
pytz            2023.3
setuptools      56.0.0
six             1.16.0
tzdata          2023.3
```
<a name="KfuMH"></a>
### Conda
Conda删除了该软件包及其依赖关系。
```bash
$ conda install -c conda pandas

$ conda uninstall -c conda pandas

Collecting package metadata (repodata.json): done
Solving environment: done

## Package Plan ##

  environment location: /Users/khuyentran/miniconda3/envs/test-conda

  removed specs:
    - pandas


The following packages will be REMOVED:

  blas-1.0-openblas
  bottleneck-1.3.5-py311ha0d4635_0
  libcxx-14.0.6-h848a8c0_0
  libgfortran-5.0.0-11_3_0_hca03da5_28
  libgfortran5-11.3.0-h009349e_28
  libopenblas-0.3.21-h269037a_0
  llvm-openmp-14.0.6-hc6e5704_0
  numexpr-2.8.4-py311h6dc990b_1
  numpy-1.24.3-py311hb57d4eb_0
  numpy-base-1.24.3-py311h1d85a46_0
  pandas-1.5.3-py311h6956b77_0
  python-dateutil-2.8.2-pyhd3eb1b0_0
  pytz-2022.7-py311hca03da5_0
  six-1.16.0-pyhd3eb1b0_1


Proceed ([y]/n)? 

Preparing transaction: done
Verifying transaction: done
Executing transaction: done
```
<a name="Cx0pL"></a>
### Poetry
Poetry还删除了该软件包及其依赖关系。
```bash
$ poetry add pandas
$ poetry remove pandas

  • Removing numpy (1.24.3)
  • Removing pandas (2.0.2)
  • Removing python-dateutil (2.8.2)
  • Removing pytz (2023.3)
  • Removing six (1.16.0)
  • Removing tzdata (2023.3)
```
<a name="vk2uf"></a>
## 依赖性文件
依赖文件通过指定所需软件包的确切版本或版本范围来确保软件项目环境的可重复性。<br />这有助于在不同的系统或不同的时间点上重新创建相同的环境，确保具有相同依赖关系的开发人员之间的协作。
<a name="bYVBk"></a>
### Conda
为了在Conda环境中保存依赖关系，需要手动将它们写入一个文件中。在environment.yml文件中指定的版本范围可能会导致不同的版本被安装，在复制环境时可能会引入兼容性问题。<br />假设已经安装了1.5.3版的pandas作为例子。下面是一个指定了依赖关系的environment.yml文件的例子：
```bash
# environment.yml
name: test-conda
channels:
  - defaults
dependencies:
  - python=3.8
  - pandas>=1.5
```
如果一个新的用户试图在pandas的最新版本是2.0的时候重现环境，那么pandas 2.0将被替代安装。
```bash
# 创建并激活一个虚拟环境
$ conda env create -n env
$ conda activate env

# 列出当前环境中的软件包
$ conda list
...
pandas 2.0
```
如果代码库依赖于pandas 1.5.3版本特有的语法或行为，而语法在2.0版本中发生了变化，那么用pandas 2.0运行代码可能会带来错误。
<a name="uVxzL"></a>
### Pip
同样的问题也可能发生在管道上。
```bash
# Requirements.txt
pandas>=1.5
# 创建并激活一个虚拟环境
$ python3 -m venv venv
$ source venv/bin/activate

# 安装依赖项
$ pip install -r requirements.txt

# 列出软件包
$ pip list
Package    Version
---------- -------
pandas       2.0
...
...
```
可以通过把它们冻结在requirements.txt文件中来确定版本：
```bash
$ pip freeze > requirements.txt
# requirements.txt

numpy==1.24.3
pandas==1.5.3
python-dateutil==2.8.2
pytz==2023.3
six==1.16.0
```
```
然而，这样的代码环境并不那么灵活，从长远来看，可能更难维护。对依赖关系的任何改变都需要手动修改requirements.txt文件，这可能很费时且容易出错。
<a name="NVvgP"></a>
### Poetry
Poetry在安装软件包时自动更新pyproject.toml文件。<br />在下面的例子中，"pandas "包被加入了版本约束"^1.5"。这种灵活的版本管理方法确保你的项目可以适应更新的版本，而不需要手动调整。
```bash
$ poetry add 'pandas=^1.5'
# pyproject.toml

[tool.poetry.dependencies]
python = "^3.8"
pandas = "^1.5"
```
poetry.lock文件存储了每个软件包及其依赖关系的精确版本号。
```
# poetry.lock
...
[[package]]
name = "pandas"
version = "1.5.3"
description = "Powerful data structures for data analysis, time series, and statistics"
category = "main"
optional = false
python-versions = ">=3.8"

[package.dependencies]
numpy = [
    {version = ">=1.20.3", markers = "python_version < \"3.10\""},
    {version = ">=1.21.0", markers = "python_version >= \"3.10\""},
    {version = ">=1.23.2", markers = "python_version >= \"3.11\""},
]
python-dateutil = ">=2.8.2"
pytz = ">=2020.1"
tzdata = ">=2022.1"
...
```
这保证了所安装软件包的一致性，即使一个软件包在 pyproject.toml 文件中指定了版本范围。在这里，可以看到pandas 1.5.3被安装，而不是pandas 2.0
```bash
$ poetry install

$ poetry show pandas

name         : pandas                                                                  
version      : 1.5.3                                                                   
description  : Powerful data structures for data analysis, time series, and statistics 

dependencies
 - numpy >=1.20.3
 - numpy >=1.21.0
 - numpy >=1.23.2
 - python-dateutil >=2.8.1
 - pytz >=2020.1
```
<a name="wMhG8"></a>
## 为开发和生产环境提供单独的依赖
通过分离依赖关系，可以明确区分开发目的所需的包，如测试框架和代码质量工具，和生产环境所需的包，通常包括核心依赖关系。<br />这确保生产环境只包含运行应用程序所需的软件包，减少冲突或兼容性问题的风险。
<a name="SKnGv"></a>
### Conda
Conda本质上并不支持不同环境下的独立依赖，但一个变通方法是创建两个环境文件：一个用于开发环境，一个用于生产环境。开发文件包含了生产和开发的依赖关系。
```yaml
# environment.yml
name: test-conda
channels:
  - defaults
dependencies:
  # Production packages
  - numpy
  - pandas
# environment-dev.yml
name: test-conda-dev
channels:
  - defaults
dependencies:
  # Production packages
  - numpy
  - pandas
  # Development packages
  - pytest
  - pre-commit
```
<a name="HDtdF"></a>
### Pip
Pip也不直接支持独立的依赖关系，但类似的方法可以用独立的需求文件。
```bash
# requirements.txt
numpy 
pandas
# requirements-dev.txt
-r requirements.txt
pytest
pre-commit
# Install prod
$ pip install -r requirements.txt

# Install both dev and prod
$ pip install -r requirements-dev.txt
```
<a name="qPOfI"></a>
### Poetry
Poetry通过支持一个文件中的分组，简化了对依赖关系的管理。这使你能够在一个地方跟踪所有的依赖关系。
```bash
$ poetry add numpy pandas
$ poetry add --group dev pytest pre-commit
# pyproject.toml
[tool.poetry.dependencies]
python = "^3.8"
pandas = "^2.0"
numpy = "^1.24.3"

[tool.poetry.group.dev.dependencies]
pytest = "^7.3.2"
pre-commit = "^3.3.2"
```
要只安装生产依赖性：
```bash
$ poetry install --only main
```
要同时安装开发和生产依赖：
```bash
$ poetry install
```
<a name="BMv25"></a>
## 更新一个环境
更新依赖关系对于从较新的软件包版本中引入的错误修复、性能改进和新功能中获益至关重要。
<a name="F8yOM"></a>
### Conda
Conda可以只更新一个指定的软件包。
```bash
$ conda install -c conda pandas
$ conda install -c anaconda scikit-learn
# 新版本可用
$ conda update pandas
$ conda update scikit-learn
```
之后，需要手动更新environment.yaml文件，使其与更新的依赖关系保持同步。
```bash
$ conda env export > environment.yml
```
<a name="svHmJ"></a>
### Pip
Pip也只允许更新一个指定的软件包，并要求手动更新requirements.txt文件。
```bash
$ pip install -U pandas
$ pip freeze > requirements.txt
```
<a name="dicQp"></a>
### Poetry
使用Poetry，可以使用 update 命令来升级 pyproject.toml 文件中指定的所有软件包。这个动作会自动更新poetry.lock文件，确保软件包规格和锁文件的一致性。
```bash
$ poetry add pandas scikit-learn

# 新版本可用
poetry update

Updating dependencies
Resolving dependencies... (0.3s)

Writing lock file

Package operations: 0 installs, 2 updates, 0 removals

  • Updating pandas (2.0.0 -> 2.0.2)
  • Updating scikit-learn (1.2.0 -> 1.2.2)
```
<a name="J70R7"></a>
## 依赖关系的解决
当项目所需的包或库有冲突的版本或不兼容的依赖性时，就会发生依赖性冲突。正确解决冲突对于避免错误、运行时问题或项目失败至关重要。
<a name="jYHsk"></a>
### Pip
pip是按顺序安装软件包的，这意味着它按照指定的顺序一个一个地安装每个软件包。当软件包有不兼容的依赖关系或版本要求时，这种顺序方法有时会导致冲突。<br />例如，假设你先安装pandas==2.0.2，这需要numpy>=1.20.3。后来，用pip安装numpy==1.20.2。尽管这将产生依赖性冲突，但pip会继续更新numpy的版本。
```bash
$ pip install pandas==2.0.2

$ pip install numpy==1.22.2
Collecting numpy=1.20.2
  Attempting uninstall: numpy
    Found existing installation: numpy 1.24.3
    Uninstalling numpy-1.24.3:
      Successfully uninstalled numpy-1.24.3
ERROR: pip's dependency resolver does not currently take into account all the packages that are installed. This behaviour is the source of the following dependency conflicts.
pandas 2.0.2 requires numpy>=1.20.3; python_version < "3.10", but you have numpy 1.20.2 which is incompatible.
Successfully installed numpy-1.20.2
```
<a name="ze5gR"></a>
### Conda
Conda使用一个SAT求解器来探索软件包版本和依赖关系的所有组合，以找到一个兼容的集合。<br />例如，如果一个现有的软件包对其依赖关系有特定的约束（例如，statsmodels==0.13.2要求numpy>=1.21.2,<2.0a0），而你要安装的软件包不符合该要求（例如，numpy<1.21.2），conda不会立即引发错误。相反，它将勤奋地搜索所有需要的软件包及其依赖的兼容版本，只有在没有找到合适的解决方案时才会报告错误。
```bash
$ conda install 'statsmodels==0.13.2'

$ conda search 'statsmodels==0.13.2' --info
dependencies: 
  - numpy >=1.21.2,<2.0a0
  - packaging >=21.3
  - pandas >=1.0
  - patsy >=0.5.2
  - python >=3.9,<3.10.0a0
  - scipy >=1.3

$ conda install 'numpy<1.21.2'

...
Package ca-certificates conflicts for:
python=3.8 -> openssl[version='>=1.1.1t,<1.1.2a'] -> ca-certificates
openssl -> ca-certificates
ca-certificates
cryptography -> openssl[version='>1.1.0,<3.1.0'] -> ca-certificates

Package idna conflicts for:
requests -> urllib3[version='>=1.21.1,<1.27'] -> idna[version='>=2.0.0']
requests -> idna[version='>=2.5,<3|>=2.5,<4']
idna
pooch -> requests -> idna[version='>=2.5,<3|>=2.5,<4']
urllib3 -> idna[version='>=2.0.0']

Package numexpr conflicts for:
statsmodels==0.13.2 -> pandas[version='>=1.0'] -> numexpr[version='>=2.7.0|>=2.7.1|>=2.7.3']
numexpr
pandas==1.5.3 -> numexpr[version='>=2.7.3']

Package patsy conflicts for:
statsmodels==0.13.2 -> patsy[version='>=0.5.2']
patsy

Package chardet conflicts for:
requests -> chardet[version='>=3.0.2,<4|>=3.0.2,<5']
pooch -> requests -> chardet[version='>=3.0.2,<4|>=3.0.2,<5']

Package python-dateutil conflicts for:
statsmodels==0.13.2 -> pandas[version='>=1.0'] -> python-dateutil[version='>=2.7.3|>=2.8.1']
python-dateutil
pandas==1.5.3 -> python-dateutil[version='>=2.8.1']

Package setuptools conflicts for:
numexpr -> setuptools
pip -> setuptools
wheel -> setuptools
setuptools
python=3.8 -> pip -> setuptools
pandas==1.5.3 -> numexpr[version='>=2.7.3'] -> setuptools

Package brotlipy conflicts for:
urllib3 -> brotlipy[version='>=0.6.0']
brotlipy
requests -> urllib3[version='>=1.21.1,<1.27'] -> brotlipy[version='>=0.6.0']

Package pytz conflicts for:
pytz
pandas==1.5.3 -> pytz[version='>=2020.1']
statsmodels==0.13.2 -> pandas[version='>=1.0'] -> pytz[version='>=2017.3|>=2020.1']
```
虽然这种方法提高了找到解决方案的机会，但它可能是计算密集型的，特别是在处理繁杂的环境时。
<a name="qNZbo"></a>
### Poetry
通过关注项目的直接依赖关系，Poetry的确定性解析器缩小了搜索空间，使解析过程更加高效。它评估指定的约束条件，如版本范围或特定版本，并立即识别任何冲突。
```bash
$ poetry add 'seaborn==0.12.2'
$ poetry add 'matplotlib<3.1' 

Because poetry shell depends on seaborn (0.12.2) which depends on matplotlib (>=3.1,<3.6.1 || >3.6.1), matplotlib is required.
So, because poetry shell depends on matplotlib (<3.1), version solving failed.
```
这种即时反馈有助于防止潜在的问题升级，并允许开发人员在开发过程中及早解决问题。例如，在下面的代码中，可以放宽对seaborn的要求，以便能够安装特定版本的matplotlib：
```bash
poetry add 'seaborn<=0.12.2'  'matplotlib<3.1' 

Package operations: 1 install, 2 updates, 4 removals

  • Removing contourpy (1.0.7)
  • Removing fonttools (4.40.0)
  • Removing packaging (23.1)
  • Removing pillow (9.5.0)
  • Updating matplotlib (3.7.1 -> 3.0.3)
  • Installing scipy (1.9.3)
  • Updating seaborn (0.12.2 -> 0.11.2)
```
<a name="ZWXJa"></a>
## 总结
综上所述，Poetry提供了比pip和conda更多的优势：

1. **一致的软件包安装：**Poetry提供了一个一致的格式来安装任何软件包，确保整个项目有一个标准化的方法。
2. **广泛的软件包选择：**Poetry提供了对PyPI上广泛的软件包的访问，使你可以为你的项目利用一个多样化的生态系统。
3. **高效的依赖性管理：**Poetry只为指定的软件包安装必要的依赖性，减少你环境中不相干的软件包的数量。
4. **简化的软件包移除：**Poetry简化了软件包及其相关依赖关系的移除，使其易于维护一个干净和高效的项目环境。
5. **依赖性解决：**Poetry的确定性解析器有效地解决了依赖关系，及时识别并处理任何不一致或冲突。

虽然Poetry可能需要你的团队成员花费一些额外的时间和精力来学习和适应，但从长远来看，使用Poetry这样的工具可以为你节省时间和精力。
