/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.38 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : MainTask_TemperatureControl.c
Purpose     : Shows how to create a radial menu with motion support
Requirements: WindowManager - (x)
              MemoryDevices - (x)
              AntiAliasing  - (x)
              VNC-Server    - ( )
              PNG-Library   - ( )
              TrueTypeFonts - ( )
----------------------------------------------------------------------
*/

#include "LCDConf.h"
#include "DIALOG.h"
#include "Resource.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Widget IDs
//
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_KNOB_0      (GUI_ID_USER + 0x01)
#define ID_WINDOW_1    (GUI_ID_USER + 0x02)

#define ID_BUTTON_0    (GUI_ID_USER + 0x11)
#define ID_BUTTON_1    (GUI_ID_USER + 0x12)
#define ID_BUTTON_2    (GUI_ID_USER + 0x13)
#define ID_BUTTON_3    (GUI_ID_USER + 0x14)
#define ID_BUTTON_4    (GUI_ID_USER + 0x15)
#define ID_BUTTON_5    (GUI_ID_USER + 0x16)
#define ID_BUTTON_6    (GUI_ID_USER + 0x17)

#define ID_IMAGE_0     (GUI_ID_USER + 0x20)

#define ID_TEMPERATURE (GUI_ID_USER + 0x30)

//
// Some dimension defines
//
#define KNOB_X0 500
#define KNOB_Y0 139

#define KNOB_OFFSET         450
#define FAN_CONTROL_0_X0    KNOB_X0 + 20
#define FAN_CONTROL_0_Y0    KNOB_Y0 + 255
#define FAN_CONTROL_SIZE_X  80
#define FAN_CONTROL_SIZE_Y  35
#define FAN_CONTROL_1_X0    FAN_CONTROL_0_X0 + FAN_CONTROL_SIZE_X + 1
#define FAN_CONTROL_1_Y0    FAN_CONTROL_0_Y0

//
// Colors
//
#define DARK_BLUE   0x613600
#define LIGHT_BLUE  0xaa7d67
#define BLUE        0x855a41
#define LEMON       0x00d6d3

#define MAX_TEMPERATURE  30
#define MIN_TEMPERATURE   2

/*********************************************************************
*
*       Structures
*
**********************************************************************
*/

typedef struct {
  int  ID;              // Id of button
  char aName[20];       // Name to be displayed
  int  RealTemperature; // Current temperature
  int  SetTemperature;  // Temperature to be achieved
  int  Focus;           // Is this button focused
  int  FanState;        // Set the state of the fan button, 3 states: 0-2
} ROOM_DATA;

typedef struct {
  int ID;
  int Focus;
  char aName[20];
} TOGGLE_BUTTON;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Fill ROOM_DATA struct
//
static ROOM_DATA _aRooms[5] = {
  { 0, "Bedroom",        14, 0, 1, 0 },
  { 0, "Living Room",    21, 0, 0, 0 },
  { 0, "Kitchen",        19, 0, 0, 0 },
  { 0, "Child's Room 1", 21, 0, 0, 0 },
  { 0, "Child's Room 2", 22, 0, 0, 0 },
};

//
// Fill toggle button structure
//
static TOGGLE_BUTTON _aButton[3] = {
  { 0, 0, "On"},
  { 0, 0, "Auto"},
};

//
// Set pointer to a font, used for an easier exchange of fonts
//
static GUI_CONST_STORAGE GUI_FONT * pFont23pBold = &GUI_Font23S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32pBold = &GUI_Font32S_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont23p     = &GUI_Font23SL_AA4;
static GUI_CONST_STORAGE GUI_FONT * pFont32p     = &GUI_Font32SL_AA4;

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window",         ID_WINDOW_0,   0,   0, 800, 480, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Bedroom",        ID_BUTTON_0,   0,  80, 400,  80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Living Room",    ID_BUTTON_1,   0, 160, 400,  80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Kitchen",        ID_BUTTON_2,   0, 240, 400,  80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Child's Room 1", ID_BUTTON_3,   0, 320, 400,  80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Child's Room 2", ID_BUTTON_4,   0, 400, 400,  80, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "On",             ID_BUTTON_5,   FAN_CONTROL_0_X0, FAN_CONTROL_0_Y0, FAN_CONTROL_SIZE_X, FAN_CONTROL_SIZE_Y, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Auto",           ID_BUTTON_6,   FAN_CONTROL_1_X0, FAN_CONTROL_0_Y0, FAN_CONTROL_SIZE_X, FAN_CONTROL_SIZE_Y, 0, 0x0, 0 },
  { IMAGE_CreateIndirect,  "",               ID_IMAGE_0,    10,   5, 60,  30, 0, 0x0, 0 }
};

/*********************************************************************
*
*       _aGradient
*/
static const GUI_COLOR _aGradient[] = {
  0x00BC944C, 0x00BC944C, 0x00BB944C, 0x00BA944C, 0x00BA944C, 0x00B9944C, 0x00B9944C, 0x00B8944C, 0x00B7944C, 0x00B7944C, 0x00B6944C, 0x00B5944C, 0x00B5944C, 0x00B4944C, 0x00B3944C, 0x00B3944C, 0x00B2944C, 0x00B1944C, 0x00B0944C, 0x00AF944C, 0x00AE944C, 
  0x00AE944C, 0x00AD944C, 0x00AC944C, 0x00AB944C, 0x00AA944C, 0x00A9944C, 0x00A8944C, 0x00A7944C, 0x00A6944C, 0x00A5944C, 0x00A4944C, 0x00A3944C, 0x00A2944C, 0x00A1944C, 0x00A0944C, 0x009F944C, 0x009E944C, 0x009D944C, 0x009C944C, 0x009B944C, 
  0x009A944C, 0x0099944C, 0x0098944C, 0x0097944C, 0x0096944C, 0x0094944C, 0x0093944C, 0x0092944C, 0x0091944C, 0x0090944C, 0x008F944C, 0x008E944C, 0x008C944C, 0x008B944C, 0x008A944C, 0x0089944C, 0x0088944C, 0x0087944C, 0x0085944C, 0x0084944C, 
  0x0083944C, 0x0082944C, 0x0081944C, 0x007F944C, 0x007E944C, 0x007D944C, 0x007C944C, 0x007B944C, 0x0079944C, 0x0078944C, 0x0077944C, 0x0076944C, 0x0075944C, 0x0074944C, 0x0072944C, 0x0071944C, 0x0070944C, 0x006F944C, 0x006E944C, 0x006D944C, 
  0x006B944C, 0x006A944C, 0x0069944C, 0x0068944C, 0x0067944C, 0x0066944C, 0x0065944C, 0x0063944C, 0x0062944C, 0x0061944C, 0x0060944C, 0x005F944C, 0x005E944C, 0x005D944C, 0x005C944C, 0x005B944C, 0x005A944C, 0x0059944C, 0x0058944C, 0x0057944C, 
  0x0056944C, 0x0055944C, 0x0054944C, 0x0053944C, 0x0052944C, 0x0051944C, 0x0050944C, 0x004F944C, 0x004E944C, 0x004D944C, 0x004C944C, 0x004C944C, 0x004B944C, 0x004A944C, 0x0049944C, 0x0048944C, 0x0047944C, 0x0047944C, 0x0046944C, 0x0045944C, 
  0x0044944C, 0x0044944C, 0x0043944C, 0x0042944C, 0x0042944C, 0x0041944C, 0x0041944C, 0x0040944C, 0x003F944D, 0x003F944D, 0x003E944E, 0x003E944F, 0x003D934F, 0x003D9350, 0x003C9351, 0x003C9352, 0x003B9352, 0x003B9353, 0x003A9354, 0x003A9355, 
  0x00399256, 0x00399256, 0x00399257, 0x00389258, 0x00389259, 0x0037925A, 0x0037915B, 0x0037915C, 0x0036915D, 0x0036915E, 0x0035915F, 0x00359160, 0x00359061, 0x00349062, 0x00349063, 0x00349064, 0x00339066, 0x00338F67, 0x00338F68, 0x00338F69, 
  0x00328F6A, 0x00328F6B, 0x00328E6D, 0x00328E6E, 0x00318E6F, 0x00318E70, 0x00318D71, 0x00318D73, 0x00308D74, 0x00308D75, 0x00308D77, 0x00308C78, 0x00308C79, 0x002F8C7A, 0x002F8C7C, 0x002F8B7D, 0x002F8B7E, 0x002F8B80, 0x002E8B81, 0x002E8A82, 
  0x002E8A84, 0x002E8A85, 0x002E8A86, 0x002E8988, 0x002E8989, 0x002D898A, 0x002D898C, 0x002D888D, 0x002D888F, 0x002D8890, 0x002D8891, 0x002D8893, 0x002D8794, 0x002D8795, 0x002C8797, 0x002C8798, 0x002C869A, 0x002C869B, 0x002C869C, 0x002C869E, 
  0x002C859F, 0x002C85A0, 0x002C85A2, 0x002C85A3, 0x002C84A4, 0x002C84A6, 0x002C84A7, 0x002C84A8, 0x002B83AA, 0x002B83AB, 0x002B83AC, 0x002B83AD, 0x002B82AF, 0x002B82B0, 0x002B82B1, 0x002B82B3, 0x002B82B4, 0x002B81B5, 0x002B81B6, 0x002B81B7, 
  0x002B81B9, 0x002B80BA, 0x002B80BB, 0x002B80BC, 0x002B80BD, 0x002B80BE, 0x002B7FC0, 0x002B7FC1, 0x002B7FC2, 0x002B7FC3, 0x002A7FC4, 0x002A7EC5, 0x002A7EC6, 0x002A7EC7, 0x002A7EC8, 0x002A7EC9, 0x002A7ECA, 0x002A7DCB, 0x002A7DCC, 0x002A7DCD, 
  0x002A7DCE, 0x002A7DCE, 0x002A7DCF, 0x002A7CD0, 0x002A7CD1, 0x002A7CD2, 0x002A7CD2, 0x002A7CD3, 0x002A7CD4, 0x00297CD5, 0x00297CD5, 0x00297BD6, 0x00297BD7, 0x00297BD7, 0x00297BD8
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _GetSelectedRoom
*
* Function description:
*  This routine returns the index of of a room inside the ROOM_DATA
*  structure.
*
*/
static int _GetSelectedRoom(void) {
  int i;

  for (i = 0; i < 5; i++) {
    if (_aRooms[i].Focus) {
      return i;
    }
  }
  return 0;
}

/*********************************************************************
*
*       _SetFanButtonState
*
* Function description:
*  This routine sets the states of the fan control buttons.
*
*/
static void _SetFanButtonState(WM_MESSAGE  * pMsg, int Id) {
  int IndexButton;
  int IndexRoom;
  int i;
  WM_HWIN hItem;

  IndexButton = Id - ID_BUTTON_5;
  _aButton[IndexButton].Focus     = (_aButton[IndexButton].Focus) ? 0 : 1;
  _aButton[1 - IndexButton].Focus = 0;
  for (i = 0; i < 2; i++) {
    WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aButton[i].ID));
  }
  IndexRoom = _GetSelectedRoom();
  _aRooms[IndexRoom].FanState = (_aButton[IndexButton].Focus == 0) ? 0 : (IndexButton) ? 2 : 1;
  hItem = WM_GetDialogItem(pMsg->hWin, IndexRoom + ID_BUTTON_0);
  WM_InvalidateWindow(hItem);
}

/*********************************************************************
*
*       _cbButton
*
* Function description:
*  This is the callback function for the buttons which represent the
*  different rooms.
*
*/
static void _cbButton(WM_MESSAGE * pMsg) {
  GUI_RECT Rect;
  GUI_COLOR ColorFrame;
  GUI_COLOR Color;
  GUI_COLOR ColorText;
  GUI_CONST_STORAGE GUI_FONT * pFont23;
  GUI_CONST_STORAGE GUI_FONT * pFont32;
  int Index;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    //
    // Get index and check if it is focused or not
    //
    Index = WM_GetId(pMsg->hWin) - ID_BUTTON_0;
    if (_aRooms[Index].Focus == 1) {            // Set colors dependend on focus state
      Color = LIGHT_BLUE;
      ColorText = LEMON;
      ColorFrame = GUI_WHITE;
      pFont23 = pFont23pBold;
      pFont32 = pFont32pBold;
    } else {
      Color = DARK_BLUE;
      ColorText = GUI_WHITE;
      ColorFrame = GUI_WHITE;
      pFont23 = pFont23p;
      pFont32 = pFont32p;
    }
    //
    // Draw thin frame around the button
    //
    WM_GetClientRectEx(pMsg->hWin, &Rect);
    GUI_SetColor(ColorFrame);
    GUI_FillRectEx(&Rect);
    GUI_SetColor(Color);
    //
    // Resize drawing rect
    //
    Rect.x0 += 1;
    Rect.x1 -= 1;
    Rect.y1 -= 1;
    //
    // Draw rest of the button
    //
    GUI_AA_FillRoundedRectEx(&Rect, 3);
    //
    // Only button 0 needs a thin gradient
    //
    if (WM_GetId(pMsg->hWin) == ID_BUTTON_0) {
      GUI_DrawGradientV(0, 0, 399, 7, GUI_BLACK, Color);
      GUI_DrawGradientV(0, 0, 0, 7, GUI_BLACK, ColorFrame);
      GUI_DrawGradientV(399, 0, 400, 7, GUI_BLACK, ColorFrame);
    }
    //
    // Draw Text
    //
    GUI_SetFont(pFont23);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(ColorText);
    Rect.x0 += 10;
    GUI_DispStringInRect((char *)_aRooms[Index].aName, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Display fan status
    //
    GUI_SetFont(pFont23);
    GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
    GUI_GotoXY(Rect.x1 - 196, Rect.y1 / 2);
    if (_aRooms[Index].FanState == 0) {
      GUI_DispString("Off");
    } else if (_aRooms[Index].FanState == 1) {
      GUI_DispString("On");
    } else {
      GUI_DispString("Auto");
    }
    //
    // Display temperature with a bigger font
    //
    GUI_SetFont(pFont32);
    GUI_SetTextAlign(GUI_TA_RIGHT | GUI_TA_VCENTER);
    GUI_GotoXY(Rect.x1 - 38, Rect.y1 / 2);
    GUI_DispDecSpace(_aRooms[Index].RealTemperature, 2);
    GUI_DispString("�C");
    break;
  default:
    BUTTON_Callback(pMsg); // The original callback
    break;
  }
}

/*********************************************************************
*
*       _cbButtonFan
*/
static void _cbButtonFan(WM_MESSAGE * pMsg) {
  GUI_RECT Rect;
  int Index;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    WM_GetClientRectEx(pMsg->hWin, &Rect);
    if (BUTTON_IsPressed(pMsg->hWin)) {
      GUI_DrawBitmap(&bmButtonRoundPressed_80x35, 0, 0);
    } else {
      GUI_DrawBitmap(&bmButtonRound_80x35, 0, 0);
    }
    Index = WM_GetId(pMsg->hWin) - ID_BUTTON_5;
    if (_aButton[Index].Focus == 1) {            // Choos text color
      GUI_SetColor(LEMON);
    } else {
      GUI_SetColor(GUI_DARKGRAY);
    }
    //
    // Draw Text
    //
    GUI_SetFont(pFont23pBold);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_DispStringInRect(_aButton[Index].aName, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
    break;
  default:
    BUTTON_Callback(pMsg); // The original callback
    break;
  }
}

/*********************************************************************
*
*       _cbKnob
*
* Function description:
*   Fills the background of the KNOB with transparency
*/
static void _cbKnob(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    GUI_SetBkColor(GUI_TRANSPARENT);
    GUI_Clear();
    KNOB_Callback(pMsg); // The original callback
    break;
  default:
    KNOB_Callback(pMsg); // The original callback
    break;
  }
}

/*********************************************************************
*
*       _cbKnobTemperature
*
* Function description:
*   Shows the current temperature (text and color) behind the center of the KNOB
*/
static void _cbKnobTemperature(WM_MESSAGE * pMsg) {
  GUI_RECT Rect;
  int Index, Value;
  GUI_COLOR Color;

  switch (pMsg->MsgId) {
  case WM_GET_ID:
    pMsg->Data.v = ID_TEMPERATURE;
    break;
  case WM_PAINT:
    Index = _GetSelectedRoom();
    Value = _aRooms[Index].RealTemperature;
    WM_GetClientRect(&Rect);
    Index = ((Value - MIN_TEMPERATURE) * (GUI_COUNTOF(_aGradient) - 1)) / (MAX_TEMPERATURE - MIN_TEMPERATURE);
    Color = _aGradient[Index];
    GUI_SetBkColor(Color);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign(GUI_TA_RIGHT | GUI_TA_VCENTER);
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_GotoXY(Rect.x1 / 2, Rect.y1 / 2);
    GUI_SetFont(pFont32p);
    GUI_DispDecSpace(Value, 2);
    GUI_DispString("�C");
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _cbKnobParent
*
* Function description:
*   Responsible only for processing feedback from KNOB. No drawing.
*/
static void _cbKnobParent(WM_MESSAGE * pMsg) {
  WM_HWIN hParent, hItem;
  int Id;
  int NCode;
  int KnobValue;
  int i;

  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_KNOB_0: // Notifications sent by 'Knob'
      switch(NCode) {
      case WM_NOTIFICATION_VALUE_CHANGED:
        hItem = WM_GetDialogItem(pMsg->hWin, Id);
        KnobValue = KNOB_GetValue(hItem);
        //
        // Invalidate button
        //
        hParent = WM_GetFirstChild(WM_GetDesktopWindowEx(0));
        for (i = 0; i < 5; i++) {
          if (_aRooms[i].Focus == 1) {
            hParent = WM_GetFirstChild(WM_GetDesktopWindowEx(0));
            _aRooms[i].RealTemperature = MAX_TEMPERATURE - KnobValue * (MAX_TEMPERATURE - MIN_TEMPERATURE) / 2700;
            WM_InvalidateWindow(WM_GetDialogItem(hParent, _aRooms[i].ID));
            break;
          }
        }
        //
        // Invalidate temperature
        //
        hItem = WM_GetDialogItem(hParent, ID_TEMPERATURE);
        WM_InvalidateWindow(hItem);
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _DrawKnob
*
* Function description:
*   Fill memory device used for drawing the KNOB
*/
static void _DrawKnob(GUI_MEMDEV_Handle hMem, GUI_COLOR Color) {
  int xSizeMem, ySizeMem;

  xSizeMem = GUI_MEMDEV_GetXSize(hMem);
  ySizeMem = GUI_MEMDEV_GetYSize(hMem);
  GUI_AA_PreserveTrans(1);
  GUI_MEMDEV_Select(hMem);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_SetColor(Color);
  GUI_FillRect(xSizeMem - 16, 0, xSizeMem, ySizeMem - 1);
  GUI_SetColor(DARK_BLUE);
  GUI_DrawLine(xSizeMem - 16, ySizeMem / 2, xSizeMem, ySizeMem / 2);
  GUI_SetColor(Color);
  GUI_MEMDEV_Select(0);
  GUI_AA_PreserveTrans(0);
}

/*********************************************************************
*
*       _OnRelease
*/
static void _OnRelease(WM_MESSAGE * pMsg, int Id) {
  WM_HWIN hItem, hParent;
  int     Value;
  int     Index;
  int     i;

  Index = Id - ID_BUTTON_0;
  for (i = 0; i < 5; i++) {
    _aRooms[i].Focus = 0;
    if (_aRooms[i].ID != Id) {
      WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aRooms[i].ID));
    }
  }
  hParent = WM_GetDesktopWindowEx(1);
  hItem = WM_GetDialogItem(hParent, ID_KNOB_0);
  Value = (MAX_TEMPERATURE - _aRooms[Index].RealTemperature) * 2700 / MAX_TEMPERATURE - MIN_TEMPERATURE;
  KNOB_SetPos(hItem, Value);
  _aRooms[Index].Focus = 1;
  //
  // Check states for fan buttons
  //
  if (_aRooms[Index].FanState == 0) {
    _aButton[0].Focus = 0;
    _aButton[1].Focus = 0;
  } else if (_aRooms[Index].FanState == 1) {
    _aButton[0].Focus = 1;
    _aButton[1].Focus = 0;
  } else {
    _aButton[0].Focus = 0;
    _aButton[1].Focus = 1;
  }
  //
  // Tell fan buttons to redraw
  //
  for (i = 0; i < 2; i++) {
    WM_InvalidateWindow(WM_GetDialogItem(pMsg->hWin, _aButton[i].ID));
  }
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  static GUI_MEMDEV_Handle hMemKnob;
  static GUI_COLOR         ColorKnob = GUI_WHITE;
  WM_HWIN                  hItem;
  WM_HWIN                  hParent;
  WM_HWIN                  hWin;
  int                      NCode;
  int                      Id;
  int                      i;
  int                      Value;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(DARK_BLUE));
    //
    // Create window showing the current temperature (layer 0)
    //
    WM_CreateWindowAsChild(KNOB_X0 + 42, KNOB_Y0 + 43, 115, 113, pMsg->hWin, WM_CF_SHOW, _cbKnobTemperature, 0);
    //
    // Switch to layer 1 and get desktop window
    //
    GUI_SelectLayer(1);
    hParent = WM_GetDesktopWindowEx(1);
    //
    // Create background window for knob which is only responsible for getting feedback from KNOB
    //
    hWin = WM_CreateWindowAsChild(0, 0, 200, 200, hParent, WM_CF_SHOW, _cbKnobParent, 0);
    //
    // Create KNOB itself
    //
    KNOB_CreateUser(0, 0, 200, 200, hWin, WM_CF_SHOW | WM_CF_MEMDEV, ID_KNOB_0, 0);
    //
    // Create memory device for KNOB
    //
    hMemKnob   = GUI_MEMDEV_CreateFixed32(0, 0, 200, 5);
    _DrawKnob(hMemKnob, ColorKnob);
    //
    // All creating jobs done, switch back to right layer
    //
    GUI_SelectLayer(0);
    //
    // Set several KNOB properties
    //
    hItem = WM_GetDialogItem(hParent, ID_KNOB_0);
    WM_SetCallback(hItem, _cbKnob);
    KNOB_SetDevice(hItem, hMemKnob);
    KNOB_SetRange(hItem, 0, 2700);
    KNOB_SetOffset(hItem, -KNOB_OFFSET);
    KNOB_SetPeriod(hItem,  500);
    Value = (MAX_TEMPERATURE - _aRooms[0].RealTemperature) * 2700 / MAX_TEMPERATURE - MIN_TEMPERATURE;
    KNOB_SetPos(hItem, Value);
    //
    // Initialization Buttons for each room
    //
    for (i = 0; i < 5; i++) {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0 + i);
      WM_SetCallback(hItem, _cbButton);
      _aRooms[i].ID = ID_BUTTON_0 + i;
    }
    //
    // Initialization Buttons for fan
    //
    for (i = 0; i < 2; i++) {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5 + i);
      WM_SetCallback(hItem, _cbButtonFan);
      _aButton[i].ID = ID_BUTTON_5 + i;
    }
    //
    // Init logo
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    IMAGE_SetBitmap(hItem, &bmSeggerLogo_60x30);
    break;
  case WM_PAINT:
    GUI_DrawBitmap(&bmTemperatureControlSegger800x480, 0, 0);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Bedroom'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _OnRelease(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Living Room'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _OnRelease(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Kitchen'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _OnRelease(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'Child's Room 1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _OnRelease(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Child's Room 2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _OnRelease(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_5: // Notifications sent by 'On'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _SetFanButtonState(pMsg, Id);
        break;
      }
      break;
    case ID_BUTTON_6: // Notifications sent by 'Auto'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        _SetFanButtonState(pMsg, Id);
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _PID_Hook
*/
static void _PID_Hook(GUI_PID_STATE * pState) {
  if (pState->Layer == 0) {
    if ((pState->x >= KNOB_X0) && (pState->y >= KNOB_Y0) && (pState->x < (KNOB_X0 + 200)) && (pState->y < (KNOB_Y0 + 200))) {
      pState->Layer = 1;
    }
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
*       MainTask
*/
void MainTask(void) {
  GUI_Init();
  //
  // Set hook function to change layer of PID input within range of KNOB-layer
  //
  GUI_PID_SetHook(_PID_Hook);
  //
  // Prepare layers
  //
  GUI_SelectLayer(2);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_DrawBitmap(&bmKNOB_175x175, 0, 0);
  GUI_SelectLayer(1);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_SelectLayer(0);
  LCD_SetVisEx(1, 1);
  LCD_SetVisEx(2, 1);
  WM_MULTIBUF_Enable(1);
  //
  // Create application window...
  //
  GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  //
  // ...and keep it alive
  //
  while (1) {
    GUI_Delay(100);
  }
}

/*************************** End of file ****************************/
