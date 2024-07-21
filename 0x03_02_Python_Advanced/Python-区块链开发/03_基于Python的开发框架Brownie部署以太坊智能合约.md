# Python 

## 介绍
在区块链开发中，常用的是以太坊虚拟机智能合约语言Solidity，它具有许多不错的功能，并且仍然可以使用 Python 进行部署。刚开始使用Solidity时，使用了Remix（[https://remix.ethereum.org/](https://remix.ethereum.org/)），这是一个强大的Web IDE，可让进行智能合约可视化。Remix很棒，但是在单个IDE之外可以实现很多其他功能。Truffle（[https://www.trufflesuite.com/](https://www.trufflesuite.com/)）和HardHat（[https://hardhat.org/guides/mainnet-forking.html](https://hardhat.org/guides/mainnet-forking.html)），它们是用于部署智能合约的Node.js框架。

这些框架都不错，Brownie 和web3.py：一个用于部署智能合约的Python框架和一个用于区块链开发的开源协议。


## 为什么选择Python
有这么多数据科学家、学者和金融科技机构使用Python是有原因的。它用途广泛，具有轻松的开发体验，并且与各种第三方库紧密结合在一起。顶级 defi 项目开始意识到这一点，诸如yearn.finance之类的项目使用python来部署其所有生产代码。Yearn.finance由一群非常有才华的金融科技工程师经营，他们转向了区块链，带着他们熟悉和喜爱的Python工具。

## Brownie是什么？
Brownie是由Ben Hauser创建的Python智能合约开源框架，又名“iamdefinitelyahuman”（中文意思“非绝对人类”），是一件艺术品。这就是yearn.finance团队用来部署和维护智能合约的工具。您可以使用简单的命令启动项目，然后立即开始使用代码。

## 用Python部署第一个智能合约
### 1、安装 Brownie 和 bake
Brownie具有“baking”功能，可让您使用一些基础代码启动存储库，因为大多数项目都需要很多相同的部分，类似于create-eth-app。要开始使用，和其他所有Python软件包的安装方式一样。
```bash
pip install eth-brownie
```
还需要安装ganache-cli一个用于部署本地区块链的软件包。为此，需要安装npm和nodejs。
```bash
npm install -g ganache-cli
```
准备开始！将使用chainlink-mix入门，因为许多顶级defi项目都使用Chainlink来获取其资产数据。
```bash
brownie bake chainlink-mix
cd chainlink
```
通过`ls`命令展示项目的结构布局

Brownie项目布局

```bash
build : This is where the project keeps track of your deployed smart contracts and compiled contracts
contracts : The source code of your contracts, typically written in solidity or vyper
interfaces : A layout of interfaces you’ll need to work with deployed contracts. Every interaction with a contract needs an ABI and an address. Interfaces are great ways to get a contract’s ABI
scripts : Scripts we create to automate processes of working with our contracts
tests : Tests
brownie-config.yaml : This is where we have all the information for brownie to understand how to work with our smart contract. What blockchain do we want to deploy to? Are there any special parameters we want to set? All these are set in the config file.
```
requirements.txt，README.md，LICENSE和.gitignore可以忽略，在后面了解它们的用途。

### 2、设置环境变量
如果熟悉区块链开发，就会知道本地区块链，测试网区块链和主网区块链都是不同的东西，这里将部署到测试网，以便可以与真实的实时区块链网络进行交互。需要一个`WEB3_INFURA_PROJECT_ID`，可以通过创建Infura帐户来检索该`WEB3_INFURA_PROJECT_ID`。这就是用来连接到测试网络的东西。还将获得一个`metamask`或其他web3以太坊钱包，并用一些ETH进行注资。对于这个demo，要使用Kovan测试网络。

可以跳过有关LINK资金的部分，只需要testnet ETH。也不会使用Ropsten，而是使用Kovan。如果已经有了钱包，请从[https://gitter.im/kovan-testnet/faucet](https://gitter.im/kovan-testnet/faucet)获取一些Kovan Ether。


#### 安装，配置和Metamask
一旦有了Metamask钱包，就可以将私钥导出到`PRIVATE_KEY`环境变量。在此处([https://www.twilio.com/blog/2017/01/how-to-set-environment-variables.html](https://www.twilio.com/blog/2017/01/how-to-set-environment-variables.html))阅读有关设置环境变量的信息。请随意将代码中的`PRIVATE_KEY`替换为自己的私钥和`WEB3_INFURA_PROJECT_ID`。

### 3、部署智能合约
在脚本文件夹中，有一个名为`deploy_price_consumer_v3.py`的脚本，该脚本将部署智能合约，该合约读取以太坊的美元价格。如果想更轻松地了解该合约的功能以及如何部署它，请随时查看有关部署价格订阅合同的Chainlink教程([https://docs.chain.link/docs/beginners-tutorial/](https://docs.chain.link/docs/beginners-tutorial/))。brownie run是可以用来运行脚本的命令。如果仅运行`brownie`，则可以看到所有命令的列表。
```bash
brownie run scripts/price_feed_scripts/deploy_price_consumer_v3.py --network kovan
```
`--network kovan`允许设置要使用的网络。正在使用kovan testnet进行此演示。需要Kovan ETH来做到这一点！将获得很多输出内容，但最终会得到类似以下结果：
```bash
Running 'scripts/price_feed_scripts/deploy_price_consumer_v3.py::main'...
Transaction sent: 0x23d1dfa3937e0cfbab58f8d5ecabe2bfffc28bbe2349527dabe9289e747bac56
Gas price: 20.0 gwei   Gas limit: 145600   Nonce: 1339
PriceFeed.constructor confirmed - Block: 22721813   Gas used: 132364 (90.91%)
PriceFeed deployed at: 0x6B2305935DbC77662811ff817cF3Aa54fc585816
```
如果此方法正常运行，可以转到kovan etherscan并找到部署的合约。上面的链接显示了此示例中部署的合约。

### 4、读取智能合约
现在已经部署了智能合约，可以从刚刚部署的合约中读取以太坊的价格。将运行另一个脚本：
```bash
brownie run scripts/price_feed_scripts/read_price_feed.py --network kovan
```
得到类似以下的输出：
```bash
Brownie v1.12.2 - Python development framework for Ethereum
ChainlinkProject is the active project.
Running 'scripts/price_feed_scripts/read_price_feed.py::main'...
Reading data from 0x6B2305935DbC77662811ff817cF3Aa54fc585816
62322000000
Where 62322000000 is the current price of ETH in USD! Solidity doesn’t understand decimals, and we know that this example has 8 decimals, so the price is $623.22 .
```

## 使用web3.py
Brownie使用名为web3.py的工具让您的开发更轻松，但是如果机智点，则始终可以直接在没有框架的情况下使用合约。Web3.py是一个原始程序包，可以使用它来更直接地处理合同。为此，只需要上面的Kovan infura项目ID。请记住，要与任何智能合约进行交互，需要做两件事：

- 智能合约ABI
- 智能合约地址

Brownie 会在后台处理很多此类工作，也可以手动进行。这是通过web3.py从链上合同中读取的内容。首先，需要安装web3.py。
```bash
pip install web3
```
然后，可以在文件中运行以下内容。
```python
web3 = Web3(Web3.HTTPProvider('https://kovan.infura.io/v3/<infura_project_id>')) 
abi = '[{"inputs":[],"name":"decimals","outputs":[{"internalType":"uint8","name":"","type":"uint8"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"description","outputs":[{"internalType":"string","name":"","type":"string"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"uint80","name":"_roundId","type":"uint80"}],"name":"getRoundData","outputs":[{"internalType":"uint80","name":"roundId","type":"uint80"},{"internalType":"int256","name":"answer","type":"int256"},{"internalType":"uint256","name":"startedAt","type":"uint256"},{"internalType":"uint256","name":"updatedAt","type":"uint256"},{"internalType":"uint80","name":"answeredInRound","type":"uint80"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"latestRoundData","outputs":[{"internalType":"uint80","name":"roundId","type":"uint80"},{"internalType":"int256","name":"answer","type":"int256"},{"internalType":"uint256","name":"startedAt","type":"uint256"},{"internalType":"uint256","name":"updatedAt","type":"uint256"},{"internalType":"uint80","name":"answeredInRound","type":"uint80"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"version","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"view","type":"function"}]' 
addr = '0x9326BFA02ADD2366b30bacB125260Af641031331' 
contract = web3.eth.contract(address=addr, abi=abi) 
latestData = contract.functions.latestRoundData().call() print(latestData)
```
运行上述操作后将在控制台中打印以美元为单位的ETH的最新价格。请查看Chainlink文档以确定是否有问题。



## 结论

可以从他们的文档中了解有关Web3.py和Brown的更多信息。这两个项目都是开源的，任何人都可以做出贡献！

[https://github.com/eth-brownie/brownie](https://github.com/eth-brownie/brownie)

[https://github.com/ethereum/web3.py](https://github.com/ethereum/web3.py)
