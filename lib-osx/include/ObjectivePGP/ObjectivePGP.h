//
//  ObjectivePGP.h
//  ObjectivePGP
//
//  Created by Marcin Krzyzanowski on 03/05/14.
//  Copyright (c) 2014 Marcin Krzyżanowski. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PGPTypes.h"
#import "PGPKey.h"

@interface ObjectivePGP : NSObject

/**
 *  Array of PGPKey
 */
@property (strong, nonatomic) NSArray *keys;

// Load from keyring
- (NSArray *) importKeysFromFile:(NSString *)path;
- (NSArray *) importKeysFromData:(NSData *)data;
- (BOOL) importKey:(NSString *)shortKeyStringIdentifier fromFile:(NSString *)path;

// Save to keyring
- (BOOL) exportKeysOfType:(PGPKeyType)type toFile:(NSString *)path error:(NSError * __autoreleasing *)error;
- (BOOL) exportKeys:(NSArray *)keys toFile:(NSString *)path error:(NSError * __autoreleasing *)error;
- (NSData *) exportKey:(PGPKey *)key armored:(BOOL)armored;

- (PGPKey *) getKeyForIdentifier:(NSString *)keyIdentifier type:(PGPKeyType)keyType;
- (PGPKey *) getKeyForKeyID:(PGPKeyID *)searchKeyID type:(PGPKeyType)keyType;
- (NSArray *) getKeysForUserID:(NSString *)userID;
- (NSArray *) getKeysOfType:(PGPKeyType)keyType;

- (NSData *) signData:(NSData *)dataToSign usingSecretKey:(PGPKey *)secretKey passphrase:(NSString *)passphrase;
- (NSData *) signData:(NSData *)dataToSign usingSecretKey:(PGPKey *)secretKey passphrase:(NSString *)passphrase detached:(BOOL)detached;
- (NSData *) signData:(NSData *)dataToSign withKeyForUserID:(NSString *)userID passphrase:(NSString *)passphrase;
- (NSData *) signData:(NSData *)dataToSign withKeyForUserID:(NSString *)userID passphrase:(NSString *)passphrase detached:(BOOL)detached;

- (BOOL) verifyData:(NSData *)signedData;
- (BOOL) verifyData:(NSData *)signedData withSignature:(NSData *)signatureData;
- (BOOL) verifyData:(NSData *)signedData withSignature:(NSData *)signatureData usingKey:(PGPKey *)publicKey;

- (NSData *) encryptData:(NSData *)dataToEncrypt usingPublicKey:(PGPKey *)publicKey armored:(BOOL)armored error:(NSError * __autoreleasing *)error;
- (NSData *) decryptData:(NSData *)messageDataToDecrypt passphrase:(NSString *)passphrase error:(NSError * __autoreleasing *)error;

@end
