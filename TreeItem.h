
#ifndef _TREE_ITEM_H
#define _TREE_ITEM_H


#include "ItemHeader.h"

class CListItem;


class CTreeItem : public CItemHeader
{
public:
	void RemoveSubListItem(CListItem *pListItem);
	void ClearSubListItems();
	BOOL SaveToFile(CJCDFile *pJCDFile);
	BOOL LoadFromFile(CJCDFile *pJCDFile);
	BOOL AddSubListItem(CListItem *pListItem);
	int GetSubListItemCount();
	void SetDefaultDir(LPCTSTR pszDefaultDir);
	void SetCategoryName(LPCTSTR pszCategoryName);
	CListItem *GetSubListItem(DWORD dwIndex);
	LPCTSTR GetDefaultDir();
	LPCTSTR GetCategoryName();
	BOOL IsSystemItem();
	CTreeItem();
	~CTreeItem();

protected:
	CString		m_strCategoryName;
	CString		m_strDefaultDir;

	CArray <CListItem *, CListItem *> m_arListItem;
private:
	BOOL prv_FreeMEM();
};


#endif //_TREE_ITEM_H