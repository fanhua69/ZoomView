; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInfoView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "zoomview.h"
LastPage=0

ClassCount=29
Class1=CCMPBaseView
Class2=CColorDlg
Class3=CDlgAskOneValue
Class4=CDlgEditViewSize
Class5=CDlgMonitor
Class6=CDlgPassword
Class7=CDlgSeisDataProperty
Class8=CDlgTreeProperty
Class9=CFHEditDoc
Class10=CFHEditView
Class11=CFHPointLocDoc
Class12=CFHPointLocView
Class13=CFHTreeView
Class14=CFHZoomApprDlg
Class15=CFHZoomView
Class16=CGridCtrl
Class17=CGridDropTarget
Class18=CInfoDlg
Class19=CInfoView
Class20=CInfoViewPar
Class21=CInPlaceEdit
Class22=CInPlaceList
Class23=CProgressDlg
Class24=CSeisDataBaseDoc
Class25=CSeisDataBaseView
Class26=CSplitterTabWnd
Class27=CWndTab
Class28=CXYZDoc
Class29=CXYZView

ResourceCount=17
Resource1=IDR_InfoViewPopup
Resource2=IDD_FHEditViewSetSize
Resource3=IDD_FHZoomAppearance
Resource4=IDD_InfoDlg
Resource5=IDD_DlgColorModal
Resource6=IDD_AskOneValue
Resource7=IDD_DlgTreeProperty
Resource8=CG_IDD_PROGRESS
Resource9=IDD_StackSectionPar
Resource10=IDD_DlgPassword
Resource11=IDD_ProgressMonitor
Resource12=IDD_DlgSetSeisDataType
Resource13=IDD_DlgSeisDataProperty
Resource14=IDR_FHEditViewPopUp
Resource15=IDR_FHZoomViewPopUp
Resource16=IDR_FHTreeViewPopUp
Resource17=IDR_FHZoomViewToolBar

[CLS:CCMPBaseView]
Type=0
BaseClass=CSeisDataBaseView
HeaderFile=include\CMPBaseView.h
ImplementationFile=source\CMPBaseView.cpp
LastObject=CCMPBaseView

[CLS:CColorDlg]
Type=0
BaseClass=CDialog
HeaderFile=include\ColorScale.h
ImplementationFile=source\ColorScale.cpp

[CLS:CDlgAskOneValue]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgAskOneValue.h
ImplementationFile=source\DlgAskOneValue.cpp

[CLS:CDlgEditViewSize]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgEditViewSize.h
ImplementationFile=source\DlgEditViewSize.cpp

[CLS:CDlgMonitor]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgMonitor.h
ImplementationFile=source\DlgMonitor.cpp

[CLS:CDlgPassword]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgPassword.h
ImplementationFile=source\DlgPassword.cpp

[CLS:CDlgSeisDataProperty]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgSeisDataProperty.h
ImplementationFile=source\DlgSeisDataProperty.cpp

[CLS:CDlgTreeProperty]
Type=0
BaseClass=CDialog
HeaderFile=include\DlgTreeProperty.h
ImplementationFile=source\DlgTreeProperty.cpp

[CLS:CFHEditDoc]
Type=0
BaseClass=CDocument
HeaderFile=include\FHEditDoc.h
ImplementationFile=source\FHEditDoc.cpp

[CLS:CFHEditView]
Type=0
BaseClass=CScrollView
HeaderFile=include\fheditview.h
ImplementationFile=source\fheditview.cpp

[CLS:CFHPointLocDoc]
Type=0
BaseClass=CFHEditDoc
HeaderFile=include\FHPointLocDoc.h
ImplementationFile=source\FHPointLocDoc.cpp

[CLS:CFHPointLocView]
Type=0
BaseClass=CFHZoomView
HeaderFile=include\FHPointLocView.h
ImplementationFile=source\FHPointLocView.cpp

[CLS:CFHTreeView]
Type=0
BaseClass=CTreeView
HeaderFile=include\fhtreeview.h
ImplementationFile=source\FHTreeView.cpp

[CLS:CFHZoomApprDlg]
Type=0
BaseClass=CDialog
HeaderFile=include\FHZoomApprDlg.h
ImplementationFile=source\FHZoomApprDlg.cpp

[CLS:CFHZoomView]
Type=0
BaseClass=CScrollView
HeaderFile=include\fhzoomview.h
ImplementationFile=source\fhzoomview.cpp

[CLS:CGridCtrl]
Type=0
BaseClass=CWnd
HeaderFile=include\GridCtrl.h
ImplementationFile=source\GridCtrl.cpp

[CLS:CGridDropTarget]
Type=0
BaseClass=COleDropTarget
HeaderFile=include\GridDropTarget.h
ImplementationFile=source\GridDropTarget.cpp

[CLS:CInfoDlg]
Type=0
BaseClass=CDialog
HeaderFile=include\InfoDlg.h
ImplementationFile=source\InfoDlg.cpp

[CLS:CInfoView]
Type=0
BaseClass=CScrollView
HeaderFile=include\InfoView.h
ImplementationFile=source\InfoView.cpp
LastObject=ID_IV_SetMapShowPar
Filter=C
VirtualFilter=VWC

[CLS:CInfoViewPar]
Type=0
BaseClass=CDialog
HeaderFile=include\InfoViewPar.h
ImplementationFile=source\InfoViewPar.cpp

[CLS:CInPlaceEdit]
Type=0
BaseClass=CEdit
HeaderFile=include\InPlaceEdit.h
ImplementationFile=source\InPlaceEdit.cpp

[CLS:CInPlaceList]
Type=0
BaseClass=CComboBox
HeaderFile=include\InPlaceList.h
ImplementationFile=source\InPlaceList.cpp

[CLS:CProgressDlg]
Type=0
BaseClass=CDialog
HeaderFile=include\ProgDlg.h
ImplementationFile=source\ProgDlg.cpp

[CLS:CSeisDataBaseDoc]
Type=0
BaseClass=CDocument
HeaderFile=include\SeisDataBaseDoc.h
ImplementationFile=source\SeisDataBaseDoc.cpp

[CLS:CSeisDataBaseView]
Type=0
BaseClass=CScrollView
HeaderFile=include\SeisDataBaseView.h
ImplementationFile=source\SeisDataBaseView.cpp

[CLS:CSplitterTabWnd]
Type=0
BaseClass=CSplitterWnd
HeaderFile=include\splitterTabWnd.h
ImplementationFile=source\splitterTabWnd.cpp

[CLS:CWndTab]
Type=0
BaseClass=CWnd
HeaderFile=include\splitterTabWnd.h
ImplementationFile=source\splitterTabWnd.cpp

[CLS:CXYZDoc]
Type=0
BaseClass=CFHEditDoc
HeaderFile=include\XYZDoc.h
ImplementationFile=source\XYZDoc.cpp

[CLS:CXYZView]
Type=0
BaseClass=CFHEditView
HeaderFile=include\XYZView.h
ImplementationFile=source\XYZView.cpp

[DLG:IDD_DlgColorModal]
Type=1
Class=CColorDlg
ControlCount=0

[DLG:IDD_AskOneValue]
Type=1
Class=CDlgAskOneValue
ControlCount=4
Control1=IDC_sValue,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_sAsk,static,1342308354

[DLG:IDD_FHEditViewSetSize]
Type=1
Class=CDlgEditViewSize
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308354
Control4=IDC_STATIC,static,1342308354
Control5=IDC_RowNumber,edit,1350631552
Control6=IDC_ColNumber,edit,1350631552

[DLG:IDD_ProgressMonitor]
Type=1
Class=CDlgMonitor
ControlCount=9
Control1=IDCANCEL,button,1342242817
Control2=IDC_MPROGRESS,msctls_progress32,1342177281
Control3=IDC_MPERCENT,static,1342308352
Control4=IDC_MoStatus,static,1342308353
Control5=IDC_Monitor,edit,1353779396
Control6=IDC_STATIC,static,1342308354
Control7=IDC_STATIC,static,1342308354
Control8=IDC_nStart,edit,1350631552
Control9=IDC_nEnd,edit,1350631552

[DLG:IDD_DlgPassword]
Type=1
Class=CDlgPassword
ControlCount=4
Control1=IDC_sAnswer,edit,1350631584
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308354

[DLG:IDD_DlgSeisDataProperty]
Type=1
Class=CDlgSeisDataProperty
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ZV_AutoDecideGather,button,1342242819
Control4=IDC_ZV_AutoDecideType,button,1342242819
Control5=IDC_ZV_DataFormat,combobox,1344340035
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308866
Control8=IDC_STATIC,static,1342308353
Control9=IDC_ZV_GroupHeadName,combobox,1344340035
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_ZV_GroupHeadStartByte,combobox,1344340035
Control13=IDC_ZV_GroupHeadOrder,combobox,1344340035
Control14=IDC_STATIC,static,1342308352
Control15=IDC_ZV_SelfGroupStartByte,combobox,1344340035
Control16=IDC_ZV_SelfDataType,combobox,1344340035
Control17=IDC_ZV_SelfGroupHeadByteQuantity,combobox,1344340035
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308353
Control20=IDC_STATIC,static,1342308353
Control21=IDC_STATIC,static,1342308353
Control22=IDC_STATIC,button,1342177543

[DLG:IDD_DlgTreeProperty]
Type=1
Class=CDlgTreeProperty
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308353
Control4=IDC_STATIC,static,1342308353
Control5=IDC_nAreaNumber,edit,1350631552
Control6=IDC_nLineNumber,edit,1350631552
Control7=IDC_STATIC,static,1342308353
Control8=IDC_nTotalLineNumber,edit,1350631552
Control9=IDC_STATIC,static,1342308353
Control10=IDC_sAreaCurrent,edit,1350631552

[DLG:IDD_FHZoomAppearance]
Type=1
Class=CFHZoomApprDlg
ControlCount=47
Control1=IDC_CheckDrawTitle,button,1342242819
Control2=IDC_ButtonTitleFont,button,1342242816
Control3=IDC_EditTitle,edit,1350631552
Control4=IDC_ButtonMapCharFont,button,1342242816
Control5=IDC_ButtonDrawLineColor,button,1342242816
Control6=IDC_ButtonDrawCurrentLineColor,button,1342242816
Control7=IDC_CheckDrawCoor,button,1342242819
Control8=IDC_CheckUseUserDefineCoor,button,1342242819
Control9=IDC_ButtonCoorFont,button,1342242816
Control10=IDC_ButtonCoorThickColorHor,button,1342242816
Control11=IDC_nThickLineStartHor,edit,1350631552
Control12=IDC_nThickLineEndHor,edit,1350631552
Control13=IDC_nThickLineStepHor,edit,1350631552
Control14=IDC_ButtonCoorThinColorHor,button,1342242816
Control15=IDC_nThinLineStartHor,edit,1350631552
Control16=IDC_nThinLineEndHor,edit,1350631552
Control17=IDC_nThinLineStepHor,edit,1350631552
Control18=IDC_ButtonCoorThickColorVer,button,1342242816
Control19=IDC_nThickLineStartVer,edit,1350631552
Control20=IDC_nThickLineEndVer,edit,1350631552
Control21=IDC_nThickLineStepVer,edit,1350631552
Control22=IDC_ButtonCoorThinColorVer,button,1342242816
Control23=IDC_nThinLineStartVer,edit,1350631552
Control24=IDC_nThinLineEndVer,edit,1350631552
Control25=IDC_nThinLineStepVer,edit,1350631552
Control26=IDOK,button,1342242817
Control27=IDCANCEL,button,1342242816
Control28=IDC_STATIC,static,1342308354
Control29=IDC_STATIC,static,1342308354
Control30=IDC_STATIC,static,1342308354
Control31=IDC_STATIC,static,1342308354
Control32=IDC_STATIC,static,1342308354
Control33=IDC_STATIC,button,1342177287
Control34=IDC_STATIC,static,1342308354
Control35=IDC_STATIC,static,1342308354
Control36=IDC_STATIC,button,1342177287
Control37=IDC_STATIC,static,1342308354
Control38=IDC_STATIC,static,1342308352
Control39=IDC_FrameThickLineColorHor,static,1342177287
Control40=IDC_FrameThinLineColorHor,static,1342177287
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC,static,1342308354
Control45=IDC_STATIC,static,1342308354
Control46=IDC_FrameThickLineColorVer,static,1342177287
Control47=IDC_FrameThinLineColorVer,static,1342177287

[DLG:IDD_InfoDlg]
Type=1
Class=CInfoDlg
ControlCount=0

[DLG:IDD_StackSectionPar]
Type=1
Class=CInfoViewPar
ControlCount=29
Control1=IDC_nPixelPerTimePoint,edit,1350631552
Control2=IDC_nPixelPerGroup,edit,1350631552
Control3=IDC_nZoomCoeWave,edit,1350631552
Control4=IDC_nFrameThickTimeInterval,edit,1350631552
Control5=IDC_nFrameThinTimeInterval,edit,1350631552
Control6=IDC_nGroupNumberPerMark,edit,1350631552
Control7=IDC_CheckWave,button,1342242819
Control8=IDC_CheckPositiveArea,button,1342242819
Control9=IDC_CheckNegativeArea,button,1342242819
Control10=IDC_CheckColorBlock,button,1342242819
Control11=IDC_BalanceStyle,button,1342308361
Control12=IDC_BalanceStyle2,button,1342177289
Control13=IDOK,button,1342242817
Control14=IDCANCEL,button,1342242816
Control15=IDC_STATIC,static,1342308354
Control16=IDC_STATIC,static,1342308354
Control17=IDC_STATIC,static,1342308354
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308354
Control20=IDC_STATIC,static,1342308354
Control21=IDC_STATIC,static,1342308354
Control22=IDC_STATIC,static,1342308354
Control23=IDC_STATIC,static,1342308354
Control24=IDC_STATIC,static,1342308354
Control25=IDC_STATIC,static,1342308354
Control26=IDC_STATIC,static,1342308354
Control27=IDC_ButtonColor,button,1342242816
Control28=IDC_ColorBox,static,1342181383
Control29=IDC_STATIC,button,1342177287

[DLG:CG_IDD_PROGRESS]
Type=1
Class=CProgressDlg
ControlCount=4
Control1=IDCANCEL,button,1342242817
Control2=CG_IDC_PROGDLG_PROGRESS,msctls_progress32,1342177281
Control3=CG_IDC_PROGDLG_PERCENT,static,1342308352
Control4=CG_IDC_PROGDLG_STATUS,static,1342308353

[DLG:IDD_DlgSetSeisDataType]
Type=1
Class=?
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308353
Control4=IDC_Check_AutoDecideGather,button,1342242819
Control5=IDC_COMBO_GroupHeadName,combobox,1344340035
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308353
Control8=IDC_COMBO_GroupHeadStartByte,combobox,1344340035
Control9=IDC_COMBO_GroupHeadOrder,combobox,1344340035
Control10=IDC_STATIC,static,1342308352
Control11=IDC_COMBO_SelfGroupStartByte,combobox,1344340035
Control12=IDC_COMBO_SelfDataType,combobox,1344340035
Control13=IDC_COMBO_SelfGroupHeadByteQuantity,combobox,1344340035
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308353
Control16=IDC_STATIC,static,1342308353
Control17=IDC_STATIC,static,1342308353
Control18=IDC_COMBO_DataFormat,combobox,1344340035
Control19=IDC_STATIC,button,1342177543
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308866
Control22=IDC_Check_AutoDecideType,button,1342242819

[MNU:IDR_FHEditViewPopUp]
Type=1
Class=?
Command1=ID_FHEditViewInsRow
Command2=ID_FHEditViewInsCol
Command3=ID_FHEditViewDelRow
Command4=ID_FHEditView_DelCol
Command5=ID_FHEditViewAddRow
Command6=ID_FHEditViewAddCol
Command7=ID_FHEditViewSetSize
Command8=ID_FHEditViewUpdateGrid
CommandCount=8

[MNU:IDR_FHZoomViewPopUp]
Type=1
Class=?
Command1=ID_FHZoomIn
Command2=ID_FHZoomOut
Command3=ID_FHZoomInHor
Command4=ID_FHZoomOutHor
Command5=ID_FHZoomInVer
Command6=ID_FHZoomInVer
Command7=ID_FHZoomFull
Command8=ID_FHSetFont
Command9=ID_FHEnableCor
Command10=ID_FHZoomViewNext
Command11=ID_FHZoomViewLast
CommandCount=11

[MNU:IDR_FHTreeViewPopUp]
Type=1
Class=?
Command1=ID_FHTreeNewDataBase
Command2=ID_FHTreeOpenDataBase
Command3=ID_FHTreeInsertArea
Command4=ID_FHTreeInsertLine
Command5=ID_FHTreeRemoveArea
Command6=ID_FHTreeRemoveLine
Command7=ID_FHTreeDrawLineData
Command8=ID_FHTreeDrawLineMap
Command9=ID_FHTreeDrawAreaMap
Command10=ID_FHTreeNewLineData
Command11=ID_FHTreeShowFullName
Command12=ID_FHTreeShowSortedName
Command13=ID_FHTreeProperty
CommandCount=13

[MNU:IDR_InfoViewPopup]
Type=1
Class=CInfoView
Command1=ID_IV_SetMapShowPar
CommandCount=1

[TB:IDR_FHZoomViewToolBar]
Type=1
Class=?
Command1=ID_FHZoomIn
Command2=ID_FHZoomOut
Command3=ID_FHZoomInArea
Command4=ID_FHZoomInHor
Command5=ID_FHZoomOutHor
Command6=ID_FHZoomInVer
Command7=ID_FHZoomOutVer
Command8=ID_FHZoomFull
Command9=ID_FHEnableInfoWnd
Command10=ID_FHEnableCor
Command11=ID_FHSetFont
Command12=ID_FHZoomErrorMsg
Command13=ID_FHZoomViewLast
Command14=ID_FHZoomViewNext
Command15=ID_FHZoomViewSearch
Command16=ID_FHZoomViewExportBitmap
CommandCount=16

