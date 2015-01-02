///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       AddrTrieBase.h
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Header file which defines searching trie base class.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file AddrTrieBase.h
 *
 * @brief Defines searching trie base class.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef ADDRTRIEBASE_H
#define ADDRTRIEBASE_H

#include <cstring>

#include "AddrFamilies.h"
#include "TrieNode.h"

using namespace std;

/**
 * CLass of the searching trie.
 */
class AddrTrieBase
{
public:
	/**
	 * Constructs address search trie.
	 * @param familyInfo Informations about addresses which will this trie accept.
	 */
    AddrTrieBase(FamilyInfoBase *familyInfo);

    /**
     * Destructor of the trie.
     */
    virtual ~AddrTrieBase();

    /**
     * Inserts new ASN into trie.
     * @param addrStr Address in string representation which should be inserted.
     * @param prefix Defines how many bits should be stored into trie.
     * @param asn Number of the autonomous system to be stored in destination node.
     * @return True if no error occurs, else false.
     */
    inline void insertASN(char *addrStr, int prefix, char *asn)
    {
        familyInfo->ipToAddr(addrStr, _addr);
        insertASN(_addr, prefix, asn);
    }

    /**
     * Inserts new ASN into trie.
     * @param addr Address which should be inserted.
     * @param prefix Defines how many bits should be stored into trie.
     * @param asn Number of the autonomous system to be stored in destination node.
     * @return True if no error occurs, else false.
     */
    inline bool insertASN(uint32_t *addr, int prefix, char *asn) {
        TrieNode *currNode = rootNode;
        TrieNode *childNode;
        register uint32_t ip_seg = 0;

        /* Iterates through all bits in the address. */
        for (int i = 0; i < prefix; i++) {

            if (i % 32 == 0) {  // New address segment reached, load it into ip_seg
                ip_seg = *addr;
            }

            if (ip_seg & 0x80000000) { // MSB is set, then right child node will be used for iteration
                childNode = currNode->getRightChild();
                if (childNode == 0) {  // There is no right child yet, create a new one
                    childNode = new TrieNode();
                    currNode->setRightChild(childNode);
                }
                currNode = childNode;
            } else {                  // MSB is not set, then left child node will be used for iteration
                childNode = currNode->getLeftChild();
                if (childNode == 0) { // There is no left child yet, create a new one
                    childNode = new TrieNode();
                    currNode->setLeftChild(childNode);
                }
                currNode = childNode;
            }

            ip_seg <<= 1;       // Shift address segment left - new MSB will be tested in the next iteration

            if (i % 32 == 31) { // End of the segment reached move to the next one
                addr++;
            }
        }

        if (currNode->getValue() != 0) { // Return fail whether destination node contains some value
            return false;
        } else {                         // Insert new AS whether destination node is empty
            char *val = new char[ASN_STR_MAXLEN + 1];
            strncpy(val, asn, ASN_STR_MAXLEN);
            currNode->setValue(val);
            return true;
        }
    }

    /**
     * Searches address inside the trie and tries to find the corresponding ASN.
     * @param addrStr Address in string representation which should be searched.
     * @return Found number of the autonomous system on successful searching, or -1 if no address matched.
     */
    inline char *longestPrefixMatch(char *addrStr) {
        return longestPrefixMatch((uint32_t *)familyInfo->ipToAddr(addrStr, _addr));
    }

    /**
     * Searches address inside the trie and tries to find the corresponding ASN.
     * @param addr Address which should be searched.
     * @return Found number of the autonomous system on successful searching, or -1 if no address matched.
     */
    inline char *longestPrefixMatch(uint32_t *addr) {
        TrieNode *currNode = rootNode;
        register uint32_t ip_seg = 0;
        char *ret_value = NULL;

        /* Iterates through all bits in the address. */
        for (int i = 0; 1; i++) {

            if (i % 32 == 0) {         // New address segment reached, load it into ip_seg
                ip_seg = *addr;
            }

            if (ip_seg & 0x80000000) { // MSB is et, try to walk through the right child
                currNode = static_cast<TrieNode *>(currNode->getRightChild());
            } else {                   // MSB is not set, try to walk through the left child
                currNode = static_cast<TrieNode *>(currNode->getLeftChild());
            }

            if (currNode == 0) {       // There is no path, return with fail
                break;
            }

            if (currNode->getValue() != 0) { // We have found the currently longest corresponding ASN, but continue and try to find a better one
                ret_value = static_cast<char *>(currNode->getValue());
            }

            ip_seg <<= 1;             // Shift address segment left - new MSB will be tested in the next iteration

            if (i % 32 == 31) {       // End of the segment reached move to the next one
                addr++;
            }
        }

        return ret_value;            // Found ASN
    }

    /**
     * Returns information about family of addresses which are stored inside this trie
     */
    inline const FamilyInfoBase &getFamilyInfo() {
        return *familyInfo;
    }

    /**
     * Clears all trie/removes from the memory.
     */
    void clear();

    const static int ASN_STR_MAXLEN;

protected:
    TrieNode *rootNode;          /**< Pointer to root node */
    FamilyInfoBase *familyInfo;  /**< Stored informations about used address family */

    /**
     * Clears all nodes recursive from the specified node.
     * @param node Node from which should be all other nodes removed.
     */
    void clearProtected(TrieNode *node);

private:
    uint32_t *_addr;            /**< Helper address variable used for computing */
};

#endif // ADDRTRIEBASE_H
