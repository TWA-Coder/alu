import { BrowserProvider, Contract } from 'ethers';

// --- CONFIGURATION ---
// IMPORTANT: Replace this with your actual deployed ALUAssetRegistry contract address
const contractAddress = "0xCf7Ed3AccA5a467e9e704C703E8D87F634fB0Fc9";

const abi = [
  "function registerAsset(string _name, string _fileType, bytes32 _contentHash) public returns (uint256)",
  "function verifyLogoIntegrity(uint256 _tokenId, bytes32 _contentHash) public view returns (bool, string)",
  "function getAsset(uint256 _tokenId) public view returns (tuple(string name, string fileType, bytes32 contentHash, address registrant, uint256 timestamp))",
  "event AssetRegistered(uint256 indexed tokenId, string name, bytes32 contentHash, address registrant)"
];

// --- APP STATE ---
let provider;
let signer;
let contract;

// --- DOM ELEMENTS ---
const connectWalletBtn = document.getElementById('connectWalletBtn');
const statusMessage = document.getElementById('statusMessage');
const verifySection = document.getElementById('verifySection');

const registerForm = document.getElementById('registerForm');
const registerBtn = document.getElementById('registerBtn');

const verifyForm = document.getElementById('verifyForm');
const verifyBtn = document.getElementById('verifyBtn');
const verifyResult = document.getElementById('verifyResult');

// --- HELPER FUNCTIONS ---
function showMessage(msg, isError = false) {
  statusMessage.textContent = msg;
  statusMessage.className = `status-message ${isError ? 'error' : 'success'}`;
  statusMessage.classList.remove('hidden');
}

// --- CORE LOGIC ---

async function connectWallet() {
  if (window.ethereum == null) {
    showMessage("MetaMask not installed; please install it to use this dApp.", true);
    return;
  }

  try {
    // Connect to the MetaMask EIP-1193 object
    provider = new BrowserProvider(window.ethereum);
    
    // Request account access
    const accounts = await provider.send("eth_requestAccounts", []);
    signer = await provider.getSigner();
    
    // Initialize Contract instance
    contract = new Contract(contractAddress, abi, signer);

    connectWalletBtn.textContent = `Connected: ${accounts[0].substring(0, 6)}...${accounts[0].substring(38)}`;
    connectWalletBtn.disabled = true;
    
    showMessage(`Wallet connected successfully! Active network: ${(await provider.getNetwork()).name}`);
    verifySection.classList.remove('hidden'); // Show verify section once connected
    
  } catch (error) {
    console.error("Connection error:", error);
    showMessage(`Failed to connect wallet: ${error.message}`, true);
  }
}

async function handleRegister(e) {
  e.preventDefault();
  if (!contract) {
    showMessage("Please connect your wallet first.", true);
    return;
  }

  const name = document.getElementById('regName').value;
  const fileType = document.getElementById('regFileType').value;
  const contentHash = document.getElementById('regHash').value;

  try {
    registerBtn.disabled = true;
    registerBtn.textContent = "Registering...";
    showMessage("Sending transaction... please approve in MetaMask.");

    const tx = await contract.registerAsset(name, fileType, contentHash);
    showMessage(`Transaction sent! Waiting for confirmation... Hash: ${tx.hash}`);

    const receipt = await tx.wait();
    
    // Look for AssetRegistered event to get the Token ID
    // ethers v6 parsing
    let tokenId = "Unknown (Check transaction)";
    for (const log of receipt.logs) {
        try {
            const parsedLog = contract.interface.parseLog(log);
            if (parsedLog && parsedLog.name === "AssetRegistered") {
                tokenId = parsedLog.args.tokenId.toString();
            }
        } catch (e) {
            // Ignore logs that don't belong to this contract
        }
    }

    showMessage(`Asset registered successfully! Your Logo Token ID is: ${tokenId}`);
    registerForm.reset();
  } catch (error) {
    console.error("Registration error:", error);
    showMessage(`Error: ${error.reason || error.message}`, true);
  } finally {
    registerBtn.disabled = false;
    registerBtn.textContent = "Register Logo";
  }
}

async function handleVerify(e) {
  e.preventDefault();
  if (!contract) {
    showMessage("Please connect your wallet first.", true);
    return;
  }

  const tokenId = document.getElementById('veriTokenId').value;
  const contentHash = document.getElementById('veriHash').value;

  try {
    verifyBtn.disabled = true;
    verifyBtn.textContent = "Verifying...";
    verifyResult.className = "result-box hidden"; // Reset result box

    const [isAuthentic, message] = await contract.verifyLogoIntegrity(tokenId, contentHash);
    
    verifyResult.classList.remove('hidden');
    if (isAuthentic) {
      verifyResult.className = "result-box authentic";
      verifyResult.innerHTML = `<strong>✅ Authentic!</strong><br>${message}`;
    } else {
      verifyResult.className = "result-box fake";
      verifyResult.innerHTML = `<strong>❌ Verification Failed!</strong><br>${message}`;
    }

  } catch (error) {
    console.error("Verification error:", error);
    showMessage(`Error: Contract might not have this token ID or ${error.message}`, true);
  } finally {
    verifyBtn.disabled = false;
    verifyBtn.textContent = "Verify";
  }
}

// --- EVENT LISTENERS ---
connectWalletBtn.addEventListener('click', connectWallet);
registerForm.addEventListener('submit', handleRegister);
verifyForm.addEventListener('submit', handleVerify);
