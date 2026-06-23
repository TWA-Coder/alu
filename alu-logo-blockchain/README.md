# ALU Logo Blockchain Project

This project implements a system to register the ALU logo on the Ethereum blockchain as a Non-Fungible Token (NFT) and issue "ownership tokens" representing fractional ownership of the logo.

The project consists of two smart contracts:
1. `ALUAssetRegistry.sol` (ERC-721): Serves as an asset registry that registers the logo using its unique SHA-256 hash, preventing duplicate registrations and verifying authenticity.
2. `ALULogoToken.sol` (ERC-20): A tokenization contract that creates a fixed supply of 1,000,000 `ALUT` (ALU Logo Tokens). These tokens represent fractional ownership of the logo and can be distributed to stakeholders.

### Logo Registration Hash
The SHA-256 hash of the registered ALU logo (`alu-logo.png`) generated during deployment is mathematically verified to ensure authenticity. The deploy script generates the exact hash; verify it in the deployment output.

## Installation & Setup

1. **Install Node.js**: Ensure Node.js is installed on your computer.
2. **Install Dependencies**: Run the following command in the project root:
   ```bash
   npm install
   ```

## Development and Testing

### Compiling Smart Contracts
To compile the smart contracts, run:
```bash
npx hardhat compile
```

### Running Automated Tests
The project contains 8 automated tests covering ERC-721 registration and ERC-20 tokenization behavior. To execute the tests, run:
```bash
npx hardhat test
```

### Deployment
To deploy the smart contracts to a local Hardhat network and register the logo simultaneously, run:
```bash
npx hardhat run scripts/deploy.js
```

## Versions Used
- **Solidity**: ^0.8.20
- **Hardhat**: (Refer to package.json, typically 2.x)

## Development Notes
- The OpenZeppelin library was utilized to inherit standard ERC-721 and ERC-20 functionalities, ensuring secure and standardized implementations.
