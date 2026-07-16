const { expect } = require("chai");
const { ethers } = require("hardhat");

describe("ALU Logo Blockchain Project", function () {
  let registry, token;
  let owner, addr1, addr2;
  const dummyHash1 = ethers.id("alu-logo-real-hash");
  const dummyHash2 = ethers.id("another-hash");

  before(async function () {
    [owner, addr1, addr2] = await ethers.getSigners();
  });

  describe("Part B: ALUAssetRegistry (ERC-721)", function () {
    this.beforeEach(async function () {
      const RegistryFactory = await ethers.getContractFactory("ALUAssetRegistry");
      registry = await RegistryFactory.deploy();
      await registry.waitForDeployment();
    });

    it("1. The ALU logo registers successfully and returns a token ID", async function () {
      const tx = await registry.registerAsset("ALU Logo", "png", dummyHash1);
      const receipt = await tx.wait();
      // Inspect events to make sure AssetRegistered is emitted with correct ID
      const event = receipt.logs.find(e => e.fragment && e.fragment.name === 'AssetRegistered');
      expect(event).to.not.be.undefined;
      const tokenId = event.args[0];
      expect(tokenId).to.equal(0);
    });

    it("2. Attempting to register the same hash a second time is rejected with an error", async function () {
      await registry.registerAsset("ALU Logo", "png", dummyHash1);
      await expect(
        registry.registerAsset("ALU Logo 2", "png", dummyHash1)
      ).to.be.revertedWith("Hash already registered");
    });

    it("3. verifyLogoIntegrity() returns true when the correct hash is supplied", async function () {
      await registry.registerAsset("ALU Logo", "png", dummyHash1);
      const [isValid, msg] = await registry.verifyLogoIntegrity(0, dummyHash1);
      expect(isValid).to.be.true;
      expect(msg).to.equal("Logo is authentic.");
    });

    it("4. verifyLogoIntegrity() returns false when an incorrect hash is supplied", async function () {
      await registry.registerAsset("ALU Logo", "png", dummyHash1);
      const [isValid, msg] = await registry.verifyLogoIntegrity(0, dummyHash2);
      expect(isValid).to.be.false;
      expect(msg).to.equal("Warning: logo does not match.");
    });

    it("5. getAsset() returns the correct asset name and file type for a registered token", async function () {
      await registry.registerAsset("ALU Logo Test", "jpg", dummyHash1);
      const asset = await registry.getAsset(0);
      expect(asset.name).to.equal("ALU Logo Test");
      expect(asset.fileType).to.equal("jpg");
      expect(asset.contentHash).to.equal(dummyHash1);
    });
  });

  describe("Part C: ALULogoToken (ERC-20)", function () {
    this.beforeEach(async function () {
      const TokenFactory = await ethers.getContractFactory("ALULogoToken");
      token = await TokenFactory.deploy(owner.address);
      await token.waitForDeployment();
    });

    it("6. The full supply of 1,000,000 ALUT tokens is minted to the logo owner when the contract is deployed", async function () {
      const balance = await token.balanceOf(owner.address);
      const expectedSupply = ethers.parseUnits("1000000", 18);
      expect(balance).to.equal(expectedSupply);
      
      const totalSupply = await token.totalSupply();
      expect(totalSupply).to.equal(expectedSupply);
    });

    it("7. distributeShares() correctly transfers the specified number of tokens to a recipient address", async function () {
      const transferAmount = ethers.parseUnits("5000", 18);
      await token.distributeShares(addr1.address, transferAmount);
      
      const addr1Balance = await token.balanceOf(addr1.address);
      expect(addr1Balance).to.equal(transferAmount);
    });

    it("8. ownershipPercentage() returns the correct percentage for a wallet that holds a known number of tokens", async function () {
      // 250,000 tokens out of 1,000,000 is 25%
      const transferAmount = ethers.parseUnits("250000", 18);
      await token.distributeShares(addr1.address, transferAmount);
      
      const percentage = await token.ownershipPercentage(addr1.address);
      expect(percentage).to.equal(25);
    });
  });
});
