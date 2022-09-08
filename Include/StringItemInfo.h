
#ifndef TAG_CStringItemInfo
#define TAG_CStringItemInfo

#include "ItemInfo.h"
#include "zoomview.h"


class _FHZOOMVIEW_EXT_CLASS CStringItemInfo : public CItemInfo  
{
public:

	CStringItemInfo(int nItem):CItemInfo(nItem)
	{
	};

    virtual ~CStringItemInfo() 
	{
		
		char *pString=NULL;
		std::list <char *>::iterator p;
		for ( p = m_list.begin(); p!= m_list.end(); ++p )
		{
			pString=*p;
			delete []p;
		}

		m_list.clear ();
	};

	void AppendItem(char s[])
	{
		char *p=new char [strlen(s)+1];
		strcpy(p,s);
		m_list.push_back(s);
	};


public:
	std::list <char *> m_list;
};


#endif