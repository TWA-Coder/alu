// const hre = require("hardhat");
import hre from "hardhat";
const fs = require("fs");
const crypto = require("crypto");

async function main() {
  // Hash the ALU logo file to get the real hash
  let logoHash;
  try {
    const fileBuffer = fs.readFileSync("alu-logo.png");
    const hashSum = crypto.createHash("sha256");
    hashSum.update(fileBuffer);
    logoHash = "0x" + hashSum.digest("hex");
    console.log(`Generated ALU Logo Hash: ${logoHash}`);
  } catch (error) {
    console.log("Could not read 'alu-logo.png'. Using a dummy hash for now, please ensure the file exists.");
    // Dummy hash for testing if file is missing
    logoHash = "0x" + crypto.createHash("sha256").update("dummy-logo").digest("hex");
  }

  const [deployer] = await hre.ethers.getSigners();
  console.log("Deploying contracts with the account:", deployer.address);

  // Deploy Registry
  const ALUAssetRegistry = await hre.ethers.getContractFactory("ALUAssetRegistry");
  const registry = await ALUAssetRegistry.deploy();
  await registry.waitForDeployment();
  const registryAddress = await registry.getAddress();
  console.log(`ALUAssetRegistry deployed to: ${registryAddress}`);

  // Register the Logo immediately
  const tx = await registry.registerAsset("ALU Logo", "png", logoHash);
  await tx.wait();
  console.log(`ALU Logo registered on the blockchain with hash: ${logoHash}`);

  // Deploy Token
  const ALULogoToken = await hre.ethers.getContractFactory("ALULogoToken");
  const token = await ALULogoToken.deploy(deployer.address);
  await token.waitForDeployment();
  const tokenAddress = await token.getAddress();
  console.log(`ALULogoToken deployed to: ${tokenAddress}`);
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
