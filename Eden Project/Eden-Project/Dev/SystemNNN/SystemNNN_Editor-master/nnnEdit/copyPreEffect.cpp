#include <windows.h>
#include <stdio.h>

#include "..\..\systemNNN\nyanlib\include\commonMacro.h"

#include "..\..\systemNNN\nyanEffectLib\effectStruct.h"
#include "..\..\systemNNN\nyanLib\include\effect.h"
#include "..\..\systemNNN\nyanEffectLib\effectlist.h"

#include "..\..\systemNNN\nnnUtilLib\nameList.h"

#include "case.h"
#include "messageData.h"
#include "myApplicationBase.h"

#include "filmCaseData.h"
#include "filmData.h"
#include "komaData.h"
#include "messageData.h"

#include "copyPreEffect.h"

CCopyPreEffect::CCopyPreEffect(CMyApplicationBase* app)
{
	m_app = app;
	m_effect = m_app->GetEffect();
}


CCopyPreEffect::~CCopyPreEffect()
{
	End();
}

void CCopyPreEffect::End(void)
{
}

void CCopyPreEffect::CopyPreEffect(int pre)
{
	CFilmData* pFilm = m_app->GetNowSelectFilm();
	if (pFilm == NULL) return;

	int k = pFilm->GetNowSelectNumber();
//	if (k== 0) return;
	if (k-pre<0) return;

	CKomaData* pKoma = pFilm->GetKoma(k);
	if (pKoma == NULL) return;
	CKomaData* pPre = pFilm->GetKoma(k-pre);
	if (pPre == NULL) return;

	for (int i=0;i<16;i++)
	{
		if (pPre->GetEffectFlag(i))
		{
			int preEffect = pPre->GetEffect(i);

			//if (preEffect != EFFECT_BUFFER)
			if (1)
			{
				int src = i;
				int dst = i;
				
				if (preEffect != -1)
				{
					int pn = m_effect->GetEffectEndIsChangeLayer(preEffect);
					if (pn != -1)
					{
						src = pPre->GetEffectPara(i,pn);
					}
				}


				pKoma->SetEffectFlag(dst);

				int pic = pPre->GetPictureNumber(src);
				if (pic != -1)
				{
					LPSTR filename = pPre->GetPicFileName(pic);
					pKoma->SetPicFileName(dst,filename);
				}

				pKoma->SetEffect(dst,preEffect);

				int paraKosuu = pPre->GetEffectParaKosuu(i);	//not src
				pKoma->SetEffectParaKosuu(dst,paraKosuu);

				int j;
				for (j=0;j<paraKosuu;j++)
				{
					int p = pPre->GetEffectPara(i,j);
					pKoma->SetEffectPara(dst,j,p);
				}


				int bt = 0x1;
				RECT rc;
				for (j=0;j<6;j++)
				{
					pPre->GetEffectRect(&rc,i,j);
					pKoma->SetEffectRect(&rc,dst,bt);
					bt <<= 1;
				}

				if (m_app->GetEffectCopyMoveFlag())
				{
					pKoma->GetEffectRect(&rc,dst,2);
					pKoma->SetEffectRect(&rc,dst,0x00000003);

					pKoma->GetEffectRect(&rc,dst,5);
					pKoma->SetEffectRect(&rc,dst,0x00000018);
				}
			}
		}
	}		

	m_app->LayerIsChanged();
}

/*_*/

