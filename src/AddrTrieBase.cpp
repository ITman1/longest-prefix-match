///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       AddrTrieBase.cpp
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Source file implementing some methods of the base class for address search tries.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file AddrTrieBase.cpp
 *
 * @brief Implements some methods for address search tries.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <cstdlib>

#include "AddrTrieBase.h"

/*
 * Maximum string length for ASN.
 * 32 bit number (11 characters) + termination character
 */
const int AddrTrieBase::ASN_STR_MAXLEN = 11;

/**
 * Constructs address search trie.
 * @param familyInfo Informations about addresses which will this trie accept.
 */
AddrTrieBase::AddrTrieBase(FamilyInfoBase *familyInfo) : familyInfo(familyInfo)
{
    rootNode = new TrieNode();
    // Allocates space for uint, even it is not necessary, it could be better because no alignment would be used
    int addrSize = (familyInfo->getTypeSize() / 4) + (familyInfo->getTypeSize() % 4 != 0);
    _addr = new uint32_t[addrSize]();
}

/**
 * Destructor of the trie.
 */
AddrTrieBase::~AddrTrieBase()
{
    clear();
    delete[] _addr;
    delete familyInfo;
}

/**
 * Clears all trie/removes from the memory.
 */
void AddrTrieBase::clear() {
    clearProtected(rootNode);
    rootNode = 0;
}

/**
 * Clears all nodes recursive from the specified node.
 * @param node Node from which should be all other nodes removed.
 */
void AddrTrieBase::clearProtected(TrieNode *node) {
    if (node != 0) {
        clearProtected(node->getLeftChild());
        clearProtected(node->getRightChild());

        char *value = static_cast<char *>(node->getValue());
        if (value != 0) {
            delete[] value;
        }

        delete node;
    }
}
