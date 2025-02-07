/*++

Copyright (c) 2012      Microsoft Corporation

Module Name:

        BTHGUID.H

Abstract:

   Public Interface and Property definitions for Bluetooth drivers

Environment:

    Kernel & user mode

Revision History:

  --*/

#ifndef __BTHGUID_H__
#define __BTHGUID_H__

#if (NTDDI_VERSION >= NTDDI_VISTA)

// 81A7FDF3-86C1-4BE8-A8C8-2A6D188B4177 
DEFINE_GUID(GUID_BTHDDI_SDP_NODE_INTERFACE, 0x81a7fdf3, 0x86c1, 0x4be8, 0xa8, 0xc8, 0x2a, 0x6d, 0x18, 0x8b, 0x41, 0x77);

// 4E719439-9CF1-4BAB-AC1D-3279865743D2 
DEFINE_GUID(GUID_BTHDDI_SDP_PARSE_INTERFACE, 0x4e719439, 0x9cf1, 0x4bab, 0xac, 0x1d, 0x32, 0x79, 0x86, 0x57, 0x43, 0xd2);

// {94A59AA8-4383-4286-AA4F-34A160F40004}
DEFINE_GUID(GUID_BTHDDI_PROFILE_DRIVER_INTERFACE, 0x94a59aa8, 0x4383, 0x4286, 0xaa, 0x4f, 0x34, 0xa1, 0x60, 0xf4, 0x0, 0x4);

#endif // (NTDDI_VERSION >= NTDDI_VISTA)

//
// Bluetooth device interface GUID
//
// 00F40965-E89D-4487-9890-87C3ABB211F4
DEFINE_GUID(GUID_BTH_DEVICE_INTERFACE, 0x00F40965,0xE89D, 0x4487, 0x98, 0x90, 0x87, 0xC3, 0xAB, 0xB2, 0x11, 0xF4);

#include <devpropdef.h>

//
// Device properties, these properties are stored in bluetooth device objects for both BR/EDR and LE devices.
//  {2bd67d8b-8beb-48d5-87e0-6cda3428040a}
//
DEFINE_DEVPROPKEY(DEVPKEY_Bluetooth_DeviceAddress,0x2bd67d8b, 0x8beb, 0x48d5, 0x87, 0xe0, 0x6c, 0xda, 0x34, 0x28, 0x04, 0x0a, 1);   // DEVPROP_TYPE_STRING
DEFINE_DEVPROPKEY(DEVPKEY_Bluetooth_ServiceGUID,0x2bd67d8b, 0x8beb, 0x48d5, 0x87, 0xe0, 0x6c, 0xda, 0x34, 0x28, 0x04, 0x0a, 2);     // DEVPROP_TYPE_GUID

#endif // __BTHGUID_H__
