# PGP
**Pretty Good Privacy**의 약자로, 컴퓨터 파일을 암호화하고 복호화하는 프로그램입니다.  
Visual Studio 2019 기반으로 코드를 완성했고, **PGP** 동작 과정을 콘솔에 표현했습니다.  
사용자 입력은 [PGP Transmission Mode](https://github.com/korkeep/PGP#pgp-transmission-mode)의 *Step 3*  단계에서 RSA key를 생성할 때 한 번 필요합니다.  

## Crypto Algorithm
| Algorithm | Description |
| --- | --- |
| SDES | Symmetric Key Algorithm |
| RSA | Public Key Algorithm |
| MD5 | Hash Digest Algorithm |

## PGP Transmission mode
- Step 1: Read Text.txt
- Step 2: Message Digest using MD5 Algorithm
- Step 3: Encrypt Digest with Sender Private Key (MAC)
- Step 4: Concat Origin Content & Encrypted MAC
- Step 5: Encrypt Concatated Data with Symmetric Key
- Step 6: Encrypt Symmetric Key with Receiver Public Key
- Step 7: Concat Encrypted Data & Write EText.txt

## PGP Receiving mode
- Step 1: Read EText.txt
- Step 2: Decrypt Symmetric Key with Receiver Private Key
- Step 3: Decrypt Encrypted Data with Symmetric Key
- Step 4: Decrypt MAC with Sender Public Key
- Step 5: Parse Plain Text Data
- Step 6: Hash Plain Text Data & Compare with MAC
- Step 7: Write DText.txt