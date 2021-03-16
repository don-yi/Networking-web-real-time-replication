//---------------------------------------------------------
// file:	DigestBuilder.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Builds encoded digests using SHA256 and OpenSSL
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "DigestBuilder.h"


/// <summary>
/// Build an encoded digest from the provided message.
/// </summary>
/// <param name="message">The message to encode.</param>
/// <returns>The message</returns>
std::string DigestBuilder::BuildDigest(const std::string& message)
{
    // create the encoders
    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(md_ctx, EVP_sha256(), NULL);
    EVP_ENCODE_CTX* encode_ctx = EVP_ENCODE_CTX_new();
    EVP_EncodeInit(encode_ctx);

    // build a digest with the messag received
    EVP_DigestUpdate(md_ctx, message.c_str(), message.length());
    auto digest = (unsigned char*)OPENSSL_malloc(EVP_MD_size(EVP_sha256()));
    unsigned int digestSize;
    EVP_DigestFinal_ex(md_ctx, digest, &digestSize); // DigestFinal retrieves the complete digest and resets the OpenSSL object

    // encode the digest
    unsigned int encodedSize = (int)ceil(4.0 * digestSize / 3.0) + 2;
    unsigned char* encodedDigest = (unsigned char*)OPENSSL_malloc(encodedSize);
    EVP_EncodeBlock(encodedDigest, digest, digestSize);

    std::string output = std::string((const char*)encodedDigest);

    // release the resources we created along the way
    OPENSSL_free(encodedDigest);
    OPENSSL_free(digest);

    // release the encoders
    EVP_ENCODE_CTX_free(encode_ctx);
    EVP_MD_CTX_free(md_ctx);

    return output;
}