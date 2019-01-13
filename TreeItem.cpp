#include "stdafx.h"
#include "JCDFile.h"
#include "TreeItem.h"
#include "ListItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CTreeItem::CTreeItem() : CItemHeader(ITEM_CLASS_TREEITEM)
{
	m_arListItem.RemoveAll();
}

CTreeItem::~CTreeItem()
{
	prv_FreeMEM();
};

BOOL CTreeItem::IsSystemItem()
{
 	switch(GetType())
	{
	case FLASHGET_ROOT_ITEM:
	case DOWNLOADING_ITEM:
	case DOWNLOADED_ITEM:
	case DELETED_ITEM:
		return TRUE;
	case OTHER_ITEMS:
	default:
		return FALSE;
	}
}

LPCTSTR CTreeItem::GetCategoryName()
{
	return m_strCategoryName;
}

LPCTSTR CTreeItem::GetDefaultDir()
{
	return m_strDefaultDir;
}

CListItem *CTreeItem::GetSubListItem(DWORD dwIndex)
{
	return m_arListItem.GetAt(dwIndex);
}


void CTreeItem::SetCategoryName(LPCTSTR pszCategoryName)
{
	m_strCategoryName = pszCategoryName;
}



void CTreeItem::SetDefaultDir(LPCTSTR pszDefaultDir)
{
	m_strDefaultDir = pszDefaultDir;
}


int CTreeItem::GetSubListItemCount()
{
	return m_arListItem.GetSize();
}

BOOL CTreeItem::AddSubListItem(CListItem *pListItem)
{
	m_arListItem.InsertAt(0, pListItem);

	return TRUE;
}

BOOL CTreeItem::LoadFromFile(CJCDFile *pJCDFile)
{
	//Call base method first.
	CItemHeader::LoadFromFile(pJCDFile);

	JCD_LOAD_STR(m_strCategoryName);
	JCD_LOAD_STR(m_strDefaultDir);

	return TRUE;
}

BOOL CTreeItem::SaveToFile(CJCDFile *pJCDFile)
{
	CItemHeader::SaveToFile(pJCDFile);

	//Save CategoryName
	JCD_SAVE_STR(m_strCategoryName);
	JCD_SAVE_STR(m_strDefaultDir);


	CListItem *pListItem;

	for (int i = GetSubListItemCount() - 1; i >= 0; i --)
	{
		pListItem = m_arListItem[i];

		pListItem->SaveToFile(pJCDFile);
	}

	return TRUE;
}


BOOL CTreeItem::prv_FreeMEM()
{
	CListItem *pListItem;

	for (int i = 0; i < GetSubListItemCount(); i ++)
	{
		pListItem = m_arListItem[i];

		delete pListItem;
	}

	m_arListItem.RemoveAll();

	return TRUE;
}

void CTreeItem::ClearSubListItems()
{
	m_arListItem.RemoveAll();
}

void CTreeItem::RemoveSubListItem(CListItem *pListItem)
{
	for (int i =0; i < m_arListItem.GetSize(); i ++)
	{
		if (pListItem == m_arListItem[i])
		{
			m_arListItem.RemoveAt(i);

			return;
		}

	}
}
