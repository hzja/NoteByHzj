// SPDX-License-Identifier: GPL-3.0
pragma solidity ^0.8.4;
contract SimpleAuction {
    // 拍卖的参数。
    // 时间是 unix 的绝对时间戳（自1970-01-01以来的秒数）
    // 或以秒为单位的时间段。
   
    address payable public beneficiary;
    // address payable: 这是Solidity中的一种数据类型，它扩展了基本的 address 类型，增加了接收以太币（ether）的功能。
        // 在以太坊中，address 类型通常用来表示一个以太坊账户的地址，但是基本的 address 类型不能直接接收以太币。
        // 为了能够接收以太币，必须使用 address payable 类型。

    uint public auctionEndTime;

    // 拍卖的当前状态。
    address public highestBidder;
    uint public highestBid;

    // 允许取回以前的竞标。
    mapping(address => uint) pendingReturns;
    // 定义了一个名为 pendingReturns 的映射（Map），
    //     它在 Solidity 智能合约中用来存储与地址（address）相关的未处理返回金额（uint，即无符号整数）
    // mapping(address => uint): 这部分声明了一个映射类型，其中键（key）是 address 类型，值（value）是 uint 类型。
        // 在以太坊中，address 类型用于表示一个账户的地址，而 uint 是一个无符号整数，通常用来表示金额或数量等非负数值。
    // pendingReturns: 这是映射的名称，它将存储每个地址对应的未处理返回金额。

    // 拍卖结束后设为 `true`，将禁止所有的变更
    // 默认初始化为 `false`。
    bool ended;

    // 变化时将会发出的事件。
    event HighestBidIncreased(address bidder, uint amount);
    event AuctionEnded(address winner, uint amount);
    // event 是一种用于记录和通知智能合约中特定事件的机制
    
    // 描述失败的错误信息。
    // 三斜线的注释是所谓的 natspec 注释。
    // 当用户被要求确认一个交易或显示一个错误时，它们将被显示。

    /// 竞拍已经结束。
    error AuctionAlreadyEnded();

    /// 已经有一个更高的或相等的出价。
    error BidNotHighEnough(uint highestBid);

    /// 竞拍还没有结束。
    error AuctionNotYetEnded();

    /// 函数 auctionEnd 已经被调用。
    error AuctionEndAlreadyCalled();

    /// 以受益者地址 `beneficiaryAddress` 创建一个简单的拍卖，
    /// 拍卖时长为 `_biddingTime`。
    constructor(
        uint biddingTime,
        address payable beneficiaryAddress
    ) {
        beneficiary = beneficiaryAddress;
        auctionEndTime = block.timestamp + biddingTime;
        // block.timestamp是一个全局变量，返回自以太坊区块链创世区块(genesis block)以来的秒数。
    }

    /// 对拍卖进行出价，具体的出价随交易一起发送。
    /// 如果没有在拍卖中胜出，则返还出价。
    function bid() external payable {
        // 参数不是必要的。因为所有的信息已经包含在了交易中。
        // 关键字 `payable` 是函数能够接收以太币的必要条件。

        // 如果拍卖已结束，撤销函数的调用。
        if (block.timestamp > auctionEndTime)
            revert AuctionAlreadyEnded();
            // revert 是一个关键字，用于在智能合约中触发异常，从而中止当前的函数执行并消耗掉所有剩余的 gas，
            // 同时回滚所有已经进行的状态改变。revert 通常与错误信息一起使用，以便于调试和日志记录。

        // 如果出价不高，就把钱送回去
        //（revert语句将恢复这个函数执行中的所有变化，
        // 包括它已经收到钱）。
        if (msg.value <= highestBid)
            revert BidNotHighEnough(highestBid);
        if (highestBid != 0) {
            // 简单地使用 highestBidder.send(highestBid)
            // 返还出价时，是有安全风险的，
            // 因为它可能执行一个不受信任的合约。
            // 让接收方自己取钱总是比较安全的。
            pendingReturns[highestBidder] += highestBid;
        }
        // 這段代碼逻辑是，如果新的出价不高于当前最高出价，合约将拒绝出价并触发异常；
        // 如果有有效的最高出价，并且当前出价未能超越，合约将把之前的最高出价记入pendingReturns，以便出价者稍后取回。

        highestBidder = msg.sender;
        highestBid = msg.value;
        emit HighestBidIncreased(msg.sender, msg.value);
        // 语法：emit EventName(arg1, arg2, ..., argN);
        // 其中 EventName 是之前在合约中声明的事件名称，而 arg1 到 argN 是传递给事件的具体参数值。
    }

    /// 撤回出价过高的竞标。
    function withdraw() external returns (bool) {
        uint amount = pendingReturns[msg.sender];
        if (amount > 0) {
            // 将其设置为0是很重要的，
            // 因为接收者可以在 `send` 返回之前再次调用这个函数
            // 作为接收调用的一部分。
            pendingReturns[msg.sender] = 0;

            // msg.sender 不属于 `address payable` 类型，
            // 必须使用 `payable(msg.sender)` 明确转换，
            // 以便使用成员函数 `send()`。
            if (!payable(msg.sender).send(amount)) {
            // payable(address): payable是一个修饰符，它可以用来修改address类型，使其成为一个可接收以太币的地址。
                // 在Solidity中，基本的address类型是不能接收以太币的，只有address payable类型才能接收以太币。
            // msg.sender: 这是一个全局变量，代表当前调用智能合约的以太坊地址，即发送消息的地址。
            // !payable(msg.sender): 这个表达式尝试将msg.sender转换为address payable类型。
                // 在Solidity 0.8.0及以后的版本中，这种直接的类型转换是不允许的，因为address payable不再被视为address的隐式超类型。
                // 如果你使用的是这些版本，你需要显式地转换类型，例如使用payable(msg.sender)。
                // .send(amount): 这是address payable类型合约的一个内置函数，用于向该地址发送以太币。
                // amount是一个变量，表示要发送的以太币的数量，单位通常是Wei（1 Ether = 10^18 Wei）

                // 这里不需抛出异常，只需重置未付款
                pendingReturns[msg.sender] = amount;
                return false;
            }
        }
        return true;
    }
    // 这段代码是一个智能合约中的 withdraw 函数，它允许用户提取之前在合约中标记为待退款的以太币。以下是代码逻辑的详细解释：

        // 提取金额：
        //     uint amount = pendingReturns[msg.sender];
        // 这行代码从 pendingReturns 映射中提取与 msg.sender（当前调用者的地址）关联的待退款金额。

        // 检查金额：
        //     if (amount > 0) {
        // 这个 if 语句检查 msg.sender 是否有大于0的待退款金额。

        // 重置待退款金额：
        //     pendingReturns[msg.sender] = 0;
        // 如果存在待退款金额，这里将其重置为0。这是重要的一步，因为它防止了在 send 操作成功之前，用户可能通过调用者（可能是一个合约）重入 withdraw 函数，导致多次提取相同的资金。

        // 发送退款：
        //     if (!payable(msg.sender).send(amount)) {
        // 这行代码尝试向 msg.sender 发送 amount 数量的以太币。payable(msg.sender) 是对 msg.sender 的显式转换，确保它是一个 address payable 类型，这样才能使用 .send() 方法发送以太币。

        // 处理发送失败：
        //     pendingReturns[msg.sender] = amount;
        //     return false;
        // 如果 .send() 调用失败（可能是因为 msg.sender 是一个合约，而该合约没有接收以太币的能力），则函数将重置 pendingReturns[msg.sender] 为原来的金额，并返回 false 表示退款失败。

        // 正常结束：
        //     return true;    
        // 如果一切顺利，函数将返回 true 表示退款成功。


    /// 结束拍卖，并把最高的出价发送给受益人。
    function auctionEnd() external {
        // 对于可与其他合约交互的函数（意味着它会调用其他函数或发送以太币），
        // 一个好的指导方针是将其结构分为三个阶段：
        // 1. 检查条件
        // 2. 执行动作 (可能会改变条件)
        // 3. 与其他合约交互
        // 如果这些阶段相混合，其他的合约可能会回调当前合约并修改状态，
        // 或者导致某些效果（比如支付以太币）多次生效。
        // 如果合约内调用的函数包含了与外部合约的交互，
        // 则它也会被认为是与外部合约有交互的。

        // 1. 条件
        if (block.timestamp < auctionEndTime)
            revert AuctionNotYetEnded();
        if (ended)
            revert AuctionEndAlreadyCalled();

        // 2. 影响
        ended = true;
        emit AuctionEnded(highestBidder, highestBid);

        // 3. 交互
        beneficiary.transfer(highestBid);
        // beneficiary.transfer 是 Solidity 智能合约中的一个表达式，它用于向一个名为 beneficiary 的可支付地址（address payable）发送以太币。
        // 这个表达式由两部分组成：
            // beneficiary: 这是一个之前定义的 address payable 类型的公共变量或状态变量，它代表了接收以太币的受益人地址。
            // .transfer: 这是 address payable 类型的一个内置函数，用于向该地址发送以太币。

        // 语法和参数
        //     .transfer 函数的基本语法如下：
        //     bool transfer(uint256 amount) public payable returns (bool);
        //     amount: 这是一个 uint256 类型的参数，表示要发送的以太币的数量，单位是 Wei（1 Ether = 10^18 Wei）。

        // 返回值
        //     .transfer 函数返回一个布尔值，用于指示转账是否成功：
        //     如果转账成功，返回 true。
        //     如果转账失败，返回 false。转账可能因为多种原因失败，例如发送者的账户余额不足，或者交易的 gas 不足。
    }
}