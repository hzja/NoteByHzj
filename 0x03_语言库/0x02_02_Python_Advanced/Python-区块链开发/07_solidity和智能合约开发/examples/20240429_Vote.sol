// SPDX-License-Identifier: GPL-3.0
pragma solidity >=0.7.0 <0.9.0;
/// @title 委托投票
contract Ballot {
    // 这声明了一个新的复杂类型，用于稍后变量。
    // 它用来表示一个选民。
    struct Voter {
        uint weight; // 计票的权重
        bool voted;  // 若为真，代表该人已投票
        address delegate; // 被委托人
        uint vote;   // 投票提案的索引
    }

    // 提案的类型
    struct Proposal {
        bytes32 name;   // 简称（最长32个字节）
        uint voteCount; // 得票数
    }

    //chairperson是主席，主持投票的人;
    address public chairperson;
    
    // 这声明了一个状态变量，为每个可能的地址存储一个 `Voter`。
    mapping(address => Voter) public voters;
    // 这段代码是智能合约中的一部分，使用的是Solidity语言，它是用于编写以太坊智能合约的编程语言。让我们逐部分解释这段代码的含义：
    // mapping(address => Voter): 这部分定义了一个名为voters的映射（map）。在Solidity中，映射是一种数据结构，
        // 它将一个值（键）映射到另一个值（值）。在这个例子中，映射的键是address类型，它代表以太坊地址，
        // 而映射的值是Voter类型，这可能是一个用户定义的合约或者结构体，用于存储与投票相关的信息。
    // public: 这个关键字表示voters映射是一个公共状态变量。在智能合约中，公共状态变量允许外部直接读取其值，但不允许直接修改它。
        // 要修改公共变量，通常需要通过合约中定义的公共函数来进行。
    // voters: 这是映射的名称，它将存储与每个以太坊地址关联的Voter信息。
    // 综上所述，这段代码定义了一个公共的映射，用于存储与每个以太坊地址关联的投票者信息。
        // 智能合约的其他部分可能会包含函数，允许添加、更新或查询这些投票者的信息。

    // 一个 `Proposal` 结构类型的动态数组。
    Proposal[] public proposals;
    // Proposal[]: 这表示proposals是一个数组，它可以存储多个Proposal类型的对象。
            //Proposal很可能是一个之前定义好的用户自定义类型（可能是一个合约或者一个结构体），用于表示一个提案的详细信息，
            // 比如提案的标题、描述、投票选项等。

    /// 为 `proposalNames` 中的每个提案，创建一个新的（投票）表决
    constructor(bytes32[] memory proposalNames) {
    // bytes32[]: 这表示一个bytes32类型的数组。在Solidity中，bytes32是一个固定长度的字节数组，包含32个字节。
    //     这种数据类型常用于存储哈希值、地址或者其它需要固定长度表示的数据。
    // memory: 这个关键字指明了变量的存储位置。在Solidity中，memory用于表示这个数组将存储在调用者的内存中，
    //     而不是存储在区块链的存储中。这意味着这个数组是暂时的，仅在当前函数的执行上下文中有效，一旦函数执行完毕，这个数组就会被销毁。
    // proposalNames: 这是变量的名称，它代表一个包含bytes32类型元素的数组。根据变量的命名，我们可以推测这个数组可能用于
    //     存储提案的名称或者标识符。

        chairperson = msg.sender;
        voters[chairperson].weight = 1;

        // 对于提供的每个提案名称，
        // 创建一个新的 Proposal 对象并把它添加到数组的末尾。
        for (uint i = 0; i < proposalNames.length; i++) {
            // `Proposal({...})` 创建一个临时 Proposal 对象
            // `proposals.push(...)` 将其添加到 `proposals` 的末尾
            proposals.push(Proposal({
                name: proposalNames[i],
                voteCount: 0
            }));
        }
    }

    // 给予 `voter` 在这张选票上投票的权利。
    // 只有 `chairperson` 可以调用该函数。
    function giveRightToVote(address voter) external {
        // 若 `require` 的第一个参数的计算结果为 `false`，
        // 则终止执行，撤销所有对状态和以太币余额的改动。
        // 在旧版的 EVM 中这曾经会消耗所有 gas，但现在不会了。
        // 使用 `require` 来检查函数是否被正确地调用，通常是个好主意。
        // 您也可以在 `require` 的第二个参数中提供一个对错误情况的解释。
        require(
            msg.sender == chairperson,
            "Only chairperson can give right to vote."
        );
        require(
            !voters[voter].voted,
            "The voter already voted."
        );
        require(voters[voter].weight == 0);
        voters[voter].weight = 1;
    }

    /// 把您的投票委托给投票者 `to`。
    function delegate(address to) external {
        // 指定引用
        Voter storage sender = voters[msg.sender];
        // Voter storage sender: 这里声明了一个名为sender的变量，它的类型是Voter，并且存储在合约的持久存储（storage）中。
        //     Voter很可能是一个之前定义好的用户自定义类型，比如一个结构体或合约，用于表示一个投票者的信息。
        // =: 这是赋值操作符，用于将右侧表达式的值赋给左侧的变量。
        // voters[msg.sender]: 这是一个映射访问操作。voters是一个之前定义的映射，它将address类型的键（以太坊地址）映射到Voter类型的值。
        //     msg.sender是一个全局变量，代表当前消息的发送者，即调用智能合约的以太坊地址。
        //     因此，voters[msg.sender]表示获取与msg.sender关联的Voter类型的值。

        require(sender.weight != 0, "You have no right to vote");
        require(!sender.voted, "You already voted.");
        require(to != msg.sender, "Self-delegation is disallowed.");

        // 委托是可以传递的，只要被委托者 `to` 也设置了委托。
        // 一般来说，这样的循环委托是非常危险的，因为如果传递的链条太长，
        // 可能需要消耗的gas就会超过一个区块中的可用数量。
        // 这种情况下，委托不会被执行。
        // 但在其他情况下，如果形成闭环，则会导致合约完全被 "卡住"。
        while (voters[to].delegate != address(0)) {
            // 这是一个while循环，它将持续执行，直到条件voters[to].delegate != address(0)不再为真。这里涉及到几个概念：
            //     voters: 这是一个映射，它将address类型的键映射到某个自定义类型（在这个场景中很可能是Voter类型）。
            //     to: 这是当前正在处理的以太坊地址，它代表了一个投票者。
            //     delegate: 这是Voter类型中的一个属性，代表了一个代理地址。如果一个投票者选择了代理投票，那么他们的投票权将委托给这个代理地址。
            //     address(0): 这是以太坊中表示空地址的常量，通常用来表示没有地址或者无效的地址。

            to = voters[to].delegate;
            // to = voters[to].delegate;: 循环体内的这行代码将当前处理的地址to更新为该地址对应的代理地址。
            //     如果voters[to].delegate是一个有效的地址（不是address(0)），循环将继续，to将被更新为这个代理地址，
            //     然后再次检查这个新的地址是否有自己的代理。

            // 不允许闭环委托
            require(to != msg.sender, "Found loop in delegation.");
        }

        Voter storage delegate_ = voters[to];

        // 投票者不能将投票权委托给不能投票的账户。
        require(delegate_.weight >= 1);

        // 由于 `sender` 是一个引用，
        // 因此这会修改 `voters[msg.sender]`。
        sender.voted = true;
        sender.delegate = to;

        if (delegate_.voted) {
            // 若被委托者已经投过票了，直接增加得票数。
            proposals[delegate_.vote].voteCount += sender.weight;
        } else {
            // 若被委托者还没投票，增加委托者的权重。
            delegate_.weight += sender.weight;
        }
    }

    /// 把您的票(包括委托给您的票)，
    /// 投给提案 `proposals[proposal].name`。
    function vote(uint proposal) external {
        Voter storage sender = voters[msg.sender];
        require(sender.weight != 0, "Has no right to vote");
        require(!sender.voted, "Already voted.");
        sender.voted = true;
        sender.vote = proposal;

        // 如果 `proposal` 超过了数组的范围，
        // 则会自动抛出异常，并恢复所有的改动。
        proposals[proposal].voteCount += sender.weight;
    }

    /// @dev 结合之前所有投票的情况下，计算出获胜的提案。
    function winningProposal() public view
            returns (uint winningProposal_)
    {
        uint winningVoteCount = 0;
        for (uint p = 0; p < proposals.length; p++) {
            if (proposals[p].voteCount > winningVoteCount) {
                winningVoteCount = proposals[p].voteCount;
                winningProposal_ = p;
            }
        }
    }

    // 调用winningProposal()函数获取提案数组中获胜者的索引,
    // 并以此返回获胜者的名称。
    function winnerName() external view
            returns (bytes32 winnerName_)
    {
        winnerName_ = proposals[winningProposal()].name;
    }
}