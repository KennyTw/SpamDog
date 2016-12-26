// AcMatch.h: interface for the CAcMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACMATCH_H__D4785862_9E82_4A1E_B0AF_9BE4AB8F13A2__INCLUDED_)
#define AFX_ACMATCH_H__D4785862_9E82_4A1E_B0AF_9BE4AB8F13A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif

typedef struct _Node
{
	bool EndNode;
	unsigned int EndLength;
	wchar_t CharData;
	wchar_t *StrData;
	
	struct _Node *FailNode; //AC 節點
	struct _Node *ParentNode; //上方節點
	struct _Node *ChildNode; //下方節點
	struct _Node *RightNode;	 //右手節點	
} Node;

class CAcMatch  
{
public:
	CAcMatch();
	virtual ~CAcMatch();

	void InsertPattern(wchar_t* AStr);
	void StrMatch(wchar_t* AStr);
	void Travel();


private:
	Node* RootNode;
	struct _Node **BFSList;	 //BFS list
	//BFSNode* BFSList;
	unsigned int BFSListCount ;

	void BFSTravel(int *BFSindex);
	Node* InsertNode(Node* ParentNode,Node* thisNode);
	Node* FindNextDupNode(Node* ParentNode,Node* thisNode);  
	Node* TravelTree(Node* thisNode);
	Node* FreeTravelTree(Node* thisNode);

	void SetFailTree(int *BFSindex);

	Node* ShowTravelTree(Node* thisNode);

};

#endif // !defined(AFX_ACMATCH_H__D4785862_9E82_4A1E_B0AF_9BE4AB8F13A2__INCLUDED_)
