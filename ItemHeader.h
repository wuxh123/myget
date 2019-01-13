// ItemHeader.h: interface for the CItemHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMHEADER_H__8E0F2CA6_E761_4595_B80D_CE4EAC84DFC2__INCLUDED_)
#define AFX_ITEMHEADER_H__8E0F2CA6_E761_4595_B80D_CE4EAC84DFC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//define the item class;
#define ITEM_CLASS_TREEITEM	1
#define ITEM_CLASS_LISTITEM 2
#define ITEM_CLASS_TREEITEM_WITH_LAST_PROPERTY_ON 5

//define Item Type
#define FLASHGET_ROOT_ITEM	1
#define DOWNLOADING_ITEM	2
#define DOWNLOADED_ITEM		4
#define DELETED_ITEM		6
#define OTHER_ITEMS			0

#define ITEM_OTHER_IMAGE_INDEX  6
//define ITEM Header constant
#define ITEM_HEADER_UNKNOWN		1 //mabye 1, 2, 9.
#define ITEM_HEADER_END_FLAG	0x0F

#define DOWNLOAD_CATEGORY_ID	2
class CJCDFile;


class CItemHeader  
{
public:
	BOOL SaveToFile(CJCDFile *pJCDFile);
	BOOL LoadFromFile(CJCDFile *pJCDFile);
	void SetType(int iType);
	int GetType();
	void SetParentID(int iParentID);
	int GetParentID();
	void SetImageIndex(int iImageIndex);
	int GetImageIndex();
	void SetID(int iID);
	int GetID();
	void SetClass(int iClass);
	int GetClass();
	CItemHeader(int iItemClass);
	virtual ~CItemHeader();

private:
	int m_iClass;
	int m_iID;
	int m_iImageIndex;
	int m_iUnknown;
	int m_iParentID;
	int m_iType;
	int m_iEndFlag;

};

#endif // !defined(AFX_ITEMHEADER_H__8E0F2CA6_E761_4595_B80D_CE4EAC84DFC2__INCLUDED_)
