#include "ncbind/ncbind.hpp"
#include "sqlite3.h"

//
// SQLg£
// 
// cnt(a,b)   ¶ñ a É b ªÜÜêêÎ^
// ncnt(a,b)  ³K»³ê½¶ñ a É ³K»³ê½¶ñ b ªÜÜêÄêÎ^

static const tjs_char *normalizeBefore = 
L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
L"`abcdefghijklmnopqrstuvwxy"
L""
L"PQRSTUVWXO"
L" ¢¤¦¨©«­¯±³µ·¹»½¿ÂÄÆÈÉÊËÌ"
L"ÍÐÓÖÙÜÝÞßàâäæçèéêëíîïðñ¡£¥§Ááãå"
L"ª¬®°²´¶¸º¼¾ÀÃÅÇÎÑÔ×ÚÏÒÕØÛ"
L"ACEGIJLNPRTVXZ\^`cegijklm"
L"nqtwz}~@BDFHb"
L"KMOQSUWY[]_adfhorux{psvy|"
L"§¨©ª«¯¬­®"
L"[EAB°"
L"[]{}"
L"CDFGHILMOPQZ[\]^_`"
L"befghijklmnopqrstuvwxyz{|~"
L"¡£¥¦";

static const tjs_char *normalizeAfter = 
L"abcdefghijklmnopqrstuvwxyz"
L"abcdefghijklmnopqrstuvwxyz"
L"abcdefghijklmnopqrstuvwxyz"
L"1234567890"
L"±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉ"
L"ÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜ²´¦Ý±²³´µÂÔÕÖ"
L"¶·¸¹º»¼½¾¿ÀÁÂÃÄÊËÌÍÎÊËÌÍÎ"
L"±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉ"
L"ÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜ²´¦Ý±²³´µÂÔÕÖ"
L"¶·¸¹º»¼½¾¿ÀÁÂÃÄÊËÌÍÎÊËÌÍÎ"
L"±²³´µÂÔÕÖ"
L"-¥,.-"
L"()()"
L",.:;?!'`^~_ü---/_-"
L"|`'\"\"()()()()()()¢£¢£()+-x="
L"<>\\$%#&*@ ¢¤*";

// Á¶
static const tjs_char *clearChar = L"ÞJK";

// ³K»e[u
static tjs_char normalizeData[65536];

static void
normalize(ttstr &store, const tjs_char *str)
{
	while (*str) {
		store += normalizeData[*str++];
	}
}

void
initNormalize()
{
	// ³K»pf[^Ìú»
	for (int i=0;i<65536;i++) {
		normalizeData[i] = i;
	}
	const tjs_char *p = normalizeBefore;
	const tjs_char *q = normalizeAfter;
	while (*p) {
		normalizeData[*p++] = *q++;
	}
	// Á¶
	p = clearChar;
	while (*p) {
		normalizeData[*p++] = 0;
	}
}

static void
cntFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	if (argc < 2) return;
	sqlite3_result_int(context, wcsstr((const tjs_char*)sqlite3_value_text16(argv[0]),
									   (const tjs_char*)sqlite3_value_text16(argv[1])) != NULL);
}

static void
ncntFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	if (argc < 2) return;
	ttstr a,b;
	normalize(a, (const tjs_char*)sqlite3_value_text16(argv[0]));
	normalize(b, (const tjs_char*)sqlite3_value_text16(argv[1]));
	sqlite3_result_int(context, wcsstr(a.c_str(), b.c_str()) != NULL);
}

void
initContainFunc(sqlite3 *db)
{
	// ärÖÌo^
	sqlite3_create_function(db, "cnt",  2, SQLITE_ANY, NULL, cntFunc,  NULL, NULL);
	sqlite3_create_function(db, "ncnt", 2, SQLITE_ANY, NULL, ncntFunc, NULL, NULL);
}
