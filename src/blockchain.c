#include <stdio.h>
// #include "..include/blockchain.h"
#include "blockchain.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0) // Create hash with SHA256 same way we did in lab

void hashFunc(struct block_core *theBlock,
              unsigned char hash[SHA256_DIGEST_LENGTH]) {
  EVP_MD_CTX *mdctx = NULL;
  unsigned int len = 0;
  // EVP_MD *sha256 = NULL;

  mdctx = EVP_MD_CTX_new();
  if (mdctx == NULL) {
    handle_error("evp md ctx new");
  }
  // sha256 = EVP_MD_fetch(NULL, "SHA256", NULL);
  // if (sha256 == NULL){
  //   handle_error("sha");
  // }

  if (!EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
    handle_error("digest init ex");
  }
  if (!EVP_DigestUpdate(mdctx, theBlock, sizeof(struct block_core))) {
    handle_error("update");
  }

  if (!EVP_DigestFinal_ex(mdctx, hash, &len)) {
    handle_error("digest final");
  }
  EVP_MD_CTX_free(mdctx);
}

int bc_init(struct blockchain *bc,
            unsigned char difficulty[SHA256_DIGEST_LENGTH]) {
  // initializes the blockchain it should iniialize the buffer passed as the
  // arguement and use the hash target also passed as an arguement (difficulty).
  // If difficulty is null it means theres no target hash. Func returns 0 for
  // success -1 for failure.
  if (bc == NULL) {
    return -1;
    // handle_error("bc");
  }

  // initialize block chain
  memset(bc, 0, sizeof(struct blockchain));
  bc->count = 0;
  if (difficulty != NULL) {
    memcpy(bc->difficulty, difficulty, SHA256_DIGEST_LENGTH);
  } else {
    // if no difficulty set to max bc point of difficulty is <= target hash and
    // if its the max no effect
    memset(bc->difficulty, 255, SHA256_DIGEST_LENGTH);
  }
  return 0;
}

int bc_add_block(struct blockchain *bc, const unsigned char data[DATA_SIZE]) {

  // adds a new block to the blockchain. should create a new block, set the
  // fields of the block and find the correct nonce that satisfies the hash
  // condition. it should then add the block to the blockchain. it should return
  // 0 for sucess -1 for failure
  if (bc == NULL || data == NULL) {
    // handle_error("start bc add block");
    return -1;
  }
  struct block *newBlock = NULL;
  newBlock = &bc->blocks[bc->count];

  if (bc->count == 0) {
    newBlock->core.index = 0;
  } else {
    newBlock->core.index = bc->blocks[bc->count - 1].core.index + 1;
  }

  // use clockget time
  clock_gettime(CLOCK_REALTIME, &newBlock->core.timestamp);

  // copy user given data in block
  memcpy(newBlock->core.data, data, DATA_SIZE);

  // Set the previous hash to currentBlock in its variable
  if (bc->count > 0) {
    memcpy(newBlock->core.p_hash, bc->blocks[bc->count - 1].hash,
           SHA256_DIGEST_LENGTH);
  } else {
    memset(newBlock->core.p_hash, 0, SHA256_DIGEST_LENGTH);
  }

  uint32_t nonce = 0;
  int nonceFound = 0;

  while (nonceFound == 0) {
    newBlock->core.nonce = nonce;
    nonceFound = 1;
    hashFunc(&newBlock->core, newBlock->hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      if (newBlock->hash[i] < bc->difficulty[i]) {
        break;
        // nonce++;
      } else if (newBlock->hash[i] > bc->difficulty[i]) {
        nonceFound = 0;
        break;
      }
    }

    if (nonceFound == 0) {
      nonce++;
    }
  }
  bc->count++;
  return 0;
  //}
}

int bc_verify(struct blockchain *bc) {
  // verifies the blockchain. it should check if the hash of each block is
  // correct and matches the p_hash field of the next block. It should also
  // check if the hash of each block satisfies the difficulty condition. It
  // should return 0 for success and -1 for failure.
  if (bc == NULL) {
    return -1;
  }
  // verift block chain
  for (size_t i = 0; i < bc->count; i++) {

    struct block *currentBlock = &bc->blocks[i];

    // get the hash of the current block and store it in buffer to compare
    unsigned char verifyHash[SHA256_DIGEST_LENGTH];
    hashFunc(&currentBlock->core, verifyHash);

    if (memcmp(verifyHash, currentBlock->hash, SHA256_DIGEST_LENGTH) != 0) {
      return -1;
    }
    int followsCondition = 1;
    // check difficulty ensure it follows condition
    for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
      if (currentBlock->hash[j] < bc->difficulty[j]) {
        // followsCondition = 0;
        break;
      } else if (currentBlock->hash[j] > bc->difficulty[j]) {
        followsCondition = 0;
        break;
      }
    }
    if (followsCondition == 0) {
      return -1;
    }

    // check that the current blocks hash matches the next blocks hash
    if (i < bc->count - 1) {
      if (memcmp(currentBlock->hash, bc->blocks[i + 1].core.p_hash,
                 SHA256_DIGEST_LENGTH) != 0) {
        return -1;
      }
    }
  }
  return 0;
}
