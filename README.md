ObjectivePGP
============

ObjectivePGP is OpenPGP implementation for iOS and OSX.

See [this post](https://medium.com/@krzyzanowskim/short-story-about-openpgp-for-ios-and-os-x-objectivepgp-9994547d4bea) for full story.

##Installation

###CocoaPods (easy way)

	pod 'ObjectivePGP', :git => 'https://github.com/krzyzanowskim/ObjectivePGP.git', :branch => :master

###Manual (hard way)

1. download library from github
2. Setup Header Search Path (you need setup valid path to headers)
![Sample configuration](http://cl.ly/image/153n3S2H0W2S/objectivepgp-set-headers.png)
3. Link project target to "libObjectivePGP" (`-lObjectivePGP`)
![Sample configuration](http://cl.ly/image/1z2s1O1h0c0F/objectivepgp-link-to-library.png)
4. link target with OpenSSL to satisfy dependency (`-lssl -lcrypto`). To bring OpenSSL to you project you can use precompiled binaries from my other project https://github.com/krzyzanowskim/OpenSSL

##Usage

#####Initialization

	#include <ObjectivePGP.h>
	
	ObjectivePGP *pgp = [[ObjectivePGP alloc] init];
	
#####Load keys (private or public)

	/* From file */
	[pgp importKeysFromFile:@"/path/to/secring.gpg"];
	[pgp importKeysFromFile:@"/path/to/key.asc"];
	
	/* Load single key from keyring */
	[pgp importKey:@"979E4B03DFFE30C6" fromFile:@"/path/to/secring.gpg"];
	
#####Search for keys

	/* long identifier 979E4B03DFFE30C6 */
	PGPKey *key = [pgp getKeyForIdentifier:@"979E4B03DFFE30C6" type:PGPKeyPublic];
	
	/* short identifier 979E4B03 (the same result as previous) */
	PGPKey *key = [pgp getKeyForIdentifier:@"979E4B03" type:PGPKeyPublic];
	
	/* first key that match given user */
	PGPKey *key = [pgp getKeysForUserID:@"Name <email@example.com>"];
	
#####Export keys (private or public)

	NSError *exportError = nil;
	
	/* export all public keys to file */
	BOOL result = [pgp exportKeysOfType:PGPKeyPublic toFile:@"pubring.gpg" error:&exportError];
	if (result) {
		NSLog(@"success");
	}
	
	PGPKey *myPublicKey = [self.oPGP getKeyForIdentifier:@"979E4B03DFFE30C6" type:PGPKeyPublic];
	
	/* export public key and save as armored (ASCII) file */
	NSData *armoredKeyData = [pgp exportKey:myPublicKey armored:YES];
	[armoredKeyData writeToFile:@"pubkey.asc" atomically:YES];

#####Sign data (or file)

	NSData *fileContent = [NSData dataWithContentsOfFile:@"/path/file/to/data.txt"];

	/* choose key to sign */
	PGPKey *keyToSign = [self.oPGP getKeyForIdentifier:@"979E4B03DFFE30C6" type:PGPKeySecret];

	/* sign and return signature (detached = YES) */
	NSData *signature = [pgp signData:fileContent usingSecretKey:keyToSign passphrase:nil detached:YES];

	/* sign and return signed data (detached = NO) */
	NSData *signedData = [pgp signData:fileContent usingSecretKey:keyToSign passphrase:nil detached:NO];
	
#####Verify signature from data (or file)

	/* embedded signature */
	NSData *signedContent = [NSData dataWithContentsOfFile:@"/path/file/to/data.signed"];
	if ([pgp verifyData:signedContent]) {
		NSLog(@"Verification success");
	}
	
	/* detached signature */
	NSData *signatureContent = [NSData dataWithContentsOfFile:@"/path/file/to/signature"];
	NSData *dataContent = [NSData dataWithContentsOfFile:@"/path/file/to/data.txt"];
	if ([pgp verifyData:dataContent withSignature:signatureContent]) {
		NSLog(@"Verification success");
	}
	
#####Encrypt data with previously loaded public key

    NSError *error = nil;

	NSData *fileContent = [NSData dataWithContentsOfFile:@"/path/file/to/data.txt"];
    
	/* public key to encrypt data, must be loaded previously */
	PGPKey *keyToEncrypt = [self.oPGP getKeyForIdentifier:@"979E4B03DFFE30C6" type:PGPKeyPublic];

	/* encrypt data, armor output (ASCII file)  */
	NSData *encryptedData = [pgp encryptData:fileContent usingPublicKey:keyToEncrypt armored:YES error:&error];
	if (encryptedData && !error) {
		NSLog(@"encryption success");
		[encryptedData writeToFile:@"/path/to/encrypted/file.gpg" atomically:YES]
	}


#####Decrypt data with previously loaded private key
    
	NSData *encryptedFileContent = [NSData dataWithContentsOfFile:@"/path/file/to/data.gpg"];
	
	/* need provide passphrase if required */
    NSError *error = nil;
	NSData *decryptedData = [pgp decryptData:encryptedFileContent passphrase:nil error:&error];
	if (encryptedData && !error) {
		NSLog(@"decryption success");
	}
	
##opgp - command line tool

For you convenience I've build command line tool (OS X) - [opgp](https://github.com/krzyzanowskim/ObjectivePGP/blob/master/usr/local/bin/opgp) that uses this library, so you can play with it easily.

If you want to install this tool, simply copy file to `/usr/local/bin` directory on you mac.

	Usage: opgp [-encrypt] [-key keyfile.asc] [-armor 1] [-msg "message"] ...
	Options:
		-decrypt     Decrypt mode (Default)
		-encrypt     Encrypt mode 
		-input       file.txt - path or URL to input file
		-msg         "message" - input text
		-keyid       [28A83333F9C27197|F9C27197] - public or secret key identifier (Optional if "-key" is specified)
		-key         key.asc - public or secret key file
		-output      file.txt.gpg - output path (Optional)
		-pubring     [pubring.gpg|pubring.asc] - keyring with public keys (Optional)
		-secring     [secring.gpg|secring.asc] - keyring with public keys (Optional)
		-passphrase  12345 - secret key password (Optional)
		-armor       [1|0] - output format (Optional)
		-help        Help
		-license     License
	
##Release notes

Known issues (0.1)

- Embeded signatures are not supported
- ZIP compression not fully
- Multiple keys for single encoding - not supported
- Old encrypted packets - sometimes not supported
- Only RSA cipher is fully supported
- Defaults hardcoded


###Contact

Follow me on Twitter ([@krzyzanowskim](http://twitter.com/krzyzanowskim))

###Creator

Marcin Krzyżanowski ([@krzyzanowskim](http://twitter.com/krzyzanowskim))

#License

```
Copyright (C) 2014 Marcin Krzyżanowski
This software is provided 'as-is', without any express or implied warranty. 

In no event will the authors be held liable for any damages arising from the use of this software. 

Permission is granted to anyone to use this software for any purpose,including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

- The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation is required.
- Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
- This notice may not be removed or altered from any source or binary distribution.
```