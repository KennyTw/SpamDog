// AcMatch.cpp: implementation of the CAcMatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcMatch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcMatch::CAcMatch()
{

	RootNode = new Node;
	memset(RootNode,0,sizeof(Node));

	BFSList = NULL;
	BFSListCount = 0;
}

CAcMatch::~CAcMatch()
{
	FreeTravelTree(RootNode);

	if (BFSList != NULL)
		HeapFree(GetProcessHeap(),NULL,BFSList);
}


Node* CAcMatch::InsertNode(Node* ParentNode,Node* thisNode)
{

	Node* ResNode = NULL;

	ResNode = FindNextDupNode(ParentNode,thisNode);

	if (ResNode == NULL)
	{
		//�S���
		//���J�̫e��
		thisNode->RightNode = ParentNode->ChildNode;
		ParentNode->ChildNode = thisNode;
		//ResNode = thisNode;
	}
 

	return ResNode;

}
Node* CAcMatch::FindNextDupNode(Node* ParentNode,Node* thisNode)
{

	Node* ResNode = NULL;	 
	Node* RightNode = ParentNode->ChildNode;


	while (RightNode != NULL)
	{
		if (RightNode->CharData == thisNode->CharData)
		{		
			ResNode = RightNode;
			break;
		}
		
		RightNode = RightNode->RightNode;
	}

	return ResNode;

}


void CAcMatch::InsertPattern(wchar_t* AStr)
{
	
	int strlen = wcslen(AStr);
	Node* TempRoot = RootNode;
	Node* NewNode = NULL;

	for (int i=0 ; i < strlen ; i++)
	{
		if (AStr[i] != 0)
		{
		    NewNode	 = new Node;
			memset(NewNode,0,sizeof(Node));

			NewNode->CharData = AStr[i];
			

			Node* FindNode = InsertNode(TempRoot,NewNode);

			if (FindNode != NULL)
			{			
				//�I����
				delete NewNode;
				NewNode = FindNode;

				TempRoot = FindNode;
			}
			else
			{
				NewNode->ParentNode = TempRoot;
				TempRoot = NewNode;			
			}
		}
	}

	if (NewNode != NULL)
	{
	
		NewNode->EndNode = true;
		NewNode->StrData = AStr;
		NewNode->EndLength = strlen; 
	}

}

Node* CAcMatch::TravelTree(Node* thisNode)
{

	//MessageBoxW(NULL,&thisNode->CharData,NULL,MB_OK);

	Node* tmpNode = thisNode->RightNode;
	while (tmpNode != NULL)
	{

		tmpNode = TravelTree(tmpNode);
	}

	//���k�����U
	if (tmpNode == NULL)
	{
		
		tmpNode = thisNode->ChildNode;
		while (tmpNode != NULL)
		{
			tmpNode = TravelTree(tmpNode);
		}
	}

	return tmpNode;
	

}

Node* CAcMatch::FreeTravelTree(Node* thisNode)
{

	//MessageBoxW(NULL,&thisNode->CharData,NULL,MB_OK);

	Node* tmpNode = thisNode->ChildNode;
    Node* RightNode = thisNode->RightNode;

	delete thisNode;	
	
	while (tmpNode != NULL)
	{		 
		//Node SaveNode = *tmpNode;
		//delete tmpNode;
		tmpNode = FreeTravelTree(tmpNode);
	}

	if (tmpNode == NULL)
	{
		
		tmpNode = RightNode;
		while (tmpNode != NULL)
		{			
			//Node SaveNode = *tmpNode;
			//delete tmpNode;
			tmpNode = FreeTravelTree(tmpNode);
		}
	}

	return tmpNode;
	

}

void CAcMatch::Travel()
{

	//TravelTree(RootNode);
	//SetFailTree(RootNode);
	int i = 0;
	SetFailTree(&i);
	//ShowTravelTree(RootNode);
	StrMatch(L"�W����H���a��");
}

void CAcMatch::SetFailTree(int *BFSindex)
{
	// traversal node

	if (BFSList == NULL)
	{		
		BFSListCount ++;
		BFSList = (Node **) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(Node **) * BFSListCount);	
	 
		*(BFSList + (BFSListCount - 1)) = RootNode->ChildNode;
		*BFSindex = 0;
 
	}
	else
	{

		Node* tmpNode = *(BFSList + (*BFSindex));
		//MessageBoxW(NULL,&tmpNode->CharData,NULL,MB_OK);

		if (tmpNode->CharData != 0)
		{
	
			if (tmpNode->ParentNode->CharData  == 0)
			{		
				//�Ĥ@�h���V root
				tmpNode->FailNode = tmpNode->ParentNode;		
			}
			else
			{
		

				Node* SearchNote = tmpNode->ParentNode->FailNode;
				Node* ResNode = NULL;
			 
				//�p�G�S���n�b�V�W
				while (ResNode == NULL )
				{ 
					if (SearchNote != NULL)
					{
						ResNode = FindNextDupNode(SearchNote,tmpNode);				

						if (ResNode != NULL)
						{
							tmpNode->FailNode = ResNode;
							break;
						}
					}
					else
					{
						//�쳻�ݤF,���J root
						tmpNode->FailNode = RootNode;
						break;
					
					}
					SearchNote = SearchNote->FailNode;
				} 
			}
		}

		//���V�k��
		if (tmpNode->RightNode != NULL)
		{
			BFSListCount ++;
			BFSList = (Node **) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,BFSList,sizeof(Node **) * BFSListCount);						
			*(BFSList + (BFSListCount - 1)) = tmpNode->RightNode;
		
		}
		//�V�U��
		if (tmpNode->ChildNode != NULL)
		{

			BFSListCount ++;			
			BFSList = (Node **) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,BFSList,sizeof(Node **) * BFSListCount);			
			*(BFSList + (BFSListCount - 1)) = tmpNode->ChildNode;		

		}

		(*BFSindex)++;


	}

	if (*BFSindex < (int) BFSListCount)
	{
		SetFailTree(BFSindex);
	}	
	
/*
	//�B�z
	if (thisNode->CharData != 0)
	{
	
		if (thisNode->ParentNode->CharData  == 0)
		{		
			//�Ĥ@�h���V root
			thisNode->FailNode = thisNode->ParentNode;		
		}
		else
		{
		

			Node* SearchNote = thisNode->ParentNode->FailNode;
			Node* ResNode = NULL;

			//��� fail note
			//if (ResNode != NULL)
			//{			
				//thisNode->FailNode = ResNode;
			//}
			//else
			//{
				//�p�G�S���n�b�V�W
			while (ResNode == NULL )
			{
					
					if (SearchNote != NULL)
					{
						ResNode = FindNextRepNode(SearchNote,thisNode);				

						if (ResNode != NULL)
						{
							thisNode->FailNode = ResNode;
							break;
						}
					}
					else
					{
						//�쳻�ݤF,���J root
						thisNode->FailNode = RootNode;
						break;
					
					}
					SearchNote = SearchNote->FailNode;
			}
			
			//}		
		}
	}
	
	Node* tmpNode = thisNode->RightNode ;    
	while (tmpNode != NULL)
	{
		//MessageBoxW(NULL,&tmpNode->CharData,NULL,MB_OK);
		//Node* ParentNode = tmpNode
		tmpNode = SetFailTree(tmpNode);
	}

	//���k�����U
	if (tmpNode == NULL)
	{
		tmpNode = thisNode->ChildNode;
		while (tmpNode != NULL)
		{
			//MessageBoxW(NULL,&tmpNode->CharData,NULL,MB_OK);
			tmpNode = SetFailTree(tmpNode);
		}
	}

	return tmpNode;
	*/



}

void CAcMatch::BFSTravel(int *BFSindex)
{

	// traversal node

	if (BFSList == NULL)
	{		
		BFSListCount ++;
		BFSList = (Node **) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(Node **) * BFSListCount);	
	 
		*(BFSList + (BFSListCount - 1)) = RootNode->ChildNode;
		*BFSindex = 0;
 
	}
	else
	{

		Node* tmpNode = *(BFSList + (*BFSindex));
		//MessageBoxW(NULL,&tmpNode->CharData,NULL,MB_OK);

		//���V�k��
		if (tmpNode->RightNode != NULL)
		{
			BFSListCount ++;
			BFSList = (Node **) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,BFSList,sizeof(Node **) * BFSListCount);						
			*(BFSList + (BFSListCount - 1)) = tmpNode->RightNode;
		
		}
		//�V�U��
		if (tmpNode->ChildNode != NULL)
		{

			BFSListCount ++;			
			BFSList = (Node **) HeapReAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,BFSList,sizeof(Node **) * BFSListCount);			
			*(BFSList + (BFSListCount - 1)) = tmpNode->ChildNode;		

		}

		(*BFSindex)++;


	}

	if (*BFSindex < (int) BFSListCount)
	{
		BFSTravel(BFSindex);
	}



}

Node* CAcMatch::ShowTravelTree(Node* thisNode)
{
	
	if (thisNode->CharData != 0)
	{
		//����I��T
		//CString tmpstr;
		//tmpstr.Format("%S -> %S",&thisNode->CharData , &thisNode->FailNode->CharData);		
		//AfxMessageBox(tmpstr);
		
	}

	Node* tmpNode = thisNode->ChildNode;
	while (tmpNode != NULL)
	{

		tmpNode = ShowTravelTree(tmpNode);
	}

	//���k�����U
	if (tmpNode == NULL)
	{
		
		tmpNode = thisNode->RightNode;
		while (tmpNode != NULL)
		{
			tmpNode = ShowTravelTree(tmpNode);
		}
	}

	return tmpNode;

}

void CAcMatch::StrMatch(wchar_t* AStr)
{
	int strlen = wcslen(AStr);
	Node* TempRoot = RootNode;
	Node DataNote;
	

	for (int i=0 ; i < strlen ; i++)
	{
		DataNote.CharData = AStr[i];
		Node *SearchNode = NULL;	
		 
		while (SearchNode == NULL)
		{
			
			SearchNode = FindNextDupNode(TempRoot,&DataNote);
			if (SearchNode == NULL)
			{		
				//�S����`�I , �� fail note ��
				if (TempRoot->FailNode != NULL)
				{
					//SearchNode = TempRoot->FailNode;
					TempRoot = TempRoot->FailNode;
				}
				else
				{
				   break;
				}
			}
			else
			{			
				if (SearchNode->FailNode->EndNode == true)
				{
					//CString tmpstr;
					//tmpstr.Format("Match pos : %d , %C", i - SearchNode->FailNode->EndLength +1 , &SearchNode->FailNode->CharData);
					//AfxMessageBox(tmpstr);					

					//MessageBoxW(NULL,(wchar_t *) tmpstr.GetBuffer(tmpstr.GetLength()*2),NULL,MB_OK);

					char tmpstr[255];
					memset(tmpstr,0,sizeof(tmpstr));

					WideCharToMultiByte(CP_ACP,NULL,SearchNode->FailNode->StrData,wcslen(SearchNode->FailNode->StrData),tmpstr,sizeof(tmpstr),NULL,NULL);

					//CString tmpctr;
					//tmpctr.Format("Match pos : %d , %s",i - SearchNode->FailNode->EndLength + 1,tmpstr);
					//AfxMessageBox(tmpctr);
					//��ܲŦX�I

				}

				if (SearchNode->EndNode == true)
				{
					//���F ..
					//CString tmpstr;
					//tmpstr.Format("Match pos : %d , %C", i - SearchNode->EndLength + 1, &SearchNode->CharData);
					//AfxMessageBox(tmpstr);
					char tmpstr[255];
					memset(tmpstr,0,sizeof(tmpstr));

					WideCharToMultiByte(CP_ACP,NULL,SearchNode->StrData,wcslen(SearchNode->StrData),tmpstr,sizeof(tmpstr),NULL,NULL);

					//CString tmpctr;
					//tmpctr.Format("Match pos : %d , %s",i - SearchNode->EndLength + 1,tmpstr);
					//AfxMessageBox(tmpctr);

					//MessageBoxW(NULL,&SearchNode->CharData,NULL,MB_OK);

					//WideCharToMultiByte(CP_ACP,NULL,&SearchNode->CharData
				}
				else
				{
				
					TempRoot = SearchNode;			
				}
			}

			
		}
		


	
	}


}