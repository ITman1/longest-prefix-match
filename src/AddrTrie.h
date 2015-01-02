///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       AddrTrie.h
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Header file which defines typed searching trie
///////////////////////////////////////////////////////////////////////////////

/**
 * @file AddrTrie.h
 *
 * @brief Defines typed searching trie
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef ADDRTRIE_H
#define ADDRTRIE_H

#include "AddrTrieBase.h"

/**
 * Templated class of the searching trie which is constructed exactly for the passed Address family.
 */
template<class AddrFamily>
class AddrTrie : public AddrTrieBase {
public:
	/**
	 * Constructs new trie.
	 */
    AddrTrie() : AddrTrieBase(new AddrFamily()) {}
    virtual ~AddrTrie() {}

    /**
     * Inserts new ASN into trie.
     * @param addr Address which should be inserted.
     * @param prefix Defines how many bits should be stored into trie.
     * @param asn Number of the autonomous system to be stored in destination node.
     * @return True if no error occurs, else false.
     */
    inline bool insertASN(typename AddrFamily::Addr *addr, int prefix, char *asn) {
        return AddrTrieBase::insertASN(addr, prefix, asn);
    }

    /**
     * Searches through the trie and tries to find the corresponding autonomous system.
     * @param addr Address which should be searched.
     * @return Found number of the autonomous system on successful searching, or -1 if no address matched.
     */
    inline char *longestPrefixMatch(typename AddrFamily::Addr *addr) {
        return AddrTrieBase::longestPrefixMatch(addr);
    }
};

#endif // ADDRTRIE_H
