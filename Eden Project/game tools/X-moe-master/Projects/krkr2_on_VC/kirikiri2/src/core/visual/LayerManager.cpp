//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Layer Management
//---------------------------------------------------------------------------

#include "tjsCommHead.h"

#include "tjsArray.h"
#include "LayerManager.h"
#include "MsgIntf.h"
#include "LayerBitmapIntf.h"
#include "WindowIntf.h"
#include "StorageIntf.h"
#include "EventIntf.h"
#include "SysInitIntf.h"
#include "TickCount.h"
#include "DebugIntf.h"




//---------------------------------------------------------------------------
// tTVPLayerManager
//---------------------------------------------------------------------------
tTVPLayerManager::tTVPLayerManager(tTJSNI_BaseWindow *window)
{
	RefCount = 1;
	Window = window;
	DrawDeviceData = NULL;
	DrawBuffer = NULL;
	DesiredLayerType = ltOpaque;

	CaptureOwner = NULL;
	LastMouseMoveSent = NULL;
	Primary = NULL;
	FocusedLayer = NULL;
	OverallOrderIndexValid = false;
	EnabledWorkRefCount = 0;
	FocusChangeLock = false;
	VisualStateChanged = true;
	LastMouseMoveX = -1;
	LastMouseMoveY = -1;
	InNotifyingHintOrCursorChange = false;
}
//---------------------------------------------------------------------------
tTVPLayerManager::~tTVPLayerManager()
{
	if(DrawBuffer) delete DrawBuffer;
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::AddRef()
{
	RefCount ++;
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::Release()
{
	if(RefCount == 1)
		delete this;
	else
		RefCount --;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::RegisterSelfToWindow()
{
	Window->RegisterLayerManager(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::UnregisterSelfFromWindow()
{
	Window->UnregisterLayerManager(this);
}
//---------------------------------------------------------------------------
tTVPBaseBitmap * tTVPLayerManager::GetDrawTargetBitmap(const tTVPRect &rect,
	tTVPRect &cliprect)
{
	// retrieve draw target bitmap
	tjs_int w = rect.get_width();
	tjs_int h = rect.get_height();

	if(!DrawBuffer)
	{
		// create draw buffer
		DrawBuffer = new tTVPBaseBitmap(w, h, 32);
	}
	else
	{
		tjs_int bw = DrawBuffer->GetWidth();
		tjs_int bh = DrawBuffer->GetHeight();
		if(bw < w || bh  < h)
		{
			// insufficient size; resize the draw buffer
			tjs_uint neww = bw > w ? bw:w;
			neww += (neww & 1); // align to even
			DrawBuffer->SetSize(neww, bh > h ? bh:h);
		}
	}

	cliprect.left = 0;
	cliprect.top = 0;
	cliprect.right = w;
	cliprect.bottom = h;
	return DrawBuffer;
}
//---------------------------------------------------------------------------
tTVPLayerType tTVPLayerManager::GetTargetLayerType()
{
	return DesiredLayerType;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::DrawCompleted(const tTVPRect &destrect,
		tTVPBaseBitmap *bmp, const tTVPRect &cliprect,
		tTVPLayerType type, tjs_int opacity)
{
	// TODO: cross platform
	Window->GetDrawDevice()->NotifyBitmapCompleted(this, destrect.left, destrect.top,
		bmp->GetBitmap()->GetBits(), bmp->GetBitmap()->GetBITMAPINFO(), cliprect, type, opacity);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::AttachPrimary(tTJSNI_BaseLayer *pri)
{
	// attach primary layer to the manager
	DetachPrimary();

	if(!Primary)
	{
		Primary = pri;
		EnabledWorkRefCount = 0;
		OverallOrderIndexValid = false;
		UpdateRegion.Clear();
		pri->SetVisible(true);
		pri->SetOpacity(255);
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::DetachPrimary()
{
	// detach primary layer from the manager
	if(Primary)
	{
		SetFocusTo(NULL);
		ReleaseCapture();
		ForceMouseLeave();
		NotifyPart(Primary);
		Primary = NULL;
	}
}
//---------------------------------------------------------------------------
bool TJS_INTF_METHOD tTVPLayerManager::GetPrimaryLayerSize(tjs_int &w, tjs_int &h) const
{
	if(IsPrimaryLayerAttached())
	{
		w = Primary->GetWidth();
		h = Primary->GetHeight();
		return true;
	}
	else
	{
		return false;
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyPart(tTJSNI_BaseLayer *lay)
{
	// notifies layer parting from its parent
	InvalidateOverallIndex();
	BlurTree(lay);
	ReleaseCaptureFromTree(lay);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::InvalidateOverallIndex()
{
	OverallOrderIndexValid = false;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::RecreateOverallOrderIndex()
{
	// recreate overall order index
	if(!OverallOrderIndexValid)
	{
		tjs_uint index = 0;
		AllNodes.clear();
		if(Primary) Primary->RecreateOverallOrderIndex(index, AllNodes);

		OverallOrderIndexValid = true;
	}
}
//---------------------------------------------------------------------------
std::vector<tTJSNI_BaseLayer*> & tTVPLayerManager::GetAllNodes()
{
	if(!OverallOrderIndexValid) RecreateOverallOrderIndex();
	return AllNodes;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::QueryUpdateExcludeRect()
{
	if(!VisualStateChanged) return;
	tTVPRect r;
	r.clear();
	if(Primary) Primary->QueryUpdateExcludeRect(r, true);
	VisualStateChanged = false;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyMouseCursorChange(
	tTJSNI_BaseLayer *layer, tjs_int cursor)
{
	if(InNotifyingHintOrCursorChange) return;

	InNotifyingHintOrCursorChange = true;
	try
	{

		tTJSNI_BaseLayer *l;

		if(CaptureOwner)
			l = CaptureOwner;
		else
			l = GetMostFrontChildAt(LastMouseMoveX, LastMouseMoveY);

		if(l == layer) SetMouseCursor(cursor);
	}
	catch(...)
	{
		InNotifyingHintOrCursorChange = false;
		throw;
	}

	InNotifyingHintOrCursorChange = false;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetMouseCursor(tjs_int cursor)
{
	if(!Window) return;

	if(cursor == 0)
		Window->GetDrawDevice()->SetDefaultMouseCursor(this);
	else
		Window->GetDrawDevice()->SetMouseCursor(this, cursor);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::GetCursorPos(tjs_int &x, tjs_int &y)
{
	if(!Window) return;
	Window->GetDrawDevice()->GetCursorPos(this, x, y);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetCursorPos(tjs_int x, tjs_int y)
{
	if(!Window) return;
	Window->GetDrawDevice()->SetCursorPos(this, x, y);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyHintChange(tTJSNI_BaseLayer *layer, const ttstr & hint)
{
	if(InNotifyingHintOrCursorChange) return;

	InNotifyingHintOrCursorChange = true;

	try
	{
		tTJSNI_BaseLayer *l;

		if(CaptureOwner)
			l = CaptureOwner;
		else
			l = GetMostFrontChildAt(LastMouseMoveX, LastMouseMoveY);

		if(l == layer) SetHint(hint);
	}
	catch(...)
	{
		InNotifyingHintOrCursorChange = false;
		throw;
	}

	InNotifyingHintOrCursorChange = false;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetHint(const ttstr &hint)
{
	if(!Window) return;
	Window->GetDrawDevice()->SetHintText(this, hint);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyLayerResize()
{
	// notifies layer resizing to the window
	if(!Window) return;

	Window->GetDrawDevice()->NotifyLayerResize(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyWindowInvalidation()
{
	// notifies layer surface is invalidated and should be transfered to window.
	if(!Window) return;

	iTVPDrawDevice * DrawDevice = Window->GetDrawDevice();
	if ( DrawDevice )
		DrawDevice->NotifyLayerImageChange(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetWindow(tTJSNI_BaseWindow *window)
{
	// sets window
	Window = window;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyResizeFromWindow(tjs_uint w, tjs_uint h)
{
	// is called by the owner window, notifies windows's client area size
	// has been changed.
	// does not be called if owner window's "autoResize" property is false.

	// currently this function is not used.

	if(Primary) Primary->InternalSetSize(w, h);
}
//---------------------------------------------------------------------------
tTJSNI_BaseLayer * tTVPLayerManager::GetMostFrontChildAt(tjs_int x, tjs_int y,
	tTJSNI_BaseLayer *except, bool get_disabled)
{
	// return most front layer at given point.
	// this does checking of layer's visibility.
	// x and y are given in primary layer's coordinates.
	if(!Primary) return NULL;

	tTJSNI_BaseLayer *lay = NULL;
	Primary->GetMostFrontChildAt(x, y, &lay, except, get_disabled);
	return lay;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryClick(tjs_int x, tjs_int y)
{
	tTJSNI_BaseLayer * l = GetMostFrontChildAt(x, y);
	if(l && CaptureOwner == l)
	{
		l->FromPrimaryCoordinates(x, y);
		l->FireClick(x, y);
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryDoubleClick(tjs_int x, tjs_int y)
{
	tTJSNI_BaseLayer * l = GetMostFrontChildAt(x, y);
	if(l /*&& CaptureOwner == l*/)
	{
		l->FromPrimaryCoordinates(x, y);
		l->FireDoubleClick(x, y);
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryMouseDown(tjs_int x, tjs_int y, tTVPMouseButton mb,
	tjs_uint32 flags)
{
	tTJSNI_BaseLayer * l = CaptureOwner ? CaptureOwner : GetMostFrontChildAt(x, y);
	if(l)
	{
		l->FromPrimaryCoordinates(x, y);
		ReleaseCaptureCalled = false;
		l->FireMouseDown(x, y, mb, flags);
		bool no_capture = ReleaseCaptureCalled;

		if(CaptureOwner != l)
		{
			ReleaseCapture();

			if(!no_capture)
			{
				CaptureOwner = l;
				if(CaptureOwner->Owner) CaptureOwner->Owner->AddRef(); // addref TJS object
			}
		}

		SetHint(ttstr());
	}
	else
	{
		ReleaseCapture();
	}

}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryMouseUp(tjs_int x, tjs_int y, tTVPMouseButton mb,
	tjs_uint32 flags)
{
	tTJSNI_BaseLayer *l;

	if(CaptureOwner)
		l = CaptureOwner;
	else
		l = GetMostFrontChildAt(x, y);

	if(l)
	{
		int orig_x = x, orig_y = y;

		l->FromPrimaryCoordinates(x, y);
		l->FireMouseUp(x, y, mb, flags);

		if(!TVPIsAnyMouseButtonPressedInShiftStateFlags(flags))
		{
			ReleaseCapture();
			PrimaryMouseMove(orig_x, orig_y, flags); // force recheck current under-cursor layer
		}
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryMouseMove(tjs_int x, tjs_int y, tjs_uint32 flags)
{
	bool poschanged = (LastMouseMoveX != x || LastMouseMoveY != y);
	LastMouseMoveX = x;
	LastMouseMoveY = y;

	tTJSNI_BaseLayer *l;

	if(CaptureOwner)
		l = CaptureOwner;
	else
		l = GetMostFrontChildAt(x, y);

	// enter/leave event
	if(LastMouseMoveSent != l)
	{
		if(LastMouseMoveSent) LastMouseMoveSent->FireMouseLeave();

		// recheck l because the layer may become invalid during
		// FireMouseLeave call.
		if(CaptureOwner)
			l = CaptureOwner;
		else
			l = GetMostFrontChildAt(x, y);

		if(l)
		{
			InNotifyingHintOrCursorChange = true;
			try
			{
				tTJSNI_BaseLayer *ll;
				do
				{
					l->FireMouseEnter();

					// recheck l because the layer may become invalid during
					// FireMouseEnter call.
					if(CaptureOwner)
						ll = CaptureOwner;
					else
						ll = GetMostFrontChildAt(x, y);

					if(l == ll) break;
					l = ll;
				} while(true);

				l->SetCurrentCursorToWindow();
				l->SetCurrentHintToWindow();
			}
			catch(...)
			{
				InNotifyingHintOrCursorChange = false;
				throw;
			}
			InNotifyingHintOrCursorChange = false;
		}
		else
		{
			SetMouseCursor(0);
			SetHint(ttstr());
		}
	}

	if(LastMouseMoveSent != l)
	{
		if(LastMouseMoveSent)
		{
			tTJSNI_BaseLayer *lay = LastMouseMoveSent;
			LastMouseMoveSent = NULL;
			if(lay->Owner) lay->Owner->Release();
		}

		LastMouseMoveSent = l;


		if(LastMouseMoveSent)
		{
			if(LastMouseMoveSent->Owner) LastMouseMoveSent->Owner->AddRef();
		}
	}

	if(l)
	{
		if(poschanged)
		{
			l->FromPrimaryCoordinates(x, y);
			l->FireMouseMove(x, y, flags);
		}
	}
	else
	{
		// no layer to send the event
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::ForceMouseLeave()
{
	if(LastMouseMoveSent)
	{
		tTJSNI_BaseLayer *lay = LastMouseMoveSent;
		LastMouseMoveSent = NULL;
		lay->FireMouseLeave();
		if(lay->Owner) lay->Owner->Release();
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::ForceMouseRecheck()
{
	PrimaryMouseMove(LastMouseMoveX, LastMouseMoveY, 0);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::MouseOutOfWindow()
{
	// notifys that the mouse cursor goes outside of the window.
	if(!CaptureOwner)
		PrimaryMouseMove(-1, -1, 0); // force mouse cursor out of the all
}
//---------------------------------------------------------------------------
void tTVPLayerManager::LeaveMouseFromTree(tTJSNI_BaseLayer *root)
{
	// force to leave mouse
	if(LastMouseMoveSent)
	{
		if(LastMouseMoveSent->IsAncestorOrSelf(root))
		{
			tTJSNI_BaseLayer *lay = LastMouseMoveSent;
			LastMouseMoveSent = NULL;
			lay->FireMouseLeave();
			if(lay->Owner) lay->Owner->Release();
		}
	}
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::ReleaseCapture()
{
	// release capture state
	ReleaseCaptureCalled = true;
	if(CaptureOwner)
	{
		tTJSNI_BaseLayer *lay = CaptureOwner;
		CaptureOwner = NULL;
		if(lay->Owner) lay->Owner->Release();
			// release TJS object
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::ReleaseCaptureFromTree(tTJSNI_BaseLayer * layer)
{
	// Release capture state, if the capture object is descendant of
	// 'layer' or 'layer' itself.
	if(CaptureOwner)
	{
		if(CaptureOwner->IsAncestorOrSelf(layer))
		{
			ReleaseCapture();
		}
	}
}
//---------------------------------------------------------------------------
bool tTVPLayerManager::BlurTree(tTJSNI_BaseLayer *root)
{
	// (primary only) remove focus from "root"
	RemoveTreeModalState(root);
	LeaveMouseFromTree(root);

	if(!FocusedLayer) return false;

	if(!FocusedLayer->IsAncestorOrSelf(root)) return false;
		// root is not ancestor of current focused layer

	tTJSNI_BaseLayer *next = root->GetNextFocusable();

	if(next != FocusedLayer)
		SetFocusTo(next, true); // focus to root's next focusable layer
	else
		SetFocusTo(NULL, true);

	return true;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::CheckTreeFocusableState(tTJSNI_BaseLayer *root)
{
	// (primary only) check newly added tree's focusable state
/*	// uncomment here to auto-focus
	if(FocusedLayer) return;

	tTJSNI_BaseLayer *lay = root->SearchFirstFocusable(true);
	if(lay) SetFocusTo(lay, true);
*/
}
//---------------------------------------------------------------------------
tTJSNI_BaseLayer *tTVPLayerManager::FocusPrev()
{
	// focus to previous layer
	tTJSNI_BaseLayer *l;
	if(!FocusedLayer)
		l = SearchFirstFocusable(false);// search first focusable layer
	else
		l = FocusedLayer->GetPrevFocusable();

	if(l) SetFocusTo(l, false);
	return l;
}
//---------------------------------------------------------------------------
tTJSNI_BaseLayer *tTVPLayerManager::FocusNext()
{
	// focus to next layer
	tTJSNI_BaseLayer *l;
	if(!FocusedLayer)
		l = SearchFirstFocusable(false);// search first focusable layer
	else
		l = FocusedLayer->GetNextFocusable();

	if(l) SetFocusTo(l, true);
	return l;
}
//---------------------------------------------------------------------------
tTJSNI_BaseLayer *tTVPLayerManager::SearchFirstFocusable(bool ignore_chain_focusable)
{
	// (primary only) search first focusable layer
	if(!Primary) return NULL;
	tTJSNI_BaseLayer *lay = Primary->SearchFirstFocusable(ignore_chain_focusable);

	return lay;
}
//---------------------------------------------------------------------------
bool tTVPLayerManager::SetFocusTo(tTJSNI_BaseLayer *layer, bool direction)
{
	// set focus to layer

	// direction = true : forward focus
	// direction = false: backward focus

	if(layer && !layer->GetNodeFocusable()) return false;

	if(layer && !layer->Shutdown)
		layer = layer->FireBeforeFocus(FocusedLayer, direction);

	if(layer && !layer->GetNodeFocusable()) return false;

	if(FocusedLayer == layer) return false;


	if(FocusChangeLock)
		TVPThrowExceptionMessage(TVPCannotChangeFocusInProcessingFocus);
	FocusChangeLock = true;

	tTJSNI_BaseLayer *org = FocusedLayer;
	FocusedLayer = layer;

	try
	{
		if(org && !org->Shutdown)
			org->FireBlur(layer);

		if(FocusedLayer && !FocusedLayer->Shutdown)
			FocusedLayer->FireFocus(org, direction);
	}
	catch(...)
	{
		if(FocusedLayer) if(FocusedLayer->Owner)
			FocusedLayer->Owner->AddRef();
		if(org) if(org->Owner)
			org->Owner->Release();
		FocusChangeLock = false;
		throw;
	}

	if(FocusedLayer) if(FocusedLayer->Owner)
		FocusedLayer->Owner->AddRef();
	if(org) if(org->Owner)
		org->Owner->Release();

	if(FocusedLayer) SetImeModeOf(FocusedLayer); else ResetImeMode();
	if(FocusedLayer) SetAttentionPointOf(FocusedLayer); else DisableAttentionPoint();

	FocusChangeLock = false;
	return true;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::ReleaseAllModalLayer()
{
	// (primary only) release all modal layer on invalidation
	std::vector<tTJSNI_BaseLayer*> copy(ModalLayerVector);
	ModalLayerVector.clear();

	std::vector<tTJSNI_BaseLayer*>::iterator i;
	for(i = copy.begin(); i < copy.end(); i++)
	{
		if((*i)->Owner) (*i)->Owner->Release();
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetModeTo(tTJSNI_BaseLayer *layer)
{
	// (primary only) set mode to layer
	if(!layer) return;

	SaveEnabledWork();

	try
	{
		tTJSNI_BaseLayer *current = GetCurrentModalLayer();
		if(current && layer->IsAncestorOrSelf(current))
			TVPThrowExceptionMessage(TVPCannotSetModeToDisabledOrModal);
				// cannot set mode to parent layer
		if(!layer->Visible) layer->Visible = true;
		if(!layer->GetParentVisible() || !layer->Enabled)
			TVPThrowExceptionMessage(TVPCannotSetModeToDisabledOrModal);
				// cannot set mode to parent layer
		if(layer == current)
			TVPThrowExceptionMessage(TVPCannotSetModeToDisabledOrModal);
				// cannot set mode to already modal layer

		SetFocusTo(layer->SearchFirstFocusable(), true);

		if(layer->Owner) layer->Owner->AddRef();
		ModalLayerVector.push_back(layer);

	}
	catch(...)
	{
		NotifyNodeEnabledState();
		throw;
	}

	NotifyNodeEnabledState();
}
//---------------------------------------------------------------------------
void tTVPLayerManager::RemoveModeFrom(tTJSNI_BaseLayer *layer)
{
	// remove modal state from given layer
	SaveEnabledWork();

	try
	{
		std::vector<tTJSNI_BaseLayer*>::iterator i;
		for(i = ModalLayerVector.begin(); i < ModalLayerVector.end();)
		{
			if(layer == *i)
			{
				if(layer->Owner) layer->Owner->Release();
				SetFocusTo(layer->GetNextFocusable(), true);
				i = ModalLayerVector.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
	catch(...)
	{
		NotifyNodeEnabledState();
		throw;
	}

	NotifyNodeEnabledState();
}
//---------------------------------------------------------------------------
void tTVPLayerManager::RemoveTreeModalState(tTJSNI_BaseLayer *root)
{
	// remove modal state from given tree
	SaveEnabledWork();

	try
	{
		std::vector<tTJSNI_BaseLayer*>::iterator i;
		for(i = ModalLayerVector.begin(); i < ModalLayerVector.end();)
		{
			if((*i)->IsAncestorOrSelf(root))
			{
				if((*i)->Owner) (*i)->Owner->Release();
				SetFocusTo(root->GetNextFocusable(), true);
				i = ModalLayerVector.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
	catch(...)
	{
		NotifyNodeEnabledState();
		throw;
	}

	NotifyNodeEnabledState();
}
//---------------------------------------------------------------------------
tTJSNI_BaseLayer *tTVPLayerManager::GetCurrentModalLayer() const
{
	// (primary only) get current modal layer
	tjs_uint size = ModalLayerVector.size();
	if(size == 0) return NULL;
	return *(ModalLayerVector.begin() + size - 1);
}
//---------------------------------------------------------------------------
bool tTVPLayerManager::SearchAttentionPoint(tTJSNI_BaseLayer *target,
	tjs_int &x, tjs_int &y)
{
	// search specified layer 's attention point
	while(target)
	{
		if(target->UseAttention)
		{
			x = target->AttentionLeft, y = target->AttentionTop;
			target->ToPrimaryCoordinates(x, y);
			return true;
		}
		target = target->Parent;
	}
	return false;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetAttentionPointOf(tTJSNI_BaseLayer *layer)
{
	if(!Window) return;
	tjs_int x, y;
	if(SearchAttentionPoint(layer, x, y))
		Window->GetDrawDevice()->SetAttentionPoint(this, layer, x, y);
	else
		Window->GetDrawDevice()->DisableAttentionPoint(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::DisableAttentionPoint()
{
	if(Window) Window->GetDrawDevice()->DisableAttentionPoint(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyAttentionStateChanged(tTJSNI_BaseLayer *from)
{
	if(FocusedLayer == from)
	{
		SetAttentionPointOf(from);
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SetImeModeOf(tTJSNI_BaseLayer *layer)
{
	if(!Window) return;
	Window->GetDrawDevice()->SetImeMode(this, layer->ImeMode);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::ResetImeMode()
{
	Window->GetDrawDevice()->ResetImeMode(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyImeModeChanged(tTJSNI_BaseLayer *from)
{
	if(FocusedLayer == from)
	{
		SetImeModeOf(from);
	}
}
//---------------------------------------------------------------------------
void tTVPLayerManager::SaveEnabledWork()
{
	// save current node enabled state to EnabledWork
	// this does recursive call
	if(EnabledWorkRefCount == 0) if(Primary) Primary->SaveEnabledWork();

	EnabledWorkRefCount ++;
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyNodeEnabledState()
{
	// notify node enabled state change to self and its children
	// this refers EnabledWork which is created by SaveEnabledWork
	EnabledWorkRefCount --;

	if(EnabledWorkRefCount == 0) if(Primary) Primary->NotifyNodeEnabledState();
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryKeyDown(tjs_uint key, tjs_uint32 shift)
{
	if(FocusedLayer)
		FocusedLayer->FireKeyDown(key, shift);
	else
		if(Primary) Primary->DefaultKeyDown(key, shift);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryKeyUp(tjs_uint key, tjs_uint32 shift)
{
	if(FocusedLayer)
		FocusedLayer->FireKeyUp(key, shift);
	else
		if(Primary) Primary->DefaultKeyUp(key, shift);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryKeyPress(tjs_char key)
{
	if(FocusedLayer)
		FocusedLayer->FireKeyPress(key);
	else
		if(Primary) Primary->DefaultKeyPress(key);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::PrimaryMouseWheel(tjs_uint32 shift, tjs_int delta,
	tjs_int x, tjs_int y)
{
	if(FocusedLayer)
		FocusedLayer->FireMouseWheel(shift, delta, x, y);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::AddUpdateRegion(const tTVPComplexRect &rects)
{
	UpdateRegion.Or(rects);
	if(UpdateRegion.GetCount() > TVP_UPDATE_UNITE_LIMIT)
		UpdateRegion.Unite();
	NotifyWindowInvalidation();
}
//---------------------------------------------------------------------------
void tTVPLayerManager::AddUpdateRegion(const tTVPRect &rect)
{
	// the window is invalidated;
	UpdateRegion.Or(rect);
	NotifyWindowInvalidation();
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::UpdateToDrawDevice()
{
	// drawdevice -> layer
	if(!Primary) return;

	Window->GetDrawDevice()->StartBitmapCompletion(this);
	try
	{
		Primary->CompleteForWindow(this);
	}
	catch(...)
	{
		Window->GetDrawDevice()->EndBitmapCompletion(this);
		throw;
	}
	Window->GetDrawDevice()->EndBitmapCompletion(this);
}
//---------------------------------------------------------------------------
void tTVPLayerManager::NotifyUpdateRegionFixed()
{
	// called by primary layer, notifying final update region is fixed
//	Window->NotifyUpdateRegionFixed(UpdateRegion);
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::RequestInvalidation(const tTVPRect &r)
{
	// called by the owner window to notify window surface is invalidated by
	// the system or user.
	if(!Primary) return;

	tTVPRect ur;
	tTVPRect cr(0, 0, Primary->Rect.get_width(), Primary->Rect.get_height());

	if(TVPIntersectRect(&ur, r, cr))
	{
		AddUpdateRegion(ur);
	}
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::RecheckInputState()
{
	// To re-check current layer under current mouse position
	// and update hint, cursor type and process layer enter/leave.
	// This can be reasonably slow, about 1 sec interval.
	ForceMouseRecheck();
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPLayerManager::DumpLayerStructure()
{
	if(Primary) Primary->DumpStructure();
}
//---------------------------------------------------------------------------


