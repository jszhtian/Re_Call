
//
//  myapplication.cpp
//




#include <windows.h>
#include <stdio.h>
#include <commctrl.h>

#include "resource.h"

#include "..\..\systemNNN\nyanLib\include\commonMacro.h"
#include "..\..\systemNNN\nyanLib\include\areaControl.h"
#include "..\..\systemNNN\nyanlib\include\myFile.h"

#include "..\..\systemNNN\nyanlib\include\mygraphics.h"
#include "..\..\systemNNN\nyanlib\include\picture.h"
#include "..\..\systemNNN\nyanLib\include\allGeo.h"

#include "..\..\systemNNN\nyanPictureLib\jpegDecoder.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"

#include "..\..\systemNNN\nyanDirectXLib\myDirectSound.h"
#include "..\..\systemNNN\nyanDirectXLib\myDirectSoundBuffer.h"
#include "..\..\systemNNN\nnnUtilLib\waveData.h"

#include "..\..\systemNNN\nnnUtilLib\waveMusic.h"

#include "..\..\systemNNN\nnnUtilLib\mmlControl.h"
#include "..\..\systemNNN\nnnUtilLib\musicControl.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"
#include "..\..\systemNNN\nnnUtilLib\cutinControl.h"

#include "..\..\systemNNN\nnnUtilLib\cutinNameData.h"

#include "..\..\systemNNN\nyanLib\include\effect.h"
//#include "..\cyclib\cyclib.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"
#include "..\..\systemNNN\nnnUtilLib\scriptDefine.h"

#include "..\..\systemNNN\nnnUtilLib\shakin.h"

#include "define.h"


#include "bitmapnumber.h"

#include "commanddatatype.h"

#include "windowlist.h"

#include "myBitmap.h"
//#include "systemdib.h"


#include "effectParam.h"
#include "allEffectParam.h"

#include "configData.h"
#include "myConfig.h"

#include "case.h"



#include "messageSetup.h"


#include "myframewindow.h"
#include "myclientwindow.h"
#include "mydocument.h"

#include "mainscreendoc.h"
#include "filmdoc.h"
#include "layerdoc.h"
#include "zahyodoc.h"
#include "gamemessagedoc.h"
#include "effectdoc.h"
#include "storydoc.h"
#include "controldoc.h"
#include "filmcasedoc.h"
#include "vardoc.h"
#include "programdoc.h"
#include "storybookdoc.h"
#include "conteDoc.h"
#include "conteMessageDoc.h"


#include "starttitle.h"


#include "myview.h"

#include "selectDialog.h"
#include "myapplicationBase.h"



#include "filmdata.h"
#include "storydata.h"
#include "komadata.h"
#include "messagedata.h"

#include "storybookdata.h"
#include "filmcasedata.h"

#include "commanddata.h"

#include "filmcasedata.h"

//#include "configparamlist.h"

#include "myfileopen.h"

#include "scriptdata.h"

#include "..\..\systemNNN\nnnUtilLib\myfont.h"
#include "..\..\systemNNN\nnnUtilLib\rubiFont.h"
#include "..\..\systemNNN\nnnUtilLib\mymessage.h"

#include "commonDIB.h"

//#include "..\cycGameLib\namelist.h"

#include "myinputdialog.h"
#include "mySearchDialog.h"

//#include "minicompiler.h"

////@@@#include "wave.h"

#include "selectobjectonly.h"

//@@@#include "config.h"

#include "buttonpic.h"
#include "effectpic.h"

//#include "aviSave.h"

#include "defaultControl.h"
#include "volumeControl.h"

#include "makeUseVawBat.h"
#include "makeCheckVawBat.h"
#include "makeUseDWQBat.h"
#include "makeCheckDWQBat.h"

#include "scriptSoundControl.h"
#include "scriptVoiceControl.h"

#include "menuCheckControl.h"
#include "movieCreate.h"

#include "messageSerialControl.h"
#include "scriptCompiler.h"

#include "autoVoice.h"
#include "lockVoice.h"
#include "deleteVoice.h"
#include "makeBook.h"

#include "sameMessageDeutch.h"

#include "searchorReplace.h"

#include "varInitData.h"

#include "windowUpdateControl.h"

#include "copyPreEffect.h"
#include "checkDWQ.h"
#include "deleteNonVoice.h"

#include "changeBGMList.h"
#include "changeSeList.h"

#include "myOntimer.h"

#include "frameChange.h"

#include "rubiMaker.h"


CMyApplicationBase::CMyApplicationBase(HINSTANCE hinstance)
{
}


CMyApplicationBase::~CMyApplicationBase()
{
	End();
}

void CMyApplicationBase::End(void)
{
}


/*_*/

