#include "StreamDecoder.h"
#include <windows.h>
#include "cxdec.h"
#include "tp_stub.h"

//#pragma comment(linker, "/align:512")
#ifndef _DEBUG
//#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.rdata=.text")
#endif

#define ONI_GOKKOKEY1 0x161
#define ONI_GOKKOKEY2 0x5c9

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

const unsigned char EncryptionControlBlock[4096] = {
	0x20, 0x45, 0x6E, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x63, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x20, 0x62, 0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x2D, 0x2D, 0x20, 0x53, 0x74, 0x61,
	0x74, 0x69, 0x63, 0x61, 0x6C, 0x6C, 0x79, 0x20, 0x6F, 0x72, 0x20, 0x64, 0x79, 0x6E, 0x61, 0x6D, 0x69, 0x63, 0x61, 0x6C, 0x6C, 0x79, 0x2C, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6C, 0x79,
	0x20, 0x6F, 0x72, 0x20, 0x69, 0x6E, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6C, 0x79, 0x2C, 0x20, 0x75, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72,
	0x61, 0x6D, 0x20, 0x61, 0x6E, 0x64, 0x2F, 0x6F, 0x72, 0x20, 0x62, 0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x66, 0x72, 0x6F, 0x6D, 0x20, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72,
	0x61, 0x6D, 0x73, 0x20, 0x77, 0x69, 0x6C, 0x6C, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6C, 0x6C, 0x65, 0x67, 0x61, 0x6C, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6C, 0x69, 0x63, 0x65, 0x6E,
	0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6D, 0x65, 0x6E, 0x74, 0x2E, 0x20, 0x82, 0xB1, 0x82, 0xCC, 0x83, 0x76, 0x83, 0x8D, 0x83, 0x4F, 0x83, 0x89, 0x83, 0x80, 0x82, 0xE2, 0x83, 0x75,
	0x83, 0x8D, 0x83, 0x62, 0x83, 0x4E, 0x82, 0xF0, 0x81, 0x41, 0x90, 0xC3, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x93, 0xAE, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x81, 0x41,
	0x92, 0xBC, 0x90, 0xDA, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x8A, 0xD4, 0x90, 0xDA, 0x93, 0x49, 0x82, 0xC5, 0x82, 0xA0, 0x82, 0xEA, 0x81, 0x41, 0x91, 0xBC, 0x82, 0xCC, 0x83, 0x76,
	0x83, 0x8D, 0x83, 0x4F, 0x83, 0x89, 0x83, 0x80, 0x82, 0xA9, 0x82, 0xE7, 0x97, 0x70, 0x82, 0xA2, 0x82, 0xE9, 0x82, 0xB1, 0x82, 0xC6, 0x82, 0xCD, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5A, 0x83, 0x93,
	0x83, 0x58, 0x82, 0xC9, 0x82, 0xE6, 0x82, 0xE8, 0x8B, 0xD6, 0x82, 0xB6, 0x82, 0xE7, 0x82, 0xEA, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xDC, 0x82, 0xB7, 0x81, 0x42, 0x0A, 0x8E, 0xA9, 0x91, 0xEE, 0x82,
	0xC9, 0x8B, 0x41, 0x82, 0xE9, 0x82, 0xC6, 0x96, 0x85, 0x82, 0xAA, 0x82, 0xC9, 0x82, 0xB1, 0x82, 0xC9, 0x82, 0xB1, 0x82, 0xB5, 0x82, 0xC4, 0x8F, 0x6F, 0x8C, 0x7D, 0x82, 0xA6, 0x82, 0xC4, 0x82,
	0xAD, 0x82, 0xEA, 0x82, 0xE9, 0x81, 0x42, 0x0A, 0x81, 0x75, 0x8F, 0xAC, 0x92, 0xB0, 0x82, 0xCC, 0xE0, 0x75, 0x94, 0xD1, 0x8B, 0x6C, 0x82, 0xDF, 0x82, 0xF0, 0x83, 0x5C, 0x81, 0x5B, 0x83, 0x5A,
	0x81, 0x5B, 0x83, 0x57, 0x82, 0xCC, 0x97, 0x76, 0x97, 0xCC, 0x82, 0xC5, 0x82, 0xE2, 0x82, 0xEA, 0x82, 0xCE, 0x20, 0x6D, 0x70, 0x6C, 0x61, 0x79, 0x65, 0x72, 0x82, 0xAA, 0x8D, 0xC4, 0x90, 0xB6,
	0x82, 0xB3, 0x82, 0xEA, 0x81, 0x41, 0x91, 0xE5, 0x82, 0xCC, 0x8E, 0x9A, 0x82, 0xC9, 0x82, 0xC8, 0x82, 0xC1, 0x82, 0xBD, 0x8F, 0xF3, 0x91, 0xD4, 0x82, 0xC5, 0x82, 0xCC, 0x81, 0x41, 0x8F, 0x57,
	0x92, 0x63, 0x82, 0xC8, 0x82, 0xF1, 0x82, 0xA9, 0x95, 0xCF, 0x82, 0xC8, 0x8A, 0xB4, 0x82, 0xB6, 0x82, 0xC9, 0x8B, 0xDF, 0x82, 0xA2, 0x8F, 0xF3, 0x91, 0xD4, 0x82, 0xC5, 0x82, 0xCD, 0x81, 0x5B,
	0x81, 0x76, 0x0A, 0x91, 0x8A, 0x95, 0xCF, 0x82, 0xED, 0x82, 0xE7, 0x82, 0xB8, 0x89, 0xBD, 0x82, 0xF0, 0x8C, 0xBE, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xE9, 0x82, 0xCC, 0x82, 0xA9, 0x82, 0xED, 0x82,
	0xA9, 0x82, 0xE7, 0x82, 0xC8, 0x82, 0xA2, 0x82, 0xAA, 0x81, 0x41, 0x82, 0xBD, 0x82, 0xD4, 0x82, 0xF1, 0x96, 0x6C, 0x82, 0xF0, 0x8A, 0xBD, 0x8C, 0x7D, 0x82, 0xB5, 0x82, 0xC4, 0x82, 0xAD, 0x82,
	0xEA, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xE9, 0x82, 0xCC, 0x82, 0xBE, 0x82, 0xEB, 0x82, 0xA4, 0x81, 0x42, 0x94, 0xDE, 0x8F, 0x97, 0x82, 0xCD, 0x82, 0xC8, 0x82, 0xC9, 0x82, 0xE7, 0x82, 0xE2, 0x82,
	0xA4, 0x82, 0xEA, 0x82, 0xB5, 0x82, 0xB0, 0x82, 0xC9, 0x81, 0x75, 0x82, 0xDF, 0x82, 0xE9, 0x82, 0xCD, 0x82, 0xCD, 0x82, 0xDF, 0x82, 0xCD, 0x82, 0xEA, 0x82, 0xAF, 0x81, 0x4B, 0x82, 0xEA, 0x82,
	0xAF, 0x82, 0xEA, 0x82, 0xAF, 0x82, 0xEA, 0x82, 0xEA, 0x82, 0xCD, 0x82, 0xEA, 0x82, 0xCD, 0x82, 0xCD, 0x82, 0xEA, 0x82, 0xDF, 0x82, 0xEB, 0x82, 0xDF, 0x82, 0xEB, 0x81, 0x76, 0x82, 0xC6, 0x8C,
	0xBE, 0x82, 0xA2, 0x82, 0xC8, 0x82, 0xAA, 0x82, 0xE7, 0x81, 0x41, 0x96, 0x6C, 0x82, 0xCC, 0x8A, 0x93, 0x82, 0xC6, 0x8F, 0xE3, 0x92, 0x85, 0x82, 0xF0, 0x8E, 0xF3, 0x82, 0xAF, 0x8E, 0xE6, 0x82,
	0xC1, 0x82, 0xBD, 0x81, 0x42, 0x0A, 0x0A, 0x94, 0xDE, 0x8F, 0x97, 0x82, 0xAA, 0x82, 0xB1, 0x82, 0xA4, 0x82, 0xC8, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xB5, 0x82, 0xDC, 0x82, 0xC1, 0x82, 0xBD, 0x82,
	0xCC, 0x82, 0xCD, 0x82, 0xBD, 0x82, 0xB5, 0x82, 0xA9, 0x88, 0xEA, 0x83, 0x96, 0x8C, 0x8E, 0x91, 0x4F, 0x82, 0xBE, 0x82, 0xC1, 0x82, 0xBD, 0x82, 0xBE, 0x82, 0xEB, 0x82, 0xA4, 0x82, 0xA9, 0x81,
	0x42, 0x96, 0x85, 0x82, 0xCD, 0x97, 0xBF, 0x97, 0x9D, 0x92, 0x86, 0x82, 0xC9, 0x82, 0xB1, 0x82, 0xF1, 0x82, 0xC8, 0x82, 0xB1, 0x82, 0xC6, 0x82, 0xF0, 0x8C, 0xFB, 0x91, 0x96, 0x82, 0xC1, 0x82,
	0xBD, 0x81, 0x42, 0x0A, 0x81, 0x75, 0x8A, 0xAE, 0x91, 0x53, 0x82, 0xC9, 0x97, 0xE2, 0x82, 0xDF, 0x82, 0xBD, 0x82, 0xE7, 0x81, 0x41, 0x8D, 0xBB, 0x93, 0x9C, 0x82, 0xF0, 0x20, 0x31, 0x35, 0x67,
	0x89, 0xC1, 0x82, 0xA6, 0x82, 0xC4, 0x81, 0x41, 0x94, 0xBC, 0x95, 0xAA, 0x82, 0xAD, 0x82, 0xE7, 0x82, 0xA2, 0x8D, 0xAC, 0x82, 0xB4, 0x82, 0xC1, 0x82, 0xBD, 0x82, 0xC6, 0x82, 0xB1, 0x82, 0xEB,
	0x82, 0xC5, 0x81, 0x41, 0xE0, 0x75, 0x94, 0xD1, 0x83, 0x54, 0x83, 0x93, 0x82, 0xCD, 0x81, 0x41, 0x82, 0xE0, 0x82, 0xA4, 0x93, 0xF1, 0x8E, 0x4F, 0x90, 0x6C, 0x82, 0xC5, 0x82, 0xB7, 0x81, 0x76,
	0x0A, 0x96, 0x6C, 0x82, 0xCD, 0x82, 0xA0, 0x82, 0xED, 0x82, 0xC4, 0x82, 0xC4, 0x97, 0xBF, 0x97, 0x9D, 0x82, 0xF0, 0x82, 0xE2, 0x82, 0xDF, 0x82, 0xB3, 0x82, 0xB9, 0x81, 0x41, 0x94, 0xDE, 0x8F,
	0x97, 0x82, 0xF0, 0x8B, 0x78, 0x82, 0xDC, 0x82, 0xB9, 0x82, 0xBD, 0x81, 0x42, 0x96, 0x85, 0x82, 0xCD, 0x89, 0xBD, 0x8C, 0xCC, 0x8E, 0xA9, 0x95, 0xAA, 0x82, 0xAA, 0x97, 0xBF, 0x97, 0x9D, 0x82,
	0xF0, 0x82, 0xE2, 0x82, 0xDF, 0x82, 0xB3, 0x82, 0xB9, 0x82, 0xE7, 0x82, 0xEA, 0x82, 0xBD, 0x82, 0xCC, 0x82, 0xA9, 0x97, 0x9D, 0x89, 0xF0, 0x82, 0xC5, 0x82, 0xAB, 0x82, 0xC8, 0x82, 0xA2, 0x82,
	0xE6, 0x82, 0xA4, 0x82, 0xC8, 0x95, 0x5C, 0x8F, 0xEE, 0x82, 0xBE, 0x82, 0xC1, 0x82, 0xBD, 0x82, 0xAA, 0x81, 0x41, 0x82, 0xBB, 0x82, 0xEA, 0x88, 0xC8, 0x97, 0x88, 0x81, 0x41, 0x94, 0xDE, 0x8F,
	0x97, 0x82, 0xCC, 0x8C, 0xBE, 0x97, 0x74, 0x82, 0xCD, 0x97, 0x9D, 0x89, 0xF0, 0x95, 0x73, 0x94, 0x5C, 0x82, 0xC9, 0x82, 0xC8, 0x82, 0xC1, 0x82, 0xBD, 0x81, 0x42, 0x0A, 0x0A, 0x94, 0xDE, 0x8F,
	0x97, 0x82, 0xCD, 0x8C, 0xBE, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xE9, 0x82, 0xB1, 0x82, 0xC6, 0x82, 0xAA, 0x88, 0xD3, 0x96, 0xA1, 0x95, 0x73, 0x96, 0xBE, 0x82, 0xC8, 0x82, 0xBE, 0x82,
	0xAF, 0x82, 0xC5, 0x81, 0x41, 0x8E, 0x76, 0x8D, 0x6C, 0x8E, 0xA9, 0x91, 0xCC, 0x82, 0xCD, 0x82, 0xDC, 0x82, 0xC6, 0x82, 0xE0, 0x82, 0xC8, 0x82, 0xE6, 0x82, 0xA4, 0x82, 0xC8, 0x82, 0xCC, 0x82,
	0xC5, 0x81, 0x41, 0x8D, 0xC5, 0x8B, 0xDF, 0x82, 0xCD, 0x82, 0xA0, 0x82, 0xDC, 0x82, 0xE8, 0x96, 0x6C, 0x82, 0xE0, 0x8B, 0x43, 0x82, 0xC9, 0x82, 0xB5, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xC8, 0x82,
	0xA2, 0x81, 0x42, 0x82, 0xE0, 0x82, 0xC6, 0x82, 0xE0, 0x82, 0xC6, 0x88, 0xC0, 0x95, 0xA8, 0x82, 0xBE, 0x82, 0xA9, 0x82, 0xE7, 0x81, 0x41, 0x82, 0xC6, 0x82, 0xA2, 0x82, 0xA4, 0x82, 0xCC, 0x82,
	0xE0, 0x82, 0xA0, 0x82, 0xE9, 0x81, 0x42, 0x82, 0xC6, 0x82, 0xC1, 0x82, 0xAD, 0x82, 0xC9, 0x8E, 0xF5, 0x96, 0xBD, 0x82, 0xCD, 0x89, 0xDF, 0x82, 0xAC, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xE9, 0x82,
	0xED, 0x82, 0xAF, 0x82, 0xBE, 0x82, 0xB5, 0x81, 0x41, 0x97, 0xBF, 0x97, 0x9D, 0x90, 0xEA, 0x96, 0xE5, 0x82, 0xCC, 0x83, 0x41, 0x83, 0x93, 0x83, 0x68, 0x83, 0x8D, 0x83, 0x43, 0x83, 0x68, 0x82,
	0xC9, 0x89, 0xC6, 0x8E, 0x96, 0x91, 0x53, 0x94, 0xCA, 0x82, 0xF0, 0x82, 0xE2, 0x82, 0xE7, 0x82, 0xB9, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xE9, 0x82, 0xF1, 0x82, 0xBE, 0x82, 0xA9, 0x82, 0xE7, 0x89,
	0xDF, 0x95, 0x89, 0x89, 0xD7, 0x82, 0xAA, 0x82, 0xA9, 0x82, 0xA9, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xE9, 0x82, 0xCC, 0x82, 0xCD, 0x95, 0xAA, 0x82, 0xA9, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xA2, 0x82,
	0xE9, 0x81, 0x42, 0x0A, 0x0A, 0x82, 0xC6, 0x82, 0xCD, 0x8C, 0xBE, 0x82, 0xC1, 0x82, 0xC4, 0x82, 0xE0, 0x81, 0x63, 0x81, 0x63, 0x81, 0x63, 0x81, 0x42, 0x97, 0xBF, 0x97, 0x9D, 0x92, 0x86, 0x82,
	0xC9, 0x81, 0x75, 0x82, 0xC5, 0x82, 0xE0, 0x8E, 0xE8, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82,
	0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81,
	0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83,
	0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83,
	0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82,
	0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83,
	0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83,
	0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83,
	0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82,
	0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83,
	0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x82, 0xC2, 0x82, 0xA2, 0x82, 0xC5, 0x82, 0xC9, 0x8E, 0x43, 0x82, 0xC1, 0x82, 0xBD, 0x83, 0x4D, 0x83, 0x55, 0x83, 0x4D, 0x83, 0x55, 0x83,
	0x74, 0x83, 0x89, 0x83, 0x43, 0x83, 0x70, 0x83, 0x93, 0x82, 0xE0, 0x83, 0x6C, 0x83, 0x6F, 0x83, 0x6C, 0x83, 0x6F, 0x81, 0x60, 0x81, 0x76, 0x82, 0xC6, 0x82, 0xA9, 0x95, 0x40, 0x89, 0xCC, 0x8D,
	0xAC, 0x82, 0xB6, 0x82, 0xE8, 0x82, 0xC5, 0x89, 0xCC, 0x82, 0xED, 0x82, 0xEA, 0x82, 0xE9, 0x82, 0xC6, 0x81, 0x41, 0x82, 0xA2, 0x82, 0xC1, 0x82, 0xBD, 0x82, 0xA2, 0x89, 0xBD, 0x82, 0xF0, 0x90,
	0x48, 0x82, 0xED, 0x82, 0xB3, 0x82, 0xEA, 0x82, 0xE9, 0x82, 0xCC, 0x82, 0xA9, 0x94, 0xF1, 0x8F, 0xED, 0x82, 0xC9, 0x95, 0x73, 0x88, 0xC0, 0x82, 0xC9, 0x82, 0xC8, 0x82, 0xE9, 0x81, 0x42, 0x0A,
	0xCF, 0xE8, 0x67, 0x03, 0x62, 0x0D, 0x58, 0x2B, 0x48, 0xE7, 0x23, 0x1C, 0x0C, 0xCB, 0xFC, 0xA4, 0xC7, 0xDB, 0xCE, 0x6A, 0x7D, 0x22, 0x6A, 0x9E, 0xE1, 0x67, 0x89, 0x5E, 0x37, 0xF9, 0x5E, 0x12,
	0xE2, 0x74, 0xF3, 0x5D, 0xF5, 0x3E, 0x32, 0xE6, 0xC1, 0x7F, 0xE6, 0xDD, 0x09, 0xD2, 0xE4, 0x74, 0x7C, 0xC5, 0xCC, 0xD6, 0xD3, 0x1B, 0x46, 0xB7, 0x37, 0xE3, 0x68, 0xD2, 0xC9, 0x46, 0x30, 0x57,
	0x8C, 0xD2, 0x87, 0x96, 0x5B, 0x00, 0x94, 0x82, 0x3B, 0x82, 0x60, 0xF5, 0xD4, 0xDA, 0x13, 0x1E, 0xC1, 0x45, 0x8E, 0xDE, 0x4F, 0xF6, 0x4C, 0xF7, 0xE5, 0xD7, 0xCE, 0x46, 0x02, 0x85, 0x69, 0x9C,
	0x6D, 0x4F, 0x17, 0x80, 0xF6, 0xD7, 0x81, 0x12, 0x77, 0x0C, 0x21, 0xF6, 0x92, 0x56, 0xBF, 0x87, 0x57, 0x66, 0x83, 0x1C, 0x8F, 0xEF, 0xE6, 0xFD, 0x8A, 0xD4, 0x2D, 0x3A, 0xFF, 0xF3, 0x18, 0xDE,
	0xE0, 0x05, 0x74, 0x0D, 0x14, 0x1E, 0x02, 0xA2, 0x6E, 0x13, 0x2A, 0x3F, 0x15, 0x20, 0xDC, 0xF3, 0xD5, 0x5B, 0xA7, 0x29, 0x52, 0x79, 0x7A, 0x08, 0x8D, 0x6B, 0x72, 0x38, 0xEA, 0x1C, 0x85, 0x76,
	0x1F, 0x58, 0xBC, 0x83, 0x62, 0x0A, 0x9E, 0x86, 0xB8, 0x91, 0xA1, 0x9A, 0xCF, 0xD6, 0x5C, 0xF0, 0xEB, 0xB6, 0xAC, 0x20, 0x4F, 0x73, 0x01, 0x2E, 0xDD, 0xC0, 0x7C, 0x24, 0x1C, 0xFC, 0x15, 0xCB,
	0xB5, 0xBA, 0xDD, 0x94, 0x3B, 0x5C, 0x9D, 0x40, 0x4A, 0x57, 0x98, 0x13, 0x30, 0xA2, 0xD3, 0x1D, 0x16, 0x87, 0x58, 0x22, 0x06, 0xA7, 0x02, 0x3E, 0xC4, 0xDD, 0x48, 0x69, 0x44, 0xCD, 0x35, 0x83,
	0x02, 0xF9, 0x50, 0xA6, 0x8E, 0x8B, 0x50, 0x36, 0x5A, 0xA2, 0xAA, 0x4D, 0xAD, 0xD3, 0xAC, 0x96, 0x05, 0x2A, 0xBA, 0xA7, 0xCC, 0x64, 0xBF, 0x17, 0x6C, 0x70, 0x09, 0x45, 0xE2, 0x2F, 0x75, 0xF8,
	0x8E, 0x53, 0xE4, 0x83, 0x1E, 0x0F, 0x32, 0xF4, 0x20, 0x58, 0xAA, 0x8A, 0x31, 0x67, 0xC3, 0x45, 0x6D, 0x6E, 0x0F, 0x64, 0x4D, 0xD9, 0x54, 0x31, 0x6C, 0xC3, 0x82, 0xE6, 0xB6, 0xED, 0x2B, 0x32,
	0x73, 0x89, 0xC2, 0xB4, 0x57, 0x25, 0x0A, 0x4F, 0xBA, 0x12, 0xAA, 0x36, 0xAF, 0x5A, 0x1C, 0x53, 0x8D, 0xEB, 0x84, 0xEB, 0x4A, 0x87, 0xF1, 0x99, 0x8F, 0xB2, 0x49, 0x2B, 0x0F, 0x23, 0x48, 0x22,
	0x09, 0xC3, 0x02, 0xC3, 0x00, 0x3B, 0xE3, 0x0D, 0x6D, 0xBE, 0x91, 0xAB, 0x29, 0xF4, 0x6E, 0xF9, 0xA3, 0x3E, 0x9B, 0xDF, 0xC7, 0xFC, 0x17, 0x1F, 0x26, 0xC8, 0xC3, 0xB8, 0x91, 0x82, 0xB5, 0xDA,
	0x36, 0xF6, 0x77, 0xB1, 0x81, 0x50, 0x87, 0xE4, 0x8D, 0x1C, 0x8E, 0x68, 0x52, 0xD2, 0x54, 0xF8, 0x94, 0x68, 0x7E, 0xF6, 0x01, 0xA3, 0x6E, 0xFB, 0x8E, 0xE5, 0x95, 0x28, 0xCB, 0xF1, 0x61, 0x95,
	0xFC, 0x0E, 0x8C, 0x4C, 0x06, 0xFC, 0xCF, 0x91, 0x3D, 0x48, 0x96, 0x8B, 0x22, 0x6F, 0xD5, 0xF4, 0x01, 0x96, 0x6F, 0xFA, 0x49, 0xDD, 0x94, 0x98, 0xC0, 0x0A, 0xCA, 0x1C, 0xD4, 0xCD, 0xD4, 0x2F,
	0xA3, 0x9F, 0xDD, 0xB2, 0xB8, 0xED, 0xFA, 0xB7, 0x55, 0x26, 0x3C, 0x42, 0x90, 0x6D, 0x0B, 0xEA, 0x91, 0x91, 0x1B, 0x1F, 0xA8, 0xAB, 0x13, 0xE2, 0x9D, 0x26, 0x69, 0xDE, 0x30, 0x08, 0x79, 0x73,
	0x32, 0xE4, 0xD6, 0xC8, 0xCC, 0x10, 0x56, 0xD9, 0x2B, 0xDB, 0xA0, 0xF7, 0xB2, 0x5B, 0x25, 0x93, 0xA2, 0x0F, 0x98, 0xDF, 0xF4, 0xA4, 0xC5, 0xBA, 0xBE, 0x15, 0x46, 0x64, 0xC3, 0x68, 0x69, 0xDA,
	0xF3, 0xBA, 0x1E, 0x49, 0x32, 0x23, 0x6B, 0x25, 0x72, 0x24, 0xAA, 0x78, 0x0A, 0xB7, 0x87, 0xE1, 0xB7, 0x31, 0xC3, 0x21, 0x9D, 0xFD, 0xE3, 0x61, 0x7D, 0x19, 0xC4, 0x6D, 0x7E, 0xA3, 0xF0, 0x1B,
	0xCF, 0xE8, 0xF7, 0xD5, 0x14, 0x03, 0x00, 0xCB, 0x08, 0x3D, 0xE1, 0xD3, 0x34, 0x75, 0xC2, 0xE4, 0xCF, 0xE2, 0xC2, 0xE8, 0x75, 0x5E, 0x83, 0x1A, 0x88, 0x83, 0x47, 0x74, 0xA9, 0x16, 0x0D, 0xB1,
	0x33, 0xED, 0xF7, 0x19, 0xF1, 0x57, 0xA5, 0x31, 0xD6, 0xA1, 0x94, 0xB9, 0xC8, 0x41, 0xB2, 0xCB, 0x6C, 0xE2, 0xF0, 0x54, 0xAE, 0x44, 0x41, 0xB0, 0xAE, 0xDA, 0xA8, 0xC4, 0x63, 0xE2, 0xB1, 0xAF,
	0x4B, 0x0F, 0xAF, 0x6A, 0x16, 0xF9, 0x87, 0xB3, 0x1D, 0xE5, 0x19, 0x8C, 0x99, 0x38, 0x6D, 0x52, 0x8E, 0xB2, 0xE7, 0x24, 0x55, 0x35, 0x54, 0x45, 0x32, 0xB5, 0xCB, 0x60, 0xB4, 0xA1, 0xFD, 0x8E,
	0x4E, 0xFA, 0xAB, 0x08, 0xA2, 0x45, 0xD5, 0xB4, 0x59, 0xDF, 0xDA, 0x74, 0x23, 0xF2, 0xD8, 0x47, 0xDE, 0x2B, 0xF0, 0x0A, 0x82, 0xFE, 0xD0, 0x7C, 0x54, 0x09, 0xBB, 0x8B, 0xE8, 0x7F, 0x33, 0x41,
	0x94, 0x9E, 0x67, 0x4F, 0x74, 0x18, 0xF4, 0xC5, 0x0C, 0xF0, 0x82, 0xAA, 0x60, 0xF6, 0x12, 0xFA, 0xC6, 0x77, 0x9C, 0x83, 0x7C, 0x43, 0x57, 0xE6, 0x6A, 0xB2, 0x6F, 0x28, 0xEE, 0xEA, 0x31, 0x49,
	0xC3, 0x93, 0x95, 0x8E, 0x20, 0x3F, 0xB8, 0x22, 0xA9, 0x1C, 0x73, 0x3C, 0x45, 0xF9, 0x00, 0x49, 0x3F, 0xB6, 0xBA, 0x83, 0x22, 0x1C, 0xF1, 0x5C, 0xAA, 0xF3, 0x88, 0xB2, 0x83, 0x27, 0x1B, 0x93,
	0x4A, 0x4C, 0xDB, 0x4A, 0xCB, 0x94, 0xF0, 0xB4, 0x58, 0xBA, 0x57, 0x0D, 0xA9, 0x98, 0xB1, 0x15, 0x87, 0x03, 0x7A, 0x1F, 0xC1, 0x01, 0x57, 0x87, 0x80, 0xA4, 0xC1, 0x15, 0x61, 0x05, 0x8B, 0xCC,
	0xED, 0x46, 0x61, 0xAB, 0x14, 0x07, 0xF8, 0x7A, 0xE4, 0xFB, 0x86, 0x50, 0xD2, 0xD3, 0x64, 0x56, 0x96, 0x28, 0x6C, 0x8D, 0x25, 0x45, 0x64, 0x65, 0xA0, 0xE3, 0xDB, 0x9D, 0x3F, 0xEC, 0xFD, 0x04,
	0x21, 0x05, 0x59, 0x58, 0x60, 0xDA, 0xA0, 0x6F, 0x82, 0x0B, 0xFA, 0x3E, 0xED, 0x00, 0xE7, 0x0E, 0x70, 0xAD, 0xAD, 0x26, 0xA9, 0xDC, 0x76, 0x75, 0x14, 0xF2, 0x2F, 0xBD, 0xCF, 0x89, 0xAD, 0x4C,
	0x78, 0x12, 0x59, 0xF5, 0x35, 0xC6, 0x9A, 0x53, 0xC1, 0x04, 0x23, 0x41, 0x2D, 0x03, 0x8F, 0x67, 0x3B, 0x1A, 0xEC, 0x38, 0xF5, 0xB7, 0xFF, 0xDF, 0x17, 0x0D, 0x45, 0xF6, 0x0C, 0x60, 0x41, 0x98,
	0xDF, 0xC7, 0xA9, 0x14, 0x76, 0x56, 0x34, 0xFC, 0xD0, 0x75, 0xC0, 0xE3, 0xD1, 0x82, 0x18, 0x2C, 0xB6, 0x17, 0x20, 0xE3, 0xBE, 0x67, 0x79, 0x46, 0xE0, 0xFD, 0x94, 0x96, 0x10, 0x72, 0x64, 0x23,
	0xEC, 0x16, 0xE1, 0x4B, 0x2E, 0x29, 0x9F, 0x81, 0x6D, 0x4A, 0x9B, 0xB2, 0x59, 0x5A, 0x14, 0xE6, 0xEC, 0x35, 0x1B, 0x36, 0x39, 0x4F, 0x93, 0x15, 0xBD, 0x19, 0xBC, 0x94, 0xAC, 0x4B, 0xA3, 0x9B,
	0x7B, 0xA5, 0x8C, 0xB6, 0x3D, 0xE5, 0x87, 0x74, 0x31, 0xBF, 0x54, 0xDB, 0x1A, 0x89, 0x3D, 0x62, 0xCE, 0x1B, 0x47, 0x68, 0x08, 0xAE, 0xBC, 0x79, 0x22, 0x82, 0xEF, 0xBF, 0x91, 0xA7, 0xC3, 0x45,
	0xDE, 0x62, 0x85, 0x25, 0x18, 0x1C, 0xB0, 0xE9, 0xF9, 0x31, 0x7B, 0x47, 0x03, 0xE8, 0x68, 0x4F, 0x04, 0xE3, 0xBC, 0xCD, 0xE0, 0xA7, 0x69, 0xCD, 0x04, 0x53, 0xD8, 0x90, 0x3C, 0x7E, 0x56, 0x6E,
	0x5C, 0x67, 0x66, 0xB5, 0xE3, 0x59, 0x67, 0x9E, 0x3B, 0x01, 0xC6, 0x6D, 0xC5, 0x50, 0xC9, 0xC5, 0xAF, 0xD0, 0x0C, 0x5A, 0xA7, 0xA8, 0xBB, 0xBD, 0x2A, 0x0E, 0xAD, 0x53, 0x6F, 0xA3, 0x4E, 0xD7,
	0xBF, 0xE4, 0xB0, 0x8B, 0xA2, 0xA4, 0xF9, 0x65, 0x6D, 0x51, 0x9C, 0x22, 0x26, 0x0D, 0x5F, 0x6E, 0x54, 0xB2, 0x65, 0x76, 0xCA, 0xD7, 0x5C, 0x58, 0xC3, 0x74, 0xB8, 0xEC, 0xE9, 0x4E, 0x46, 0xE9,
	0xB8, 0xCA, 0x80, 0x09, 0xC5, 0xA8, 0x7C, 0xAD, 0x55, 0x55, 0x2C, 0x20, 0x3C, 0xC4, 0x36, 0x12, 0x40, 0xB3, 0xDE, 0x54, 0x8D, 0xCB, 0xC1, 0xD3, 0x00, 0x21, 0x9B, 0xF1, 0x36, 0x3B, 0x50, 0x69,
	0x4B, 0x81, 0x00, 0x9E, 0x4C, 0xD9, 0x20, 0x56, 0x7B, 0x7B, 0xE4, 0x8D, 0xCE, 0xA2, 0x44, 0xC2, 0x98, 0x43, 0x25, 0x15, 0x8F, 0x5D, 0x9B, 0x46, 0x6A, 0x91, 0xFD, 0x65, 0xB6, 0x2A, 0x78, 0x6A,
	0x15, 0xE3, 0xE9, 0x0D, 0xC0, 0xB5, 0x4F, 0x9A, 0x75, 0xAC, 0xF0, 0xB4, 0x26, 0x81, 0xAD, 0xE7, 0x59, 0xF1, 0xD8, 0x78, 0xD2, 0xBD, 0xB8, 0x91, 0xB6, 0xE8, 0x78, 0xCF, 0xBC, 0x03, 0x4B, 0xCC,
	0x98, 0xBD, 0x39, 0xD2, 0xCD, 0x82, 0x58, 0xCA, 0xD6, 0x5E, 0x99, 0x40, 0x7F, 0x4E, 0x94, 0xF8, 0x0D, 0xE7, 0x3A, 0xD3, 0x6A, 0x1E, 0x87, 0xBD, 0x63, 0x26, 0xEE, 0xE0, 0xEB, 0x4E, 0x91, 0x74,
	0xE5, 0x9A, 0xFF, 0xFF, 0x25, 0xB8, 0xEB, 0x9D, 0x84, 0xC4, 0xB3, 0xB1, 0xE5, 0xF1, 0xD4, 0x8B, 0xEB, 0x4E, 0x12, 0x96, 0x35, 0x28, 0x96, 0x88, 0xCE, 0x9B, 0xA1, 0x81, 0x21, 0xD7, 0x0D, 0x0A,
	0xC5, 0x0F, 0x5B, 0xB9, 0x11, 0x50, 0x18, 0x4C, 0x21, 0x4C, 0xDC, 0xC5, 0xB9, 0xEF, 0x55, 0x0E, 0x2C, 0xD9, 0xED, 0x18, 0x41, 0x04, 0xB0, 0xE9, 0x0D, 0xC7, 0x35, 0x74, 0x26, 0x1F, 0xF4, 0x7D,
	0xC7, 0xD1, 0x90, 0xA4, 0xC6, 0x4F, 0x29, 0xCD, 0xE4, 0x3B, 0x59, 0xA2, 0x21, 0x0D, 0x35, 0xE2, 0x4D, 0xB1, 0x08, 0xF2, 0x2F, 0xAA, 0xEC, 0xC5, 0x47, 0x79, 0x67, 0x50, 0x69, 0x7F, 0x5F, 0x00,
	0xCA, 0x53, 0xD2, 0xFA, 0xDE, 0x36, 0xF4, 0xCF, 0x84, 0x9C, 0x3E, 0xFB, 0x9E, 0x2D, 0x16, 0x13, 0x00, 0x93, 0x13, 0x21, 0xCE, 0xC7, 0x70, 0x12, 0x51, 0xE4, 0x42, 0x1C, 0x82, 0x9B, 0x83, 0x55,
	0x68, 0xA4, 0x1F, 0x08, 0xA8, 0xA3, 0x13, 0xDA, 0x8F, 0x45, 0xB0, 0xE9, 0x07, 0x36, 0xC7, 0xFC, 0x19, 0xC8, 0xAB, 0xB4, 0xAE, 0x6C, 0x13, 0xB5, 0x47, 0x63, 0xFA, 0xBD, 0xBB, 0xFA, 0x58, 0x11,
	0xE6, 0xDA, 0x52, 0xDE, 0x8F, 0xE5, 0x90, 0x36, 0xDC, 0x34, 0x7A, 0xC4, 0x37, 0xA6, 0x05, 0x04, 0x9A, 0xCA, 0x25, 0xC1, 0x6B, 0xCD, 0xB2, 0xCB, 0x65, 0xBF, 0x49, 0x05, 0x68, 0x78, 0x07, 0x29,
	0xFD, 0x43, 0xC8, 0xE0, 0xD8, 0xB5, 0xED, 0xE8, 0x5D, 0x00, 0xB7, 0xB4, 0x97, 0x39, 0x1D, 0xC4, 0xBE, 0xD2, 0x0C, 0xEC, 0xF0, 0xD0, 0x78, 0xAA, 0x67, 0x9D, 0x7A, 0x1A, 0xBE, 0x5F, 0x50, 0x80,
	0x2D, 0x98, 0x41, 0x86, 0x7B, 0x68, 0x35, 0x84, 0x14, 0xA7, 0x4F, 0x40, 0x5A, 0x3D, 0x29, 0x4C, 0xAB, 0x85, 0x7C, 0xB2, 0x76, 0x2A, 0x46, 0x4E, 0x22, 0x72, 0x65, 0xF7, 0x7C, 0x53, 0x2D, 0xB1,
	0xE6, 0x7A, 0x45, 0x0A, 0x55, 0x47, 0xD6, 0x0C, 0x67, 0xE9, 0x89, 0x13, 0x9B, 0x6D, 0x2A, 0xCE, 0x1E, 0x8C, 0xA6, 0x40, 0x78, 0x48, 0x10, 0xFF, 0xA0, 0x24, 0xA9, 0xC9, 0x16, 0x67, 0xF2, 0xE8,
	0x3C, 0xF7, 0x54, 0xB9, 0x71, 0x5C, 0x0B, 0x25, 0x14, 0x7F, 0x73, 0xBC, 0x2B, 0x89, 0x3E, 0xE7, 0x3C, 0xD4, 0xB1, 0x16, 0x5F, 0x48, 0xE5, 0x67, 0x01, 0x20, 0x53, 0xE6, 0x29, 0x8A, 0x29, 0x72,
	0xEF, 0xCF, 0x11, 0xB4, 0xA9, 0xE9, 0x13, 0x63, 0x08, 0xED, 0xDF, 0x2E, 0x3E, 0x6D, 0x31, 0x4C, 0x5C, 0x32, 0x4A, 0xB3, 0xAB, 0xC4, 0xE2, 0x5A, 0xD3, 0xB0, 0x3B, 0x1C, 0xFF, 0x93, 0x6D, 0x55,
	0x02, 0xC9, 0x3D, 0x58, 0xE0, 0x4E, 0xEE, 0x5C, 0x54, 0x9F, 0xB1, 0xB1, 0xC9, 0xE5, 0xAE, 0x1D, 0x00, 0xAD, 0x93, 0x8C, 0xCF, 0x25, 0x4F, 0x0E, 0xB4, 0xBE, 0x58, 0x9E, 0x4F, 0x32, 0xFA, 0xA9,
	0x28, 0xC3, 0xAB, 0x15, 0x95, 0x22, 0xF2, 0xD7, 0x83, 0x26, 0x42, 0x9A, 0x40, 0xFE, 0xCA, 0x96, 0x3E, 0x2F, 0xB0, 0x0F, 0x08, 0x49, 0xB3, 0x0E, 0xF3, 0xD2, 0xC4, 0x4E, 0x0F, 0xF5, 0x46, 0x77,
	0xB4, 0x54, 0x83, 0xED, 0x10, 0xB3, 0x62, 0xCB, 0x91, 0xC8, 0x9C, 0xEE, 0x89, 0x1B, 0x2C, 0xE5, 0xD3, 0x60, 0xC9, 0xEB, 0xF0, 0x37, 0x07, 0x16, 0x67, 0x03, 0x3C, 0x95, 0x0D, 0x31, 0xCA, 0xD3,
	0xEB, 0x2F, 0xFF, 0x59, 0xE3, 0x3B, 0x55, 0x70, 0x43, 0xA7, 0x47, 0xCF, 0x49, 0x86, 0x10, 0x60, 0xB7, 0xE0, 0x9D, 0xEE, 0x00, 0x20, 0x5C, 0x25, 0xD6, 0x28, 0xCA, 0xA6, 0x8E, 0x0F, 0xCE, 0xFF,
	0x58, 0x09, 0x75, 0x60, 0x89, 0x01, 0xAB, 0x21, 0x9E, 0x3D, 0x6B, 0x59, 0x64, 0xB8, 0x6C, 0x63, 0x4B, 0xB2, 0x8B, 0x1D, 0x58, 0x71, 0xB1, 0x03, 0x92, 0xF2, 0x6E, 0x51, 0xB4, 0x6E, 0xF2, 0x50,
	0x79, 0xCC, 0xE3, 0x18, 0xD8, 0x29, 0x4F, 0x56, 0x40, 0xF4, 0x84, 0xBD, 0x62, 0x2F, 0x69, 0x07, 0x0E, 0x00, 0xB3, 0x66, 0x71, 0xAD, 0xAE, 0x7B, 0x1A, 0xC5, 0xF5, 0xB1, 0x76, 0x65, 0xAF, 0x93,
	0xF7, 0xCE, 0x3C, 0x47, 0x12, 0x70, 0x82, 0x68, 0x5A, 0x0B, 0x6F, 0x5A, 0x79, 0xF5, 0x00, 0x07, 0xF0, 0x15, 0x00, 0x14, 0x92, 0x61, 0x12, 0xFF, 0x7D, 0x86, 0x44, 0x7C, 0x50, 0xAC, 0x99, 0xB2,
	0x37, 0x4C, 0x41, 0x00, 0x46, 0x9A, 0x74, 0x6E, 0x84, 0x19, 0x8C, 0x05, 0x07, 0xA8, 0xF0, 0x80, 0x24, 0x30, 0x5A, 0x51, 0x3C, 0xDB, 0x89, 0x0A, 0x36, 0x22, 0x5D, 0xA1, 0x8F, 0x98, 0x9F, 0x66,
	0x6D, 0xF2, 0x14, 0xD7, 0x52, 0xCE, 0xED, 0xEA, 0x40, 0x68, 0xAD, 0xDD, 0xAE, 0xC8, 0xD0, 0xD1, 0x16, 0x83, 0x37, 0xEC, 0xC7, 0x77, 0x2F, 0xEA, 0x54, 0x04, 0xD5, 0x0E, 0xD5, 0x00, 0xFC, 0x1F,
	0x91, 0xA1, 0x59, 0x92, 0xB9, 0x1D, 0x3A, 0x0E, 0x4B, 0x5B, 0xA1, 0xC8, 0x5E, 0x7B, 0x81, 0x69, 0xDB, 0x1D, 0xD1, 0x36, 0x82, 0xB6, 0x04, 0x47, 0x9D, 0xC7, 0xFE, 0xC9, 0x2F, 0xB6, 0xB6, 0xF3,
	0xD4, 0x40, 0x98, 0xF5, 0x91, 0x7C, 0xB2, 0xC5, 0x1D, 0x3C, 0x11, 0x78, 0xFE, 0x95, 0xB7, 0xAA, 0x50, 0xF1, 0x46, 0x9C, 0x04, 0xE6, 0x06, 0x10, 0xE9, 0x60, 0x03, 0x9F, 0x6E, 0x69, 0x4A, 0x58,
	0x75, 0xD7, 0xA4, 0x85, 0x4E, 0xEB, 0x7E, 0x61, 0xC4, 0x0E, 0x74, 0xA3, 0x8A, 0x97, 0xE2, 0xB9, 0x20, 0x4D, 0x55, 0xD5, 0x7A, 0x18, 0xBF, 0xA9, 0x91, 0xFB, 0xC3, 0xBD, 0x91, 0x9C, 0xDD, 0x50,
	0x17, 0xAA, 0x41, 0xEB, 0x1C, 0x0A, 0x72, 0x8E, 0x60, 0x44, 0xCF, 0xB4, 0xD3, 0xF8, 0x37, 0xE8, 0x8B, 0x6F, 0x86, 0xD4, 0x29, 0xA5, 0xF3, 0x7D, 0x89, 0x4F, 0x14, 0x4F, 0xE9, 0x03, 0xA8, 0xE6,
	0x75, 0x1D, 0x5B, 0x49, 0x1F, 0xD9, 0xAF, 0x5D, 0x7D, 0x09, 0x95, 0xC1, 0xA4, 0xE3, 0x0D, 0x0D, 0x0D, 0x86, 0x70, 0x4C, 0xED, 0xFF, 0xCE, 0xCD, 0x0C, 0x2B, 0xBE, 0x0E, 0x58, 0xFB, 0xDF, 0x26,
	0x9F, 0x9E, 0x71, 0x36, 0xC0, 0x6F, 0x59, 0x23, 0x11, 0xD9, 0x53, 0x6D, 0x17, 0x38, 0xDB, 0x6D, 0x43, 0xD9, 0x6C, 0xF6, 0xEE, 0x94, 0x28, 0x6E, 0xFA, 0x74, 0xF2, 0xD7, 0xC4, 0x60, 0xD5, 0x5C,
	0x07, 0x5C, 0x97, 0x25, 0x59, 0x59, 0x8F, 0x63, 0xDB, 0xBC, 0x75, 0x92, 0x4C, 0x07, 0x98, 0x1A, 0x36, 0xD3, 0x64, 0x68, 0xCD, 0x7E, 0x4A, 0x37, 0xC7, 0x61, 0x6D, 0x28, 0xA9, 0x83, 0x9D, 0xCF,
	0x23, 0xF5, 0x2A, 0x79, 0xF3, 0x68, 0xF8, 0x14, 0xDF, 0xCF, 0x5D, 0x5B, 0xCF, 0xD2, 0x2C, 0xE5, 0x5C, 0x36, 0x0B, 0xB0, 0x8C, 0xBF, 0xD0, 0x67, 0x96, 0xA7, 0x91, 0x35, 0xDC, 0x7F, 0xEA, 0xC4,
	0xDE, 0x7F, 0x99, 0x7E, 0x66, 0x2D, 0x9B, 0xF5, 0x46, 0xE4, 0x10, 0x20, 0x60, 0xC3, 0x45, 0x3D, 0x8A, 0xB3, 0x39, 0x05, 0xED, 0x50, 0x7E, 0xB8, 0xEA, 0xE1, 0xB5, 0x51, 0x6A, 0xA9, 0xE2, 0x85,
	0x17, 0x04, 0xE5, 0x10, 0x8C, 0xEC, 0xBC, 0x30, 0xDA, 0xFF, 0x37, 0x37, 0xE7, 0xAA, 0xCB, 0x29, 0x94, 0xAD, 0xAA, 0xB4, 0x89, 0x3C, 0x47, 0x68, 0x06, 0x68, 0xE9, 0x7D, 0xB6, 0x62, 0x3D, 0xA1,
	0x72, 0x52, 0x2E, 0x31, 0x47, 0xAD, 0x13, 0x18, 0x68, 0x60, 0xB5, 0x85, 0x51, 0x72, 0x26, 0xA9, 0x60, 0x5D, 0xE4, 0x22, 0x5F, 0x8A, 0xB5, 0x71, 0xD7, 0xE8, 0x6B, 0x44, 0x1E, 0x88, 0xD2, 0xBE,
	0xBC, 0x4C, 0x01, 0x40, 0x2D, 0xCC, 0x02, 0x2F, 0x3E, 0xEB, 0x91, 0x03, 0x93, 0x5A, 0xAD, 0x3A, 0xA3, 0x1F, 0x02, 0x14, 0x9E, 0xB5, 0x02, 0xBC, 0x1D, 0xE8, 0x98, 0xC2, 0xF1, 0xC3, 0xE5, 0x1B,
	0x99, 0x81, 0x2E, 0x1E, 0x7F, 0xB0, 0x9F, 0x4B, 0x0F, 0x23, 0x71, 0x83, 0x97, 0xA8, 0x6A, 0x37, 0x19, 0x28, 0x1C, 0x03, 0xE6, 0xF1, 0x26, 0x77, 0x11, 0x87, 0x08, 0xF1, 0x62, 0x71, 0xFB, 0xA5,
	0xCC, 0xCC, 0x5C, 0x87, 0xC0, 0x6D, 0x4B, 0x7F, 0x6E, 0xFF, 0x74, 0x6B, 0xD5, 0x9D, 0xF8, 0x64, 0xE8, 0xDE, 0x9E, 0x39, 0x41, 0x03, 0x02, 0xF0, 0xD2, 0x37, 0xA5, 0xA6, 0x50, 0x6F, 0x58, 0x44,
	0xC7, 0x38, 0x28, 0x42, 0xA4, 0x1D, 0x12, 0x24, 0xB4, 0x4E, 0x13, 0x32, 0x4A, 0x6C, 0xC7, 0xD5, 0x6D, 0x19, 0x3D, 0xFC, 0x16, 0x65, 0x06, 0x94, 0xCB, 0xA1, 0x77, 0x57, 0x8A, 0x08, 0x8B, 0xAE,
	0x4E, 0x6F, 0x84, 0x11, 0x15, 0xA0, 0x19, 0x2D, 0x01, 0xFA, 0x5F, 0xCE, 0x24, 0x63, 0xF4, 0x7D, 0x48, 0x09, 0xD1, 0xFF, 0xD2, 0xF7, 0xFA, 0xE7, 0xDE, 0x8F, 0x5C, 0x88, 0xE8, 0x37, 0xAE, 0x00,
	0xEE, 0xE7, 0x9F, 0xFB, 0x94, 0x90, 0x56, 0x0D, 0x1C, 0x5D, 0xD4, 0x60, 0x50, 0x1F, 0x03, 0x19, 0x4B, 0x86, 0xD8, 0xDF, 0x0A, 0x7E, 0xB6, 0x2E, 0x11, 0x83, 0x85, 0x08, 0xE0, 0x12, 0x22, 0xD1,
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
	switch (xcode_rand(xcode) % 3) {
	case 1:
		// MOV EAX, EDI
		if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
			return 0;
		break;
	case 2:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
			|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
	case 0:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
			|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
			|| !push_2bytesxcode(xcode, 0x8b, 0x86)
			|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
			return 0;
		break;
	}
	return 1;
}

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	}
	else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	switch (xcode_rand(xcode) & 7) {
	case 4:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
		break;
	case 5:
		// NEG EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
		break;
	case 1:
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
		break;
	case 0:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
		break;
	case 2:
		// PUSH EBX
		// MOV EBX, EAX
		// AND EBX, AAAAAAAA
		// AND EAX, 55555555
		// SHR EBX, 1
		// SHL EAX, 1
		// OR EAX, EBX
		// POP EBX
		if (!push_bytexcode(xcode, 0x53)
			|| !push_2bytesxcode(xcode, 0x89, 0xc3)
			|| !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
			|| !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
			|| !push_2bytesxcode(xcode, 0xd1, 0xeb)
			|| !push_2bytesxcode(xcode, 0xd1, 0xe0)
			|| !push_2bytesxcode(xcode, 0x09, 0xd8)
			|| !push_bytexcode(xcode, 0x5b))
			return 0;
		break;
	case 6:
		// XOR EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0x35)
			|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
	case 7:
		if (xcode_rand(xcode) & 1) {
			// ADD EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x05))
				return 0;
		}
		else {
			// SUB EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x2d))
				return 0;
		}
		if (!push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
		break;
	case 3:
		// MOV ESI, EncryptionControlBlock
		// AND EAX, 3FFh
		// MOV EAX, DWORD PTR DS:[ESI+EAX*4]
		if (!push_bytexcode(xcode, 0xbe)
			|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
			|| !push_bytexcode(xcode, 0x25)
			|| !push_dwordxcode(xcode, 0x3ff)
			|| !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
			return 0;
		break;
	}
	return 1;
}

static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

	// PUSH EBX
	if (!push_bytexcode(xcode, 0x53))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	}
	else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	// MOV EBX, EAX
	if (!push_2bytesxcode(xcode, 0x89, 0xc3))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	}
	else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	switch (xcode_rand(xcode) % 6) {
	case 0:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
	case 4:
		// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
	case 5:
		// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8)
			|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
	case 1:
		// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f, 0xaf, 0xc3))
			return 0;
		break;
	case 2:
		// PUSH ECX
		// MOV ECX, EBX
		// AND ECX, 0F
		// SHL EAX, CL
		// POP ECX
		if (!push_bytexcode(xcode, 0x51)
			|| !push_2bytesxcode(xcode, 0x89, 0xd9)
			|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
			|| !push_2bytesxcode(xcode, 0xd3, 0xe0)
			|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
	case 3:
		// PUSH ECX
		// MOV ECX, EBX
		// AND ECX, 0F
		// SHR EAX, CL
		// POP ECX
		if (!push_bytexcode(xcode, 0x51)
			|| !push_2bytesxcode(xcode, 0x89, 0xd9)
			|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
			|| !push_2bytesxcode(xcode, 0xd3, 0xe8)
			|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

const struct cxdec_callback dec_callback = {
	"",
	{ ONI_GOKKOKEY1, ONI_GOKKOKEY2 },
	xcode_building_stage1
};

static struct cxdec {
	BYTE *xcode;			// 容纳128个解密函数，每个函数100字节
	void *address_list[128];// 128个解密函数的地址(用index索引)
	DWORD current_count;		// 当前有效的解密函数的个数
	DWORD index_list[100];	// 记录有效的index编号
	int init_flag;
} cxdec;

static int cxdec_init(void)
{
	cxdec.xcode = (BYTE *)VirtualAlloc(NULL, 128 * 100, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!cxdec.xcode)
		return S_FALSE;

	memset(cxdec.address_list, 0, sizeof(cxdec.address_list));
	cxdec.current_count = 0;
	memset(cxdec.index_list, -1, sizeof(cxdec.index_list));
	cxdec.init_flag = 1;

	return 0;
}

//---------------------------------------------------------------------------
static int xcode_building_start(struct cxdec_xcode_status *xcode, int stage)
{
	// PUSH EDI, PUSH ESI, PUSH EBX, PUSH ECX, PUSH EDX
	if (!push_5bytesxcode(xcode, 0x57, 0x56, 0x53, 0x51, 0x52))
		return 0;

	// MOV EDI, DWORD PTR SS:[ESP+18] (load parameter0)
	if (!push_4bytesxcode(xcode, 0x8b, 0x7c, 0x24, 0x18))
		return 0;

	if (!xcode_building_stage1(xcode, stage))
		return 0;

	// POP EDX, POP ECX, POP EBX, POP ESI, POP EDI
	if (!push_5bytesxcode(xcode, 0x5a, 0x59, 0x5b, 0x5e, 0x5f))
		return 0;

	// RETN
	return push_bytexcode(xcode, 0xc3);
}
//---------------------------------------------------------------------------
static int xcode_building(const cxdec_callback *callback, DWORD seed, void *start, DWORD size)
{
	struct cxdec_xcode_status xcode;
	int stage;
	xcode.start = (BYTE *)start;
	xcode.curr = (BYTE *)start;
	xcode.space_size = size;
	xcode.seed = seed;
	xcode.xcode_building = callback->xcode_building;

	// @1E017A90
	for (stage = 5; stage > 0; --stage) {
		if (xcode_building_start(&xcode, stage))
			break;
		xcode.curr = (BYTE *)start;
	}
	if (!stage) {
		//TVPThrowExceptionMessage(TJS_W("Insufficient code space"));
		Ps::ExitProcess(0);
		return S_FALSE;
	}

	FlushInstructionCache(GetCurrentProcess(), start, size);
	return 1;
}
//---------------------------------------------------------------------------
static void cxdec_execute_xcode(const cxdec_callback *callback, DWORD hash, DWORD *ret1, DWORD *ret2)
{
	DWORD index = hash & 0x7f;
	hash >>= 7;

	if (!cxdec.init_flag)
		cxdec_init();

	if (!cxdec.address_list[index]) {
		if (cxdec.index_list[cxdec.current_count] != index) {
			if (cxdec.index_list[cxdec.current_count] != -1)
				cxdec.address_list[cxdec.index_list[cxdec.current_count]] = 0;
			xcode_building(callback, index, cxdec.xcode + cxdec.current_count * 128, 128);
			cxdec.address_list[index] = cxdec.xcode + cxdec.current_count * 128;
			cxdec.index_list[cxdec.current_count++] = index;
			if (cxdec.current_count >= 100)
				cxdec.current_count = 0;
		}
	}

	*ret1 = (*(DWORD(*)(DWORD))cxdec.address_list[index])(hash);
	*ret2 = (*(DWORD(*)(DWORD))cxdec.address_list[index])(~hash);
}
//---------------------------------------------------------------------------
static void __cxdec_decode(const cxdec_callback *callback, DWORD hash, DWORD offset, BYTE *buf, DWORD len)
{
	BYTE key[12];
	DWORD ret[2], i, k, *dbuf;

	cxdec_execute_xcode(callback, hash, &ret[0], &ret[1]);

	key[8] = ret[0] >> 8;
	key[9] = ret[0] >> 16;
	key[10] = ret[0];
	DWORD key1 = ret[1] >> 16;
	DWORD key2 = ret[1] & 0xffff;
	*(DWORD *)&key[0] = key1;

	if (key1 == key2)
		++key2;

	*(DWORD *)&key[4] = key2;

	if (!key[10])
		key[10] = 1;

	if ((key2 >= offset) && (key2 < offset + len))
		buf[key2 - offset] ^= key[9];

	if ((key1 >= offset) && (key1 < offset + len))
		buf[key1 - offset] ^= key[8];

	if (len>16)
	{
		k = 0x1010101 * key[10];
		for (i = 0; i < ((DWORD)buf & 3); i++) buf[i] ^= key[10];
		len -= i;
		dbuf = (DWORD*)(buf + i);
		for (i = 0; i < len >> 2; i++) *(dbuf++) ^= k;
		buf = (BYTE*)dbuf;
		for (i = 0; i < (len & 3); i++) *buf++ ^= key[10];
	}
	else
		for (i = 0; i < len; ++i)
			buf[i] ^= key[10];
}
//---------------------------------------------------------------------------
static void cxdec_decode(const cxdec_callback *callback, DWORD hash, DWORD offset, BYTE *buf, DWORD len)
{
	DWORD bondary = (hash & callback->key[0]) + callback->key[1];
	DWORD dec_len = 0;

	if (offset < bondary) {
		if (offset + len > bondary)
			dec_len = bondary - offset;
		else
			dec_len = len;
		__cxdec_decode(callback, hash, offset, buf, dec_len);
		offset += dec_len;
		buf += dec_len;
		dec_len = len - dec_len;
	}
	else
		dec_len = len;

	if (dec_len)
		__cxdec_decode(callback, (hash >> 16) ^ hash, offset, buf, dec_len);
}

Void NTAPI CxdecDecoder(DWORD hash, DWORD offset, PBYTE buffer, DWORD bufferlen)
{
	cxdec_decode(&dec_callback, hash, offset, (unsigned char*)buffer, bufferlen);
}
