#include "EditorUtility.h"
#include "EditorFrame.h"
#include "OKDialog.h"
#include "YesNoDialog.h"

void EditorUtility::logError(const std::string& info)
{
	OKDialog* ok = new OKDialog("����");
	ok->setInfo(info);
	ok->ShowModal();
	delete ok;
}

void EditorUtility::logInfo(const std::string& info)
{
	OKDialog* ok = new OKDialog("��ʾ");
	ok->setInfo(info);
	ok->ShowModal();
	delete ok;
}

int EditorUtility::askYesNODialog(const std::string& info)
{
	YesNODialog* yesNO = new YesNODialog("��ʾ");
	yesNO->setInfo(info);
	int ret = yesNO->ShowModal();
	delete yesNO;
	return ret;
}