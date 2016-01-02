// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#if 0
#define TreeDebugNum 128
#define TreeNumTests 32
#define TreeTestNum 64*1024
//#define TreeTestNum 256
//#define TreeMixedList

/*
 *  avltree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H

#define DMibAllowCodeStandardViolations 1

#ifndef AVLDATUM
#define AVLDATUM class CTestTreeClass *
#endif

#ifndef AVLKEY
#define AVLKEY(x) (x->m_Data)
#endif

/*
 *  Which of a given node's subtrees is higher?
 */
enum AVLSKEW 
{
	NONE,	
	LEFT,
	RIGHT
};

/*
 *  Did a given insertion/deletion succeed, and what do we do next?
 */
enum AVLRES
{
	ERROR = 0,
	OK,
	BALANCE,
};

/*
 *  AVL tree node structure
 */
struct avlnode
{
	struct avlnode *left, *right;
	AVLDATUM d;
	enum AVLSKEW skew;
};

/*
 *  avlinsert: insert a node into the AVL tree.
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node.
 *
 *    d           Item to be inserted.
 *
 *  Return values:
 *
 *    nonzero     The item has been inserted. The excact value of 
 *                nonzero yields is of no concern to user code; when
 *                avlinsert recursively calls itself, the number 
 *                returned tells the parent activation if the AVL tree 
 *                may have become unbalanced; specifically:
 *
 *      OK        None of the subtrees of the node that n points to 
 *                has grown, the AVL tree is valid.
 *
 *      BALANCE   One of the subtrees of the node that n points to 
 *                has grown, the node's "skew" flag needs adjustment,
 *                and the AVL tree may have become unbalanced.
 *
 *    zero        The datum provided could not be inserted, either due 
 *                to AVLKEY collision (the tree already contains another
 *                item with which the same AVLKEY is associated), or
 *                due to insufficient memory.
 */   
enum AVLRES
avlinsert(struct avlnode **n, AVLDATUM d);

/*
 *  avlremove: remove an item from the tree.
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node.
 *
 *    key         AVLKEY of item to be removed.
 *
 *  Return values:
 *
 *    nonzero     The item has been removed. The exact value of 
 *                nonzero yields if of no concern to user code; when
 *                avlremove recursively calls itself, the number
 *                returned tells the parent activation if the AVL tree
 *                may have become unbalanced; specifically:
 *
 *      OK        None of the subtrees of the node that n points to
 *                has shrunk, the AVL tree is valid.
 *
 *      BALANCE   One of the subtrees of the node that n points to
 *                has shrunk, the node's "skew" flag needs adjustment,
 *                and the AVL tree may have become unbalanced.
 *
 *   zero         The tree does not contain an item yielding the
 *                AVLKEY value provided by the caller.
 */
enum AVLRES
avlremove(struct avlnode **n, aint key);

/*
 *  avlaccess: retrieve the datum corresponding to a given AVLKEY.
 *
 *  Parameters:
 *
 *    n           Pointer to the root node.
 *
 *    key         TKEY of item to be accessed.
 *
 *  Return values:
 *
 *    non-nullptr    An item yielding the AVLKEY provided has been found,
 *                the return value points to the AVLKEY attached to it.
 *
 *    nullptr        The item could not be found.
 */
AVLDATUM *
avlaccess(struct avlnode *n, aint key);


/*
 *  Function to be called by the tree traversal functions.
 *
 *  Parameters:
 *
 *    n           Pointer to a node.
 *
 *    param       Value provided by the traversal function's caller.
 *
 *    depth       Recursion depth indicator. Allows the function to
 *                determine how many levels the node bein processed is
 *                below the root node. Can be used, for example,
 *                for selecting the proper indentation width when
 *                avldepthfirst is used to print a tree dump to 
 *                the screen.
 */
typedef void AVLWORKER(struct avlnode *n, aint param, aint depth);

/*
 *  avldepthfirst: depth-first tree traversal.
 *
 *  Parameters:
 *
 *    n          Pointer to the root node.
 *
 *    f          Worker function to be called for every node.
 *
 *    param      Additional parameter to be passed to the
 *               worker function
 *
 *    depth      Recursion depth indicator. Allows the worker function
 *               to determine how many levels the node being processed
 *               is below the root node. Can be used, for example,
 *               for selecting the proper indentation width when
 *               avldepthfirst ist used to print a tree dump to
 *               the screen.
 *
 *               Most of the time, you will want to call avldepthfirst
 *               with a "depth" value of zero.
 */
void
avldepthfirst(struct avlnode *n, AVLWORKER *f, aint param, aint depth);

/*
 *  avlbreadthfirst: breadth-first tree traversal.
 * 
 *  See avldepthfirst for details.
 */
void
avlbreadthfirst(struct avlnode *n, AVLWORKER *f, aint param);

#endif


class CTestTreeClass
{
public:

	CTestTreeClass()
	{
	}
	~CTestTreeClass()
	{
	}
	aint m_Data;
};
class CTestTreeRef : public CMalterlibTest
{
public:

	bool f_AutomaticTest() 
	{
		return true;
	}

	aint Rand (void)
	{
		static aint holdrand = 548;
		return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	static const aint mc_TestSorted = TreeTestNum;

	CTestTreeClass m_ClassList[mc_TestSorted];

	void RandomizeList()
	{
		for (aint i = 0; i < mc_TestSorted; ++i)
		{
			m_ClassList[i].m_Data = i * 2;
		}
	}
	avlnode *m_TreeRoot;

	void TreeGetMaxDepth(avlnode *_pObj, aint &_MaxDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;

		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;

		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth > _MaxDepth)
				_MaxDepth = _CurrentDepth;
		}

		if (pLeft)
			TreeGetMaxDepth(pLeft, _MaxDepth, _CurrentDepth);
		if (pRight)
			TreeGetMaxDepth(pRight, _MaxDepth, _CurrentDepth);


		--_CurrentDepth;
	}

	void TreeGetMinDepth(avlnode *_pObj, aint &_MinDepth, aint &_CurrentDepth)
	{
		++_CurrentDepth;

		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;
		
		if (!pRight &&!pLeft)
		{
			if (_CurrentDepth < _MinDepth)
				_MinDepth = _CurrentDepth;

		}

		if (pLeft)
			TreeGetMinDepth(pLeft, _MinDepth, _CurrentDepth);
		if (pRight)
			TreeGetMinDepth(pRight, _MinDepth, _CurrentDepth);

		--_CurrentDepth;
	}


	aint GetTreeLargest(avlnode *_pObj)
	{
		aint CurrentLargest = _pObj->d->m_Data;

		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;

		if (pLeft)
		{
			aint Largest;
			if ((Largest = GetTreeLargest(pLeft)) > CurrentLargest)
				CurrentLargest = Largest;
		}

		if (pRight)
		{
			aint Largest;
			if ((Largest = GetTreeLargest(pRight)) > CurrentLargest)
				CurrentLargest = Largest;
		}

		return CurrentLargest;
	}

	aint GetTreeSmallest(avlnode *_pObj)
	{
		aint CurrentSmallest = _pObj->d->m_Data;

		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;


		if (pLeft)
		{
			aint Smallest;
			if ((Smallest = GetTreeLargest(pLeft)) < CurrentSmallest)
				CurrentSmallest = Smallest;
		}

		if (pRight)
		{
			aint Smallest;
			if ((Smallest = GetTreeLargest(pRight)) < CurrentSmallest)
				CurrentSmallest = Smallest;
		}

		return CurrentSmallest;
	}

	void f_CheckTree(avlnode *_pObj)
	{
		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;

		aint LeftMaxDepth = 0;
		aint RightMaxDepth = 0;

		if (pLeft)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(pRight, LeftMaxDepth, CurrentDepth);
			DMibSafeCheck(_pObj->d->m_Data > GetTreeLargest(pLeft),"Tree is not binary");
			f_CheckTree(pLeft);
		}
		if (pRight)
		{
			aint CurrentDepth = 0;
			TreeGetMaxDepth(pRight, RightMaxDepth, CurrentDepth);
			DMibSafeCheck(_pObj->d->m_Data < GetTreeSmallest(pRight),"Tree is not binary");
			f_CheckTree(pRight);
		}
		if (LeftMaxDepth > RightMaxDepth)
		{
			DMibSafeCheck((LeftMaxDepth - RightMaxDepth) <= 1, "Tree has lost its AVL Property");
		}
		else
		{
			DMibSafeCheck((RightMaxDepth - LeftMaxDepth) <= 1, "Tree has lost its AVL Property");
		}

	}

	void f_CheckTree()
	{
		if (m_TreeRoot)
		{
			f_CheckTree(m_TreeRoot);
		}
	}

	void TraceTree(avlnode *_pObj, aint _Depth, aint &_CurrentDepth, bint &_bTraced, bint _bLeft, aint _MaxDepth)
	{
		if (_Depth == _CurrentDepth)
		{
			_bTraced = true;

			if (_bLeft)
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("L{}") << (_pObj->d->m_Data)).f_GetStr() << 4);
			else
				DMibTrace("{L*1}", (NMib::NStr::CStr::CFormat("R{}") << (_pObj->d->m_Data)).f_GetStr() << 4);
		}
		++_CurrentDepth;

		avlnode *pRight = _pObj->right;
		avlnode *pLeft = _pObj->left;

		if (pLeft)
		{
			TraceTree(pLeft, _Depth, _CurrentDepth, _bTraced, true, _MaxDepth);
		}

		if (pRight)
		{
			TraceTree(pRight, _Depth, _CurrentDepth, _bTraced, false, _MaxDepth);
		}
		--_CurrentDepth;
	}

	void TraceTree()
	{
		if (!m_TreeRoot)
			return;
		aint MaxDepth = 0;
		aint CurrentDepth = 0;
		TreeGetMaxDepth(m_TreeRoot, MaxDepth, CurrentDepth);
		DMibTrace("Tree Trace Depth({}):\n", (MaxDepth));
		for (aint i = 0; i < MaxDepth; ++i)
		{
			bint Traced = false;
			aint CurrentDepth = 0;
			TraceTree(m_TreeRoot, i, CurrentDepth, Traced, false, MaxDepth);
			if (Traced)
			{
				DMibTrace("\n", 0);
			}
		}
	}

	NMib::NStr::CStr Certify(CTestInterface &_Interface)
	{
		m_TreeRoot = nullptr;
	//	DMibTrace("AVLTree Reference\n", 0);
		RandomizeList();

		aint DebugTest = TreeDebugNum;

		// Ceck validity of tree code
		for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
		{
			avlinsert(&m_TreeRoot, &m_ClassList[i]);
			f_CheckTree();
		}

		f_CheckTree();

		
		for (aint i = 0; i < (DebugTest < mc_TestSorted ? DebugTest : mc_TestSorted); ++i)
		{
			avlremove(&m_TreeRoot, m_ClassList[i].m_Data);
		//	TraceTree();
			f_CheckTree();
		}
		f_CheckTree();
		DMibSafeCheck(!m_TreeRoot, "Must be empty");

		NMib::NTime::CTimer Timer;
		{
			DMibScopeTimer(Timer);			
			for (aint i = 0; i < mc_TestSorted; ++i)
			{
				avlinsert(&m_TreeRoot, &m_ClassList[i]);
			}
		}
		DMibTrace("Performance for AVLTree insert = {0} inserts per second\n", fp64(mc_TestSorted) / Timer.f_GetTime());


		{
			DMibScopeTimer(Timer);			
			for (aint i = 0; i < mc_TestSorted; ++i)
			{
				avlremove(&m_TreeRoot, m_ClassList[i].m_Data);
			}
		}
		DMibTrace("Performance for AVLTree remove = {0} removes per second\n", fp64(mc_TestSorted) / Timer.f_GetTime());


//		for (aint i = 1; i < mc_TestSorted; i+=2)
//		{
//			m_TestTree.f_Remove(m_ClassList[i]);
//			f_CheckTree();
//		}

		return "";

	}

};

DMibRuntimeClass(CMalterlibTest, CTestTreeRef);

/*
 *  avltree.c
 */

/*
 *  avlrotleft: perform counterclockwise rotation
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node
 */
void
avlrotleft(struct avlnode **n)
{
	struct avlnode *tmp = *n;

	*n = (*n)->right;
	tmp->right = (*n)->left;
	(*n)->left = tmp;
}

/*
 *  avlrotright: perform clockwise rotation
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node
 */
void
avlrotright(struct avlnode **n)
{
	struct avlnode *tmp = *n;

	*n = (*n)->left;
	tmp->left = (*n)->right;
	(*n)->right = tmp;
}

/*
 *  avlleftgrown: helper function for avlinsert
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node. This node's left 
 *                subtree has just grown due to item insertion; its 
 *                "skew" flag needs adjustment, and the local tree 
 *                (the subtree of which this node is the root node) may 
 *                have become unbalanced.
 *
 *  Return values:
 *
 *    OK          The local tree could be rebalanced or was balanced 
 *                from the start. The parent activations of the avlinsert 
 *                activation that called this function may assume the 
 *                entire tree is valid.
 *
 *    BALANCE     The local tree was balanced, but has grown in height.
 *                Do not assume the entire tree is valid.
 */
enum AVLRES
avlleftgrown(struct avlnode **n)
{
	switch ((*n)->skew) {
	case LEFT:
		if ((*n)->left->skew == LEFT) {
			(*n)->skew = (*n)->left->skew = NONE;
			avlrotright(n);
		}	
		else {
			switch ((*n)->left->right->skew) {
			case LEFT:
				(*n)->skew = RIGHT;
				(*n)->left->skew = NONE;
				break;

			case RIGHT:
				(*n)->skew = NONE;
				(*n)->left->skew = LEFT;
				break;

			default:
				(*n)->skew = NONE;
				(*n)->left->skew = NONE;
			}
			(*n)->left->right->skew = NONE;
			avlrotleft(& (*n)->left);
			avlrotright(n);
		}
		return OK;

	case RIGHT:
		(*n)->skew = NONE;
		return OK;
	
	default:
		(*n)->skew = LEFT;
		return BALANCE;
	}
}

/*
 *  avlrightgrown: helper function for avlinsert
 *
 *  See avlleftgrown for details.
 */
enum AVLRES
avlrightgrown(struct avlnode **n)
{
	switch ((*n)->skew) {
	case LEFT:					
		(*n)->skew = NONE;
		return OK;

	case RIGHT:
		if ((*n)->right->skew == RIGHT) {	
			(*n)->skew = (*n)->right->skew = NONE;
			avlrotleft(n);
		}
		else {
			switch ((*n)->right->left->skew) {
			case RIGHT:
				(*n)->skew = LEFT;
				(*n)->right->skew = NONE;
				break;

			case LEFT:
				(*n)->skew = NONE;
				(*n)->right->skew = RIGHT;
				break;

			default:
				(*n)->skew = NONE;
				(*n)->right->skew = NONE;
			}
			(*n)->right->left->skew = NONE;
			avlrotright(& (*n)->right);
			avlrotleft(n);
		}
		return OK;

	default:
		(*n)->skew = RIGHT;
		return BALANCE;
	}
}

/*
 *  avlinsert: insert a node into the AVL tree.
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node.
 *
 *    d           Item to be inserted.
 *
 *  Return values:
 *
 *    nonzero     The item has been inserted. The excact value of 
 *                nonzero yields is of no concern to user code; when
 *                avlinsert recursively calls itself, the number 
 *                returned tells the parent activation if the AVL tree 
 *                may have become unbalanced; specifically:
 *
 *      OK        None of the subtrees of the node that n points to 
 *                has grown, the AVL tree is valid.
 *
 *      BALANCE   One of the subtrees of the node that n points to 
 *                has grown, the node's "skew" flag needs adjustment,
 *                and the AVL tree may have become unbalanced.
 *
 *    zero        The datum provided could not be inserted, either due 
 *                to AVLKEY collision (the tree already contains another
 *                item with which the same AVLKEY is associated), or
 *                due to insufficient memory.
 */   
enum AVLRES
avlinsert(struct avlnode **n, AVLDATUM d)
{
	enum AVLRES tmp;

	if (!(*n)) 
	{
		mint Size = sizeof(struct avlnode);
		((*n) = (struct avlnode *)NMib::NMem::fg_Alloc(Size));
		if (!(*n)) {
			return ERROR;
		}
		(*n)->left = (*n)->right = nullptr;
		(*n)->d = d;
		(*n)->skew = NONE;
		return BALANCE;
	}
	
	if (AVLKEY(d) < AVLKEY((*n)->d)) {
		if ((tmp = avlinsert(& (*n)->left, d)) == BALANCE) {
			return avlleftgrown(n);
		}
		return tmp;
	}
	if (AVLKEY(d) > AVLKEY((*n)->d)) {
		if ((tmp = avlinsert(& (*n)->right, d)) == BALANCE) {
			return avlrightgrown(n);
		}
		return tmp;
	}
	return ERROR;
}

/*
 *  avlleftshrunk: helper function for avlremove and avlfindlowest
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node. The node's left
 *                subtree has just shrunk due to item removal; its
 *                "skew" flag needs adjustment, and the local tree
 *                (the subtree of which this node is the root node) may
 *                have become unbalanced.
 *

 *   Return values:
 *
 *    OK          The parent activation of the avlremove activation
 *                that called this function may assume the entire
 *                tree is valid.
 *
 *    BALANCE     Do not assume the entire tree is valid.
 */                
enum AVLRES
avlleftshrunk(struct avlnode **n)
{
	switch ((*n)->skew) {
	case LEFT:
		(*n)->skew = NONE;
		return BALANCE;

	case RIGHT:
		if ((*n)->right->skew == RIGHT) {
			(*n)->skew = (*n)->right->skew = NONE;
			avlrotleft(n);
			return BALANCE;
		}
		else if ((*n)->right->skew == NONE) {
			(*n)->skew = RIGHT;
			(*n)->right->skew = LEFT;
			avlrotleft(n);
			return OK;
		}
		else {
			switch ((*n)->right->left->skew) {
			case LEFT:
				(*n)->skew = NONE;
				(*n)->right->skew = RIGHT;
				break;

			case RIGHT:
				(*n)->skew = LEFT;
				(*n)->right->skew = NONE;
				break;

			default:
				(*n)->skew = NONE;
				(*n)->right->skew = NONE;
			}
			(*n)->right->left->skew = NONE;
			avlrotright(& (*n)->right);
			avlrotleft(n);
			return BALANCE;
		}

	default:
		(*n)->skew = RIGHT;
		return OK;
	}
}

/*
 *  avlrightshrunk: helper function for avlremove and avlfindhighest
 *
 *  See avlleftshrunk for details.
 */
enum AVLRES
avlrightshrunk(struct avlnode **n)
{
	switch ((*n)->skew) {
	case RIGHT:
		(*n)->skew = NONE;
		return BALANCE;

	case LEFT:
		if ((*n)->left->skew == LEFT) {
			(*n)->skew = (*n)->left->skew = NONE;
			avlrotright(n);
			return BALANCE;
		}
		else if ((*n)->left->skew == NONE) {
			(*n)->skew = LEFT;
			(*n)->left->skew = RIGHT;
			avlrotright(n);
			return OK;
		}
		else {
			switch ((*n)->left->right->skew) {
			case LEFT:
				(*n)->skew = RIGHT;
				(*n)->left->skew = NONE;
				break;

			case RIGHT:
				(*n)->skew = NONE;
				(*n)->left->skew = LEFT;	
				break;
			
			default:
				(*n)->skew = NONE;
				(*n)->left->skew = NONE;
			}
			(*n)->left->right->skew = NONE;
			avlrotleft(& (*n)->left);
			avlrotright(n);
			return BALANCE;
		}

	default:
		(*n)->skew = LEFT;
		return OK;
	}
}

/*
 *  avlfindhighest: replace a node with a subtree's highest-ranking item.
 *
 *  Parameters:
 *
 *    target      Pointer to node to be replaced.
 *
 *    n           Address of pointer to subtree.
 *
 *    res         Pointer to variable used to tell the caller whether
 *                further checks are necessary; analog to the return
 *                values of avlleftgrown and avlleftshrunk (see there). 
 *
 *  Return values:
 *
 *    1           A node was found; the target node has been replaced.
 *
 *    0           The target node could not be replaced because
 *                the subtree provided was empty.
 *
 */
aint
avlfindhighest(struct avlnode *target, struct avlnode **n, enum AVLRES *res)
{
	struct avlnode *tmp;


	*res = BALANCE;
	if (!(*n)) {
		return 0;
	}
	if ((*n)->right) {
		if (!avlfindhighest(target, &(*n)->right, res)) {
			return 0;
		}
		if (*res == BALANCE) {
			*res = avlrightshrunk(n);
		}
		return 1;
	}
	target->d  = (*n)->d;
	tmp = *n;
	*n = (*n)->left;
	NMib::NMem::fg_Free(tmp);
	return 1;
}

/*
 *  avlfindlowest: replace node with a subtree's lowest-ranking item.
 *
 *  See avlfindhighest for the details.
 */
aint
avlfindlowest(struct avlnode *target, struct avlnode **n, enum AVLRES *res)
{
	struct avlnode *tmp;

	*res = BALANCE;
	if (!(*n)) {
		return 0;
	}
	if ((*n)->left) {
		if (!avlfindlowest(target, &(*n)->left, res)) {
			return 0;
		}
		if (*res == BALANCE) {
			*res =  avlleftshrunk(n);
		}
		return 1;
	}
	target->d = (*n)->d;
	tmp = *n;
	*n = (*n)->right;
	NMib::NMem::fg_Free(tmp);
	return 1;
}

/*
 *  avlremove: remove an item from the tree.
 *
 *  Parameters:
 *
 *    n           Address of a pointer to a node.
 *
 *    key         AVLKEY of item to be removed.
 *
 *  Return values:
 *
 *    nonzero     The item has been removed. The exact value of 
 *                nonzero yields if of no concern to user code; when
 *                avlremove recursively calls itself, the number
 *                returned tells the parent activation if the AVL tree
 *                may have become unbalanced; specifically:
 *
 *      OK        None of the subtrees of the node that n points to
 *                has shrunk, the AVL tree is valid.
 *
 *      BALANCE   One of the subtrees of the node that n points to
 *                has shrunk, the node's "skew" flag needs adjustment,
 *                and the AVL tree may have become unbalanced.
 *
 *   zero         The tree does not contain an item yielding the
 *                AVLKEY value provided by the caller.
 */
enum AVLRES
avlremove(struct avlnode **n, aint key)
{
	enum AVLRES tmp = BALANCE;

	if (!(*n)) {
		return ERROR;
	}
	if (key < AVLKEY((*n)->d)) {
		if ((tmp = avlremove(& (*n)->left, key)) == BALANCE) {
			return avlleftshrunk(n);
		}
		return tmp;
	}
	if (key > AVLKEY((*n)->d)) {
		if ((tmp = avlremove(& (*n)->right, key)) == BALANCE) {
			return avlrightshrunk(n);
		}
		return tmp;
	}
	if ((*n)->left) {
		if (avlfindhighest(*n, &((*n)->left), &tmp)) {
			if (tmp == BALANCE) {
				tmp = avlleftshrunk(n);
			}
			return tmp;
		}
	}
	if ((*n)->right) {
		if (avlfindlowest(*n, &((*n)->right), &tmp)) {
			if (tmp == BALANCE) {
				tmp = avlrightshrunk(n);
			}
			return tmp;
		}
	}
	NMib::NMem::fg_Free(*n);
 	*n = nullptr;
	return BALANCE;
}

/*
 *  avlaccess: retrieve the datum corresponding to a given AVLKEY.
 *
 *  Parameters:
 *
 *    n           Pointer to the root node.
 *
 *    key         TKEY of item to be accessed.
 *
 *  Return values:
 *
 *    non-nullptr    An item yielding the AVLKEY provided has been found,
 *                the return value points to the AVLKEY attached to it.
 *
 *    nullptr        The item could not be found.
 */
AVLDATUM *
avlaccess(struct avlnode *n, aint key)
{
        if (!n) {
                return nullptr;
        }
        if (key < AVLKEY((n)->d)) {
                return avlaccess(n->left, key);
        }
        if (key > AVLKEY((n)->d)) {
                return avlaccess(n->right, key);
        }
        return &(n->d);
}

/*
 *  avldepthfirst: depth-first tree traversal.
 *
 *  Parameters:
 *
 *    n          Pointer to the root node.
 *
 *    f          Worker function to be called for every node.
 *
 *    param      Additional parameter to be passed to the
 *               worker function
 *
 *    depth      Recursion depth indicator. Allows the worker function
 *               to determine how many levels the node being processed
 *               is below the root node. Can be used, for example,
 *               for selecting the proper indentation width when
 *               avldepthfirst ist used to print a tree dump to
 *               the screen.
 *
 *               Most of the time, you will want to call avldepthfirst
 *               with a "depth" value of zero.
 */
void
avldepthfirst(struct avlnode *n, AVLWORKER *f, aint param, aint depth)
{
	if (!n) return;
	avldepthfirst(n->left, f, param, depth + 1);
	(*f)(n, param, depth);
	avldepthfirst(n->right, f, param, depth +1);
}

#endif
