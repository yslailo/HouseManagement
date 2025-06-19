// HouseManagementNEWDoc.cpp: CHouseManagementNEWDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HouseManagementNEW.h"
#endif

#include "HouseManagementNEWDoc.h"
#include "HouseManagementNEWView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHouseManagementNEWDoc

IMPLEMENT_DYNCREATE(CHouseManagementNEWDoc, CDocument)

BEGIN_MESSAGE_MAP(CHouseManagementNEWDoc, CDocument)
END_MESSAGE_MAP()


// CHouseManagementNEWDoc 构造/析构

CHouseManagementNEWDoc::CHouseManagementNEWDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CHouseManagementNEWDoc::~CHouseManagementNEWDoc()
{
}

BOOL CHouseManagementNEWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CHouseManagementNEWDoc 序列化

void CHouseManagementNEWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CHouseManagementNEWDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: 实现缩略图绘制");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CHouseManagementNEWDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由"分号"分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHouseManagementNEWDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHouseManagementNEWDoc 诊断

#ifdef _DEBUG
void CHouseManagementNEWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHouseManagementNEWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHouseManagementNEWDoc 命令

// 房屋管理功能实现 - 已移至HouseDBHelper数据库操作

// BOOL CHouseManagementNEWDoc::AddHouse(const HouseInfo& house)
// {
//     // 检查房屋ID是否已存在
//     if (FindHouse(house.strID) != nullptr)
//     {
//         return FALSE; // 房屋ID已存在
//     }
//     
//     m_houses.push_back(house);
//     SetModifiedFlag(TRUE);
//     UpdateAllViews(nullptr);
//     return TRUE;
// }

// BOOL CHouseManagementNEWDoc::DeleteHouse(const CString& strID)
// {
//     for (auto it = m_houses.begin(); it != m_houses.end(); ++it)
//     {
//         if (it->strID == strID)
//         {
//             m_houses.erase(it);
//             SetModifiedFlag(TRUE);
//             UpdateAllViews(nullptr);
//             return TRUE;
//         }
//     }
//     return FALSE; // 未找到指定房屋
// }

// BOOL CHouseManagementNEWDoc::UpdateHouse(const HouseInfo& house)
// {
//     for (auto& h : m_houses)
//     {
//         if (h.strID == house.strID)
//         {
//             h = house;
//             SetModifiedFlag(TRUE);
//             UpdateAllViews(nullptr);
//             return TRUE;
//         }
//     }
//     return FALSE; // 未找到指定房屋
// }

// HouseInfo* CHouseManagementNEWDoc::FindHouse(const CString& strID)
// {
//     for (auto& house : m_houses)
//     {
//         if (house.strID == strID)
//         {
//             return &house;
//         }
//     }
//     return nullptr;
// }

// std::vector<HouseInfo> CHouseManagementNEWDoc::SearchHouses(const CString& strKeyword)
// {
//     std::vector<HouseInfo> results;
//     CString strLowerKeyword = strKeyword;
//     strLowerKeyword.MakeLower();
//     
//     for (const auto& house : m_houses)
//     {
//         CString strLowerAddress = house.strAddress;
//         CString strLowerOwner = house.strOwner;
//         CString strLowerType = house.strType;
//         
//         strLowerAddress.MakeLower();
//         strLowerOwner.MakeLower();
//         strLowerType.MakeLower();
//         
//         if (strLowerAddress.Find(strLowerKeyword) >= 0 ||
//             strLowerOwner.Find(strLowerKeyword) >= 0 ||
//             strLowerType.Find(strLowerKeyword) >= 0 ||
//             house.strID.Find(strKeyword) >= 0)
//         {
//             results.push_back(house);
//         }
//     }
//     
//     return results;
// }

// BOOL CHouseManagementNEWDoc::SaveToFile(const CString& strFilePath)
// {
//     try
//     {
//         CStdioFile file;
//         if (!file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
//         {
//             return FALSE;
//         }
//         
//         // 写入文件头
//         file.WriteString(_T("房屋管理系统数据文件\n"));
//         file.WriteString(_T("ID,地址,类型,面积,价格,业主,状态,描述,联系方式\n"));
//         
//         // 写入房屋数据
//         for (const auto& house : m_houses)
//         {
//             CString strLine;
//             strLine.Format(_T("%s,%s,%s,%.2f,%.2f,%s,%s,%s,%s\n"),
//                 house.strID, house.strAddress, house.strType,
//                 house.dArea, house.dPrice, house.strOwner,
//                 house.strStatus, house.strDescription, house.strContact);
//             file.WriteString(strLine);
//         }
//         
//         file.Close();
//         SetModifiedFlag(FALSE);
//         return TRUE;
//     }
//     catch (...)
//     {
//         return FALSE;
//     }
// }

// BOOL CHouseManagementNEWDoc::LoadFromFile(const CString& strFilePath)
// {
//     try
//     {
//         CStdioFile file;
//         if (!file.Open(strFilePath, CFile::modeRead | CFile::typeText))
//         {
//             return FALSE;
//         }
//         
//         m_houses.clear();
//         
//         CString strLine;
//         int lineCount = 0;
//         
//         while (file.ReadString(strLine))
//         {
//             lineCount++;
//             if (lineCount <= 2) continue; // 跳过文件头和标题行
//             
//             // 解析CSV格式的数据
//             int pos = 0;
//             HouseInfo house;
//             
//             // 解析各个字段
//             house.strID = strLine.Tokenize(_T(","), pos);
//             house.strAddress = strLine.Tokenize(_T(","), pos);
//             house.strType = strLine.Tokenize(_T(","), pos);
//             
//             CString strArea = strLine.Tokenize(_T(","), pos);
//             house.dArea = _ttof(strArea);
//             
//             CString strPrice = strLine.Tokenize(_T(","), pos);
//             house.dPrice = _ttof(strPrice);
//             
//             house.strOwner = strLine.Tokenize(_T(","), pos);
//             house.strStatus = strLine.Tokenize(_T(","), pos);
//             house.strDescription = strLine.Tokenize(_T(","), pos);
//             house.strContact = strLine.Tokenize(_T(","), pos);
//             
//             m_houses.push_back(house);
//         }
//         
//         file.Close();
//         SetModifiedFlag(FALSE);
//         UpdateAllViews(nullptr);
//         return TRUE;
//     }
//     catch (...)
//     {
//         return FALSE;
//     }
// }

// 客户管理功能实现 - 已迁移到数据库操作，删除旧代码
