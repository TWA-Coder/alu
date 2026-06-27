// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/access/Ownable.sol";

contract ALULogoToken is ERC20, Ownable {
    constructor(address initialOwner) 
        ERC20("ALU Logo Token", "ALUT") 
        Ownable(initialOwner) 
    {
        uint256 supply = 1000000 * 10 ** decimals();
        _mint(initialOwner, supply);
    }

    function distributeShares(address recipient, uint256 amount) public onlyOwner {
        require(amount > 0, "Amount must be greater than zero");
        transfer(recipient, amount);
    }

    function ownershipPercentage(address wallet) public view returns (uint256) {
        uint256 totalSupplyRaw = totalSupply();
        if (totalSupplyRaw == 0) return 0;
        uint256 balanceRaw = balanceOf(wallet);
        return (balanceRaw * 100) / totalSupplyRaw;
    }
}
