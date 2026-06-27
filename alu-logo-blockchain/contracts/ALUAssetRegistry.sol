// SPDX-License-Identifier: MIT
pragma solidity ^0.8.20;

import "@openzeppelin/contracts/token/ERC721/ERC721.sol";

contract ALUAssetRegistry is ERC721 {
    uint256 private _nextTokenId;

    struct AssetMetadata {
        string name;
        string fileType;
        bytes32 contentHash;
        address registrant;
        uint256 timestamp;
    }

    mapping(uint256 => AssetMetadata) private _assets;
    mapping(bytes32 => bool) private _registeredHashes;

    event AssetRegistered(uint256 indexed tokenId, string name, bytes32 contentHash, address registrant);

    constructor() ERC721("ALU Asset Registry", "ALUAR") {}

    function registerAsset(string memory _name, string memory _fileType, bytes32 _contentHash) public returns (uint256) {
        require(!_registeredHashes[_contentHash], "Hash already registered");

        uint256 tokenId = _nextTokenId;
        _nextTokenId++;
        _safeMint(msg.sender, tokenId);

        _assets[tokenId] = AssetMetadata({
            name: _name,
            fileType: _fileType,
            contentHash: _contentHash,
            registrant: msg.sender,
            timestamp: block.timestamp
        });

        _registeredHashes[_contentHash] = true;

        emit AssetRegistered(tokenId, _name, _contentHash, msg.sender);

        return tokenId;
    }

    function verifyLogoIntegrity(uint256 _tokenId, bytes32 _contentHash) public view returns (bool, string memory) {
        if (_assets[_tokenId].contentHash == _contentHash) {
            return (true, "Logo is authentic.");
        } else {
            return (false, "Warning: logo does not match.");
        }
    }

    function getAsset(uint256 _tokenId) public view returns (AssetMetadata memory) {
        return _assets[_tokenId];
    }
}
