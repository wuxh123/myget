// ItemHeader.cpp: implementation of the CItemHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemHeader.h"
#include "JCDFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemHeader::CItemHeader(int iItemClass)
{

	switch(iItemClass)
	{
	case ITEM_CLASS_TREEITEM:
		m_iUnknown = 1;
		break;

	case ITEM_CLASS_LISTITEM:
		m_iUnknown = 2;
		m_iParentID = DOWNLOAD_CATEGORY_ID;
	}

	
	//Default setting
	m_iID			= -1;
	m_iParentID		= -1;
	m_iImageIndex	= ITEM_OTHER_IMAGE_INDEX;
	m_iClass		= iItemClass;
	m_iType			= OTHER_ITEMS;
	m_iEndFlag		= ITEM_HEADER_END_FLAG;
}

CItemHeader::~CItemHeader()
{
}

int CItemHeader::GetClass()
{
	return m_iClass;
}

void CItemHeader::SetClass(int iClass)
{
	m_iClass = iClass;
}

int CItemHeader::GetID()
{
	return m_iID;
}

void CItemHeader::SetID(int iID)
{
	m_iID = iID;
}

int CItemHeader::GetImageIndex()
{
	return m_iImageIndex;
}

void CItemHeader::SetImageIndex(int iImageIndex)
{
	m_iImageIndex = iImageIndex;
}

int CItemHeader::GetParentID()
{
	return m_iParentID;
}

void CItemHeader::SetParentID(int iParentID)
{
	m_iParentID = iParentID;
}

int CItemHeader::GetType()
{
	return m_iType;
}

void CItemHeader::SetType(int iType)
{
	m_iType = iType;
}

BOOL CItemHeader::LoadFromFile(CJCDFile *pJCDFile)
{
	JCD_LOAD_INT(m_iClass);
	JCD_LOAD_INT(m_iID);
	JCD_LOAD_INT(m_iImageIndex);
	JCD_LOAD_INT(m_iUnknown);
	JCD_LOAD_INT(m_iParentID);
	JCD_LOAD_INT(m_iType);
	JCD_LOAD_INT(m_iEndFlag);

	return TRUE;
}

BOOL CItemHeader::SaveToFile(CJCDFile *pJCDFile)
{
	JCD_SAVE_INT(m_iClass);
	JCD_SAVE_INT(m_iID);
	JCD_SAVE_INT(m_iImageIndex);
	JCD_SAVE_INT(m_iUnknown);
	JCD_SAVE_INT(m_iParentID);
	JCD_SAVE_INT(m_iType);
	JCD_SAVE_INT(m_iEndFlag);

	return TRUE;

}
