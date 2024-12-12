﻿#include "qtcommon.h"
#include "extension.h"
#include "blockmarkup.h"
#include "network.h"
#include <map>
#include <fstream>
#include <QTimer>

extern const char* NATIVE_LANGUAGE;
extern const char* SELECT_LANGUAGE;
extern const char* SELECT_LANGUAGE_MESSAGE;
extern const char* LANGUAGE_SAVED;
extern const wchar_t* TOO_MANY_TRANS_REQUESTS;

extern const char* TRANSLATION_PROVIDER;
extern QStringList languages;
std::pair<bool, std::wstring> Translate(const std::wstring& text);

const char* LANGUAGE = u8"Language";
const std::string TRANSLATION_CACHE_FILE = FormatString("%sCache.txt", TRANSLATION_PROVIDER);

Synchronized<std::wstring> translateTo = L"en";

Synchronized<std::map<std::wstring, std::wstring>> translationCache;
int savedSize;

void SaveCache()
{
	std::wstring allTranslations(L"\xfeff");
	for (const auto& [sentence, translation] : translationCache.Acquire().contents)
		allTranslations.append(L"|SENTENCE|").append(sentence).append(L"|TRANSLATION|").append(translation).append(L"|END|\r\n");
	std::ofstream(TRANSLATION_CACHE_FILE, std::ios::binary | std::ios::trunc).write((const char*)allTranslations.c_str(), allTranslations.size() * sizeof(wchar_t));
	savedSize = translationCache->size();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		static QSettings settings = openSettings();
		settings.beginGroup(TRANSLATION_PROVIDER);
		if (settings.contains(LANGUAGE)) translateTo->assign(S(settings.value(LANGUAGE).toString()));
		else QTimer::singleShot(0, []
		{
			QString language = QInputDialog::getItem(
				nullptr,
				SELECT_LANGUAGE,
				QString(SELECT_LANGUAGE_MESSAGE).arg(TRANSLATION_PROVIDER),
				languages,
				std::find_if(languages.begin(), languages.end(), [](QString language) { return language.startsWith(NATIVE_LANGUAGE); }) - languages.begin(),
				false,
				nullptr,
				Qt::WindowCloseButtonHint
			);
			translateTo->assign(S(language.split(": ")[1]));
			settings.setValue(LANGUAGE, S(translateTo->c_str()));
			QMessageBox::information(nullptr, SELECT_LANGUAGE, QString(LANGUAGE_SAVED).arg(CONFIG_FILE));
		});

		std::ifstream stream(TRANSLATION_CACHE_FILE, std::ios::binary);
		BlockMarkupIterator savedTranslations(stream, Array<std::wstring_view>{ L"|SENTENCE|", L"|TRANSLATION|" });
		auto translationCache = ::translationCache.Acquire();
		while (auto read = savedTranslations.Next())
		{
			auto& [sentence, translation] = read.value();
			translationCache->try_emplace(std::move(sentence), std::move(translation));
		}
		savedSize = translationCache->size();
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		SaveCache();
	}
	break;
	}
	return TRUE;
}

bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["text number"] == 0) return false;

	static class
	{
	public:
		bool Request()
		{
			auto tokens = this->tokens.Acquire();
			tokens->push_back(GetTickCount());
			if (tokens->size() > tokenCount * 5) tokens->erase(tokens->begin(), tokens->begin() + tokenCount * 3);
			tokens->erase(std::remove_if(tokens->begin(), tokens->end(), [this](DWORD token) { return GetTickCount() - token > delay; }), tokens->end());
			return tokens->size() < tokenCount;
		}

	private:
		const int tokenCount = 30, delay = 60 * 1000;
		Synchronized<std::vector<DWORD>> tokens;
	} rateLimiter;

	bool cache = false;
	std::wstring translation;
	{
		auto translationCache = ::translationCache.Acquire();
		auto translationLocation = translationCache->find(sentence);
		if (translationLocation != translationCache->end()) translation = translationLocation->second;
		else if (!(rateLimiter.Request() || sentenceInfo["current select"])) translation = TOO_MANY_TRANS_REQUESTS;
		else std::tie(cache, translation) = Translate(sentence);
		if (cache && sentenceInfo["current select"]) translationCache->try_emplace(translationLocation, sentence, translation);
	}
	if (cache && translationCache->size() > savedSize + 50) SaveCache();

	Unescape(translation);
	sentence += L"\n" + translation;
	return true;
}

TEST(
	assert(Translate(L"こんにちは").second.find(L"ello") != std::wstring::npos)
);
