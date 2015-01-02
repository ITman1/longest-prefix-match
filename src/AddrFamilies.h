///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       AddrFamilies.h
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Header file which defines most common address families
//             and info about them.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file AddrFamilies.h
 *
 * @brief Defines most common address families and info about them.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#ifndef ADDRFAMILIES_H
#define ADDRFAMILIES_H

#include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Base class for keeping the informations about address families and
 * providing some conversion helper methods.
 */
class FamilyInfoBase {
public:
	/**
	 * Constructor.
	 * @param family Family of the address.
	 * @param bitLength Length of the address in bits.
	 * @param typeSize Size of the structure which holds address.
	 */
    FamilyInfoBase (uint16_t family, uint8_t bitLength, uint8_t typeSize)
        : _family(family), _bitLength(bitLength), _typeSize(typeSize) {}

    /**
     * Returns address family.
     * @return Address family.
     */
    inline uint16_t getFamily() {
        return _family;
    }

    /**
     * Returns number of bits for address of this family.
     * @return Number of bits for address of this family.
     */
    inline uint16_t getAddrBitLength() const {
        return _family;
    }

    /**
     * Returns size of the structure which holds addresses of this family.
     * @return Size of the structure which holds addresses of this family.
     */
    inline uint16_t getTypeSize() const {
        return _typeSize;
    }

    /**
     * Converts address represented as a string into address structure.
     * @param ip Address as a string.
     * @param addr Address structure.
     * @return Address struture.
     */
    virtual void *ipToAddr(char *ip, void *addr) = 0;

private:
    uint16_t _family;    /**< Family of the address. */
    uint8_t _bitLength;  /**< Length of the address in bits.*/
    uint8_t _typeSize;   /**< Size of the structure which holds address. */
};

/**
 * Typed base class for keeping the informations about address families.
 * Templated by address structure.
 */
template<typename AddrType>
struct FamilyInfo : public FamilyInfoBase {
public:
    FamilyInfo (uint16_t family, uint8_t bitLength, uint8_t typeSize)
        : FamilyInfoBase(family, bitLength, typeSize) {}

    typedef AddrType Addr; /**< Type od the address */
};

/**
 * Class providing informations about IPv4 family.
 */
struct IPv4AddrFamily: public FamilyInfo<struct in_addr> {
public:
    IPv4AddrFamily(): FamilyInfo<struct in_addr>(AF_INET, 32, sizeof(struct in_addr)) {}

    virtual void *ipToAddr(char *ip, void *addr) {
        inet_pton(AF_INET, ip, addr);
        *(uint32_t *)addr = ntohl(*(uint32_t *)addr);
        return addr;
    }
};

/**
 * Class providing informations about IPv6 family.
 */
struct IPv6AddrFamily: public FamilyInfo<struct in6_addr> {
public:
    IPv6AddrFamily(): FamilyInfo<struct in6_addr>(AF_INET6, 128, sizeof(struct in6_addr)) {}

    virtual void *ipToAddr(char *ip, void *addr) {
        inet_pton(AF_INET6, ip, addr);
        for (int i = 0; i < 4; i++) {
            ((uint32_t *)addr)[i] = ntohl(((uint32_t *)addr)[i]);
        }

        return addr;
    }
};

#endif // ADDRFAMILIES_H
