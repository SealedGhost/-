/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.24                          *
*        Compiled Jan 27 2014, 11:28:24                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "Config.h"
#include "MainTask.h"
#include "Drawinformation.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x10)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x11)
#define ID_TEXT_0     (GUI_ID_USER + 0x12)
#define ID_TEXT_1     (GUI_ID_USER + 0x13)
#define ID_TEXT_2     (GUI_ID_USER + 0x14)
#define ID_TEXT_3     (GUI_ID_USER + 0x15)
#define ID_TEXT_4     (GUI_ID_USER + 0x16)
#define ID_TEXT_5     (GUI_ID_USER + 0x17)
#define ID_TEXT_6     (GUI_ID_USER + 0x18)
#define ID_TEXT_7     (GUI_ID_USER + 0x19)
#define ID_TEXT_8     (GUI_ID_USER + 0x1a)

extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];

extern WM_HWIN hDlg_FishMap;
extern WM_HWIN menuWin;
extern WM_HWIN subWins[4];

static void myListViewListener(WM_MESSAGE* pMsg);
static void showSelectedBoatInfo(WM_HWIN thisListView);
static void updateListViewContent(WM_HWIN thisHandle);


static int index  = -1;

extern unsigned char * pStrBuf;

/* Rect 为监控船舶信息所占的区域 
* @ Attention: 矩形坐标是相对于窗口 subWins[0] 左上角点的坐标
*/
static GUI_RECT infoRect={LV_MoniteList_WIDTH,LV_MoniteList_Y,Win_Main_WIDTH-MenuLabel_WIDTH,480};
// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, SubWin_X, SubWin_Y, SubWin_WIDTH, SubWin_HEIGHT, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, LV_MoniteList_X, LV_MoniteList_Y, LV_MoniteList_WIDTH, LV_MoniteList_HEIGHT, 0, 0x0, 0 },
	
	{ TEXT_CreateIndirect, "Monite List",        ID_TEXT_0, LV_MoniteList_X,     LV_MoniteList_Y-30,  200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "Monite boats info",  ID_TEXT_1, LV_MoniteList_WIDTH, LV_MoniteList_Y-40,  200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "Name:",              ID_TEXT_2, LV_MoniteList_WIDTH, LV_MoniteList_Y+40,  200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "N:",                 ID_TEXT_3, LV_MoniteList_WIDTH, LV_MoniteList_Y+80,  200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "E:",                 ID_TEXT_4, LV_MoniteList_WIDTH, LV_MoniteList_Y+120, 200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "SOG:",               ID_TEXT_5, LV_MoniteList_WIDTH, LV_MoniteList_Y+160, 200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "XSAlarm:",           ID_TEXT_6, LV_MoniteList_WIDTH, LV_MoniteList_Y+200, 200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "FDAlarm:",           ID_TEXT_7, LV_MoniteList_WIDTH, LV_MoniteList_Y+240, 200, 30, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "ZMAlarm:",           ID_TEXT_8, LV_MoniteList_WIDTH, LV_MoniteList_Y+280, 200, 30, 0, 0x0, 0}
  // USER START (Optionally insert additional widgets)
  // USER END
};





/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:	
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00000080);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
	  WM_SetCallback(hItem, &myListViewListener);
	
	  LISTVIEW_SetFont(hItem, &GUI_Font28);
    LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_0_WIDTH, "Dis", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_1_WIDTH, "MMSI", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, LV_MoniteList_Col_2_WIDTH, "Ang", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
	  LISTVIEW_SetHeaderHeight(hItem,LV_MoniteList_Header_HEIGHT);
	  LISTVIEW_SetRowHeight(hItem,LV_MoniteList_Row_HEIGHT);
	  
	  LISTVIEW_SetFont(hItem, &GUI_Font24B_1);	  
	  LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL,GUI_WHITE);
	  LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL,DEEPBLUE);
		LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS,GUI_BLACK);
		LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
		LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL, DEEPBLUE);
		
		updateListViewContent(hItem);

    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
		
	case WM_PAINT:
	
		GUI_SetFont(&GUI_Font24_1);
		GUI_SetColor(GUI_YELLOW);
		GUI_SetTextMode(GUI_TM_TRANS);
	  
	  if( (index>=0) && (index<N_boat) )
		{
			GUI_DispStringAt(boat_list_p[index]->name,LV_MoniteList_WIDTH+90,80);
			
			lltostr(boat_list_p[index]->latitude,pStrBuf);
			GUI_DispStringExAt(pStrBuf,LV_MoniteList_WIDTH+20,120);	
			
			lltostr(boat_list_p[index]->longitude,pStrBuf);
			GUI_DispStringExAt(pStrBuf,LV_AllList_WIDTH,160);
			
			sprintf(pStrBuf, "%d", boat_list_p[index]->SOG);
			GUI_DispStringExAt(pStrBuf,LV_AllList_WIDTH+40,200);
  	}
		break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)	
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;

    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN sub0WinCreate(void);
WM_HWIN sub0WinCreate(void) {
  WM_HWIN hWin;
	
	
//  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, menuWin, 0, 0);
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END


/**监控列表 LISTVIEW 的监听器（回调函数）
*
*/
static void myListViewListener(WM_MESSAGE* pMsg)
{
	 const WM_KEY_INFO * pInfo;
	 WM_HWIN thisListView  = pMsg->hWin; 
   
	switch(pMsg->MsgId)
	{
//		case WM_PAINT:
//			
//		GUI_SetColor(GUI_BLACK);
//		GUI_SetFont(&GUI_Font24_1);
//		GUI_DispStringAt("egg",330,160);
//		GUI_DispStringAt("eggg",400,450);
//			LISTVIEW_Callback(pMsg);
//	  	break;
		case WM_SET_FOCUS:

      if(LISTVIEW_GetNumRows(pMsg->hWin) && (LISTVIEW_GetSel(pMsg->hWin)<0))
         LISTVIEW_SetSel(pMsg->hWin, 0);
      LISTVIEW_Callback(pMsg);
      break;
		case WM_KEY:
			pInfo  = (WM_KEY_INFO*)pMsg->Data.p;
		
		  switch(pInfo->Key)
			{
				case GUI_KEY_UP:
				case GUI_KEY_DOWN:
					LISTVIEW_Callback(pMsg);	
   				showSelectedBoatInfo(thisListView);			
					break;
				
				case GUI_KEY_LEFT:				
					WM_SetFocus(menuWin);
					break;
				case GUI_KEY_MENU:
//         WM_BringToBottom(thisListView);
					WM_BringToTop(hDlg_FishMap);
				  WM_SetFocus(hDlg_FishMap);
					break;
				default:
					LISTVIEW_Callback(pMsg);
					break;
			}
			break;
		
		default:
			LISTVIEW_Callback(pMsg);
			break;
	}
}


/**监控列表 LISTVIEW 条目更新
*
*/
static void updateListViewContent(WM_HWIN thisHandle)
{
	WM_HWIN  thisListView  = thisHandle;
	int i  = 0;
	int Cnt  = 0;
  int NumOfRows  = 0;
	
	NumOfRows  = LISTVIEW_GetNumRows(thisListView);
  
	for(i=0;i<N_boat;i++)
	{
//		if(boat_list_p[i]->isVisible )
//		{	
			Cnt++;
			
			if(Cnt > NumOfRows)
			{
				LISTVIEW_AddRow(thisListView, NULL);
				NumOfRows  = LISTVIEW_GetNumRows(thisListView);
			}
			
			sprintf(pStrBuf, "%ld", boat_list_p[i]->user_id);
			LISTVIEW_SetItemText(thisListView, 1, i, pStrBuf);
			
			sprintf(pStrBuf, "%d", boat_list_p[i]->true_heading);
			LISTVIEW_SetItemText(thisListView, 2, i, pStrBuf);
//		}
	}
	NumOfRows  = LISTVIEW_GetNumRows(thisListView);
	while(NumOfRows > Cnt)
	{
		LISTVIEW_DeleteRow(thisListView, NumOfRows-1);
		NumOfRows  = LISTVIEW_GetNumRows(thisListView);
	}
}


static void showSelectedBoatInfo(WM_HWIN thisHandle)
{
	
	WM_HWIN thisListView  = thisHandle;
	int SelectedRow  = -1;
	int i  = 0;
	
	long SelectedID  = 0;

	SelectedRow  = LISTVIEW_GetSel(thisListView);
	
	
	if(SelectedRow >= 0)
	{
		LISTVIEW_GetItemText(thisListView,1,SelectedRow,pStrBuf,10);
		
		SelectedID  = strtoi(pStrBuf);
		
		while((boat_list_p[i]->user_id!=SelectedID) && (i<3))
		{
			i++;
		}
		/// Find the boat
		if(i < 3)
		{	
       index  = i;
		}
		
		WM_InvalidateRect(subWins[0],&infoRect);

	}

}

/*************************** End of file ****************************/