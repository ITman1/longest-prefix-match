///////////////////////////////////////////////////////////////////////////////
// Project:    Longest-Prefix Match
// Course:     PDS (Data Communications, Computer Networks and Protocols)
// File:       TrieNode.h
// Date:       May 2013
// Author:     Radim Loskot
// E-mail:     xlosko01(at)stud.fit.vutbr.cz
//
// Brief:      Header file which defines trie node.
///////////////////////////////////////////////////////////////////////////////

/**
 * @file TrieNode.h
 *
 * @brief Defines trie node.
 * @author Radim Loskot xlosko01(at)stud.fit.vutbr.cz
 */


#ifndef TRIENODE_H
#define TRIENODE_H

class TrieNode
{
public:
	/**
	 * Constructs an empty trie node.
	 */
	TrieNode() : _leftChild(0), _rightChild(0), _value(0) {}

	/**
	 * Constructs trie node with specified children and value.¨
	 * @param value Value which will have this node.
	 * @param leftChild Pointer to left child of this node.
	 * @param rightChild Pointer to right child of this node.
	 */
	TrieNode(void *value, TrieNode *leftChild, TrieNode *rightChild)
	: _leftChild(leftChild), _rightChild(rightChild), _value(value) {}

	/**
	 * Sets left child of this node.
	 * @param child New child will be set as a left child in this node.
	 */
    inline void setLeftChild(TrieNode *child) { _leftChild = child; }

	/**
	 * Gets left child of this node.
	 * @param Light child of this node.
	 */
    inline TrieNode *getLeftChild() { return _leftChild; }

	/**
	 * Sets right child of this node.
	 * @param child New child will be set as a right child in this node.
	 */
    inline void setRightChild(TrieNode *child) { _rightChild = child; }

	/**
	 * Gets right child of this node.
	 * @param Right child of this node.
	 */
    inline TrieNode *getRightChild() { return _rightChild; }

	/**
	 * Sets new value to this node.
	 * @param value Value to be set.
	 */
    inline void setValue(void *value) { _value = value; }

    /**
     * Returns value of this node.
     * @return Value of this node.
     */
    inline void *getValue() { return _value; }
private:
    TrieNode *_leftChild;       /**< Left child node */
    TrieNode *_rightChild;      /**< Right child node */
    void *_value;               /**< Value of this node */
};

#endif // TRIENODE_H
