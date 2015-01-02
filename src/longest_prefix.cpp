///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       longest_prefix.cpp
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Executable module of the project, performs Longest-Prefix Match
///////////////////////////////////////////////////////////////////////////////

/**
 * @file TrieNode.h
 *
 * @brief Defines trie node.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>         

#include "AddrTrie.h"

using namespace std;

/**
 * Enumeration of the flags which accepts this program
 */
enum flags {
    ASN_FILE = 'i'     /**< Input file with AS numbers */
           };

enum errors {
    ERR_ARGUMENTS = 1, /**< Error on input arguments. */
    ERR_FILE = 2       /**< Error on file handling. */
           };

const string MSG_ERR_NO_ASN_FILE = "Error: No input file with ASN records specified!";
const string MSG_WRN_ARG_GARBAGE = "Warning: Some arguments has been skipped!";
const string MSG_WRN_UNKNOWN_OPTION = "Warning: Unknown option flag: ";
const string MSG_ERR_ARG_INTERFACE_MISSING =
        "Error: No file with autonomous system has been passed!";
const string MSG_ERR_FILE_OPEN =
        "Error: Unable to open/read file with mapping of the autonomous systems!";
const string MSG_WRN_MISSING_ARGUMENT =
        "Warning: Missing argument to option flag: ";
const string MSG_ERR_STDOUT_IO = "Error: Unable to write on stdout!";

/**
 * Help message which will be printed on stdout when error occurs.
 */
const string HELP = "PDS - Longest prefix match\n"
                    "Použití:\n"
                    "  \tlpm -i <název_asn_souboru>\n"
                    "\n"
                    "Přepínače:\n"
                    "-i\t- název souboru s AS záznamy pro IP adresy";

/**
 * Filter/Mask string for getopt function.
 */
static const string GETOPT_STRING = "i:";

/**
 * Read block buffer for IO operations.
 */
const static int RBUFFER_SIZE = 65536;
static char block_rbuffer[RBUFFER_SIZE] = {0};

/**
 * Write block buffer for IO operations.
 */
const static int WBUFFER_SIZE = 65536;
static char block_wbuffer[WBUFFER_SIZE] = {0};

/**
  * Length of the line which is accepted during IO operations.
  */
const static int MAX_LINE_LENGTH = 512;

/**
 * Gets flags and arguments ryped on command line.
 * @param argc number of parameters
 * @param argv Array with parameters
 * @param flags Return mapping array with flags and corresponding arguments
 */
map<char, string> &getFlags(int argc, char **argv, map<char, string> &flags) {
    char ch;
    string optargString;

    opterr = 0;
    // Processing cmd line parameters
    while ((ch = getopt(argc, argv, GETOPT_STRING.c_str())) != -1) {
        switch (ch) {
            // known parameter
        case ASN_FILE:
            optargString = (!optarg) ? string() : optarg; // getting argument whether has
            flags.insert(pair<char, string>(ch, optargString)); // storing to map array
            break;
            // unknown flag
        case '?':
            cerr << MSG_WRN_UNKNOWN_OPTION << char(optopt) << endl;
            break;
            // missing argument
        case ':':
            cerr << MSG_WRN_MISSING_ARGUMENT << char(optopt) << endl;
            if ((flags.count('i')) && (flags[optopt].empty())) {
                flags.erase(optopt);
            }
            break;
        }

    }

    // there were some parameter which were skipped
    if (optind < argc) {
        cerr << MSG_WRN_ARG_GARBAGE << endl;
    }

    return flags;
}

/**
  * Rebuilds ASN buffer from specified position where each item of ASN record were delimited by '\0'
  * @param fromPosition Position from where should be buffer restored
  */
inline void rebuildASNBuffer(char *fromPosition) {
    char *buffEnd = &block_rbuffer[RBUFFER_SIZE - 1];
    char *buffChar = fromPosition;
    int state = 0;
    while (buffChar != buffEnd) {
        if (*buffChar == '\0') {
            switch (state) {
            case 0:
                *buffChar = '/';
                break;
            case 1:
                *buffChar = ' ';
                break;
            case 2:
                *buffChar = '\n';
                state = 0;
                break;
            }
            state++;
        }
        buffChar++;
    }
}

/**
 * Fastly loads numbers of autonomous systems into IPv4 and IPv6 tries.
 * @param filename Filename of the file with AS numbers.
 * @param ipv4Trie Trie where IPv4 to ASN mapping will be stored.
 * @param ipv6Trie Trie where IPv6 to ASN mapping will be stored.
 */
bool loadTrieFromFile(const string &filename,
                      AddrTrie<IPv4AddrFamily> &ipv4Trie,
                      AddrTrie<IPv6AddrFamily> &ipv6Trie) {

    int asnFile = open(filename.c_str(), O_RDONLY);
    if (asnFile == -1) {
        return false;
    }

    char empty_str[1] = {'\0'};

    char *buffEnd = &block_rbuffer[RBUFFER_SIZE - 1];
    ssize_t read_bytes = 0;
    size_t head_bytes = 0;
    size_t tail_bytes = 1;
    size_t buff_size = RBUFFER_SIZE - tail_bytes - head_bytes;
    while ((read_bytes = read(asnFile, &block_rbuffer[head_bytes], buff_size)) > 0) {

        block_rbuffer[head_bytes + read_bytes] = '\0'; // Make string from the mapped array

        char *ipChars = block_rbuffer; // Start pointer address of IP address is identical with the start of the block
        char *prefixChars = empty_str;
        char *asnChars = empty_str;
        char *buffChar = block_rbuffer; // Set pointer to char which we will iterate
        int prefix = 0;

        AddrTrieBase *currTrie = &ipv4Trie;

        // Parsing loop
        while (*buffChar != '\0') {
            switch (*buffChar) {
            case '/': // Begin of processing prefix number
                prefixChars = buffChar + 1;
                *buffChar = '\0'; // We ended parsing of the IP address
                break;
            case ' ': // Begin of processing ASN
                asnChars = buffChar + 1;
                *buffChar = '\0'; // We ended parsing of the prefix
                break;
            case '\n': // End of string, so end of parsing
                *buffChar = '\0'; // We ended parsing of the prefix

                /* Convert prefix number and ASN into numerical representation */
                prefix = atoi(prefixChars);

                // Insert new record into trie
                currTrie->insertASN(ipChars, prefix, asnChars);

                ipChars = buffChar + 1;
                currTrie = &ipv4Trie;
                break;
            case ':': // We are processing IPv6 address, so set IPv6 trie
                currTrie = &ipv6Trie;
                break;
            }
            buffChar++; // Move on next character
        }

        rebuildASNBuffer(ipChars);

        head_bytes = strlen(ipChars);
        memmove(block_rbuffer, ipChars, buffEnd - ipChars);

        buff_size = RBUFFER_SIZE - tail_bytes - head_bytes;
    }

    close(asnFile);
    return read_bytes != -1;
}

/**
  * Flushes IO block buffer.
  * @param fd File descriptor where should be buffer written.
  * @param toPosition Position to which should be buffer flushed.
  */
inline bool flushBuffer(int fd, char *toPosition) {
    return write(fd, block_wbuffer, toPosition - block_wbuffer) != -1;
}

/**
 * Performs searching of the IP addresses which are put on the stdin.
 * @param ipv4Trie Searching trie for IPv4 addresses.
 * @param ipv6Trie Searching trie for IPv6 addresses.
 */
bool performSearching(AddrTrie<IPv4AddrFamily> &ipv4Trie,
                      AddrTrie<IPv6AddrFamily> &ipv6Trie) {

    char *wbuffChar = block_wbuffer;
    char *wbuffEnd = &block_wbuffer[RBUFFER_SIZE - 1];
    *wbuffEnd = '\0';

    ssize_t read_bytes = 0;
    size_t head_bytes = 0;
    size_t tail_bytes = 1;
    size_t buff_size = RBUFFER_SIZE - tail_bytes - head_bytes;
    while ((read_bytes = read(STDIN_FILENO, &block_rbuffer[head_bytes], buff_size)) > 0) {

        block_rbuffer[head_bytes + read_bytes] = '\0'; // Make string from the mapped array

        char *lineChars = block_rbuffer; // Start pointer address of IP address is identical with the start of the block
        char *buffChar = block_rbuffer; // Set pointer to char which we will iterate

        // Parsing loop
        while (*buffChar != '\0') {
            AddrTrieBase *currTrie = &ipv4Trie;

            /* Remove new line character on the address string */
            while (*buffChar != '\n') {
                switch (*buffChar) {
                case ':':
                    currTrie = &ipv6Trie;
                    break;
                case '\0':
                    goto end_outerloop;
                }
                buffChar++;
            }
            *buffChar = '\0';

            char *matched = currTrie->longestPrefixMatch(lineChars);

            if (matched == NULL) {     // No match found, print -
                *wbuffChar++ = '-';
                *wbuffChar++ = '\n';
            } else {               // Match found, print corresponding ASN into write buffer
                while (*matched != '\0') {
                    *wbuffChar++ = *matched++;
                }
                *wbuffChar++ = '\n';
            }

            // Block buffer is getting full, perform write() syscall
            if (wbuffChar + MAX_LINE_LENGTH > wbuffEnd) {
                if (!flushBuffer(STDOUT_FILENO, wbuffChar)) {
                    return false;
                }
                wbuffChar = block_wbuffer;
            }

            buffChar++;           // Move on next character
            lineChars = buffChar; // Next character will be also new line
        }
        end_outerloop:

        head_bytes = strlen(lineChars);
        strcpy(block_rbuffer, lineChars);

        buff_size = RBUFFER_SIZE - tail_bytes - head_bytes;
    }

    // Flush rest of the block buffer
    if (!flushBuffer(STDOUT_FILENO, wbuffChar)) {
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    map<char, string> flags;

    // getting run parameters
    flags = getFlags(argc, argv, flags);

    // no params - print HELP text
    if (argc == 1) {
        cerr << HELP << endl;
        return EXIT_SUCCESS;
        // missing interface name
    } else if ((!flags.count(ASN_FILE)) || (flags[ASN_FILE].empty())) {
        cerr << MSG_ERR_NO_ASN_FILE << endl;
        return ERR_ARGUMENTS;
    }

    AddrTrie<IPv4AddrFamily> trieIpv4;
    AddrTrie<IPv6AddrFamily> trieIpv6;

    /* Load AS numbers from the file. */
    if (!loadTrieFromFile(flags[ASN_FILE], trieIpv4, trieIpv6)) {
        cerr << MSG_ERR_FILE_OPEN << endl;
        return ERR_FILE;
    }

    /* Searching the IP addresses which are put on the stdin. */
    if (!performSearching(trieIpv4, trieIpv6)) {
        cerr << MSG_ERR_STDOUT_IO << endl;
        return ERR_FILE;
    }

    return EXIT_SUCCESS;
}

