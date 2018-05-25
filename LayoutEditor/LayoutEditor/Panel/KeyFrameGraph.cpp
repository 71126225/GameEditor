#include "Utility.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"

#include "TremblingEditorCore.h"

#include "KeyFrameGraph.h"

enum
{
	ID_MENU_INSERT_KEY_FRAME,
	ID_MENU_DELETE_KEY_FRAME,
};

BEGIN_EVENT_TABLE(KeyFrameGraph, GraphPanelBase)
EVT_MENU(ID_MENU_INSERT_KEY_FRAME, KeyFrameGraph::OnInsertKeyFrame)
EVT_MENU(ID_MENU_DELETE_KEY_FRAME, KeyFrameGraph::OnDeleteKeyFrame)
END_EVENT_TABLE()

const int BLOCK_WIDTH = 10;
const int BLOCK_HEIGHT = 10;
const float MAX_UNIT_PIXELS = 1000.0f;
const float MIN_UNIT_PIXELS = 0.1f;
const int LEVEL_COUNT = 17;
const float LEVEL[LEVEL_COUNT] = { MIN_UNIT_PIXELS, 0.2f, 0.4f, 0.8f, 1.0f, 2.0f, 4.0f, 8.0f, 10.0f, 20.0f, 40.0f, 80.0f, 100.0f, 200.0f, 400.0f, 800.0f, MAX_UNIT_PIXELS };

KeyFrameGraph::KeyFrameGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	GraphPanelBase(parent, position, size, style, name),
	mPixelPerUnitHori(100.0f),
	mPixelPerUnitVert(20.0f),
	mHoriStart(30),
	mVertStart(230),
	mIsShiftDown(false)
{}

void KeyFrameGraph::notifyMouseWheel(wxMouseEvent& event)
{
	float delta = event.GetWheelRotation();
	// ��סshiftʱ�����м���������
	if (mIsShiftDown)
	{
		float curRelative = mVertStart - mSize.y / 2;
		if (delta > 0)
		{
			curRelative *= 1.1f;
			mPixelPerUnitVert *= 1.1f;
			if (mPixelPerUnitVert > MAX_UNIT_PIXELS)
			{
				curRelative /= 1.1f;
				mPixelPerUnitVert = MAX_UNIT_PIXELS;
			}
		}
		else
		{
			curRelative /= 1.1f;
			mPixelPerUnitVert /= 1.1f;
			if (mPixelPerUnitVert < MIN_UNIT_PIXELS)
			{
				curRelative *= 1.1f;
				mPixelPerUnitVert = MIN_UNIT_PIXELS;
			}
		}
		mVertStart = (int)(curRelative + mSize.y / 2);
	}
	// ����shiftʱ�����м����ź���
	else
	{
		float curRelative = mHoriStart - mSize.x / 2;
		if (delta > 0)
		{
			curRelative *= 1.1f;
			mPixelPerUnitHori *= 1.1f;
			if (mPixelPerUnitHori > MAX_UNIT_PIXELS)
			{
				curRelative /= 1.1f;
				mPixelPerUnitHori = MAX_UNIT_PIXELS;
			}
		}
		else
		{
			curRelative /= 1.1f;
			mPixelPerUnitHori /= 1.1f;
			if (mPixelPerUnitHori < MIN_UNIT_PIXELS)
			{
				curRelative *= 1.1f;
				mPixelPerUnitHori = MIN_UNIT_PIXELS;
			}
		}
		mHoriStart = (int)(curRelative + mSize.x / 2);
	}
}

void KeyFrameGraph::notifyMouseLeftUp(wxMouseEvent& event)
{
	txTremblingKeyFrame* keyFrame = hitKeyFrame(wxPoint(event.GetPosition().x, transPosY(event.GetPosition().y)), mMouseOffset);
	if (keyFrame != NULL)
	{
		mTremblingEditorCore->selectKeyFrame(keyFrame->getIndexInParent());
	}
	else
	{
		mTremblingEditorCore->selectKeyFrame(-1);
	}
}

void KeyFrameGraph::notifyMouseRightUp(wxMouseEvent& event)
{
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node == NULL)
	{
		return;
	}
	mMenuPosition = event.GetPosition();
	mMenuPosition = wxPoint(mMenuPosition.x, transPosY(mMenuPosition.y));
	wxMenu menu;
	menu.Append(ID_MENU_INSERT_KEY_FRAME, wxT("����ؼ�֡"));
	if (hitKeyFrame(mMenuPosition, wxPoint()) != NULL)
	{
		menu.Append(ID_MENU_DELETE_KEY_FRAME, wxT("ɾ���ؼ�֡"));
	}
	PopupMenu(&menu, event.GetPosition());// ��ʾ�����˵�  
	event.Skip();
}

void KeyFrameGraph::notifyMiddleDown(wxMouseEvent& event)
{
	wxPoint curPos = event.GetPosition();
	curPos.y = transPosY(curPos.y);
	mMiddlePosOffset = wxPoint(mHoriStart, mVertStart) - curPos;
}

void KeyFrameGraph::notifyMouseMove(wxMouseEvent& event)
{
	if (mTremblingEditorCore->getCurKeyFrame() != NULL && event.LeftIsDown())
	{
		wxPoint newPoint = event.GetPosition();
		newPoint.y = transPosY(newPoint.y);
		newPoint -= mMouseOffset;
		float time = positionXToTime(newPoint.x);
		float offset = positionYToOffset(newPoint.y);
		mTremblingEditorCore->setKeyFrameTime(time);
		mTremblingEditorCore->setKeyFrameValue(offset);
	}
	// ��ס�м������ƶ���������ϵ
	else if (event.MiddleIsDown())
	{
		wxPoint curPos = event.GetPosition();
		curPos.y = transPosY(curPos.y);
		wxPoint start = curPos + mMiddlePosOffset;
		mHoriStart = start.x;
		mVertStart = start.y;
	}
}

void KeyFrameGraph::notifyKeyDown(wxKeyEvent& event)
{
	unsigned int key = event.GetRawKeyCode();
	if (key == VK_SHIFT)
	{
		mIsShiftDown = true;
	}
	// ��O�����Իָ�����ϵ���ƶ�
	else if (key == 'O')
	{
		mHoriStart = 30.0f;
		mVertStart = mSize.y / 2.0f;
	}
}

void KeyFrameGraph::notifyKeyUp(wxKeyEvent& event)
{
	if (event.GetRawKeyCode() == VK_SHIFT)
	{
		mIsShiftDown = false;
	}
}

void KeyFrameGraph::OnInsertKeyFrame(wxCommandEvent& event)
{
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node != NULL)
	{
		float time = positionXToTime(mMenuPosition.x);
		float offset = positionYToOffset(mMenuPosition.y);
		mTremblingEditorCore->addKeyFrame(node->findIndex(time) + 1, time, offset);
	}
}

void KeyFrameGraph::OnDeleteKeyFrame(wxCommandEvent& event)
{
	txTremblingKeyFrame* keyFrame = hitKeyFrame(mMenuPosition, wxPoint());
	if (keyFrame != NULL)
	{
		mTremblingEditorCore->deleteKeyFrame(keyFrame->getIndexInParent());
	}
}

void KeyFrameGraph::render(wxDC* dc)
{
	drawCoord(dc);
	drawKeyFrame(dc);
}

void KeyFrameGraph::drawKeyFrame(wxDC* dc)
{
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node != NULL)
	{
		// �ؼ�֡����
		int lastKeyPosX = 0;
		int lastKeyPosY = 0;
		txVector<txTremblingKeyFrame*>& keyFrameList = node->getKeyFrameList();
		int keyFrameCount = keyFrameList.size();
		FOR_STL(keyFrameList, int i = 0; i < keyFrameCount; ++i)
		{
			int keyFramePosX = timeToPositionX(keyFrameList[i]->getTime());
			int keyFramePosY = offsetToPositionY(keyFrameList[i]->getOffset());
			// ������ǵ�һ���ؼ�֡,���������һ�ؼ�֡������
			if (i > 0)
			{
				drawLine(dc, lastKeyPosX, lastKeyPosY, keyFramePosX, keyFramePosY, VECTOR3(50.0f, 255.0f, 0.0f));
			}
			lastKeyPosX = keyFramePosX;
			lastKeyPosY = keyFramePosY;
		}
		END_FOR_STL(keyFrameList);
		// �ؼ�֡����
		FOR_STL(keyFrameList, int i = 0; i < keyFrameCount; ++i)
		{
			int keyFramePosX = timeToPositionX(keyFrameList[i]->getTime());
			int keyFramePosY = offsetToPositionY(keyFrameList[i]->getOffset());
			int posX = keyFramePosX - BLOCK_WIDTH / 2;
			int posY = keyFramePosY + BLOCK_HEIGHT / 2;
			if (mTremblingEditorCore->getCurKeyFrame() != keyFrameList[i])
			{
				drawSolidRect(dc, posX, posY, BLOCK_WIDTH, BLOCK_HEIGHT, VECTOR3(0.0f, 255.0f, 0.0f));
			}
			else
			{
				drawSolidRect(dc, posX, posY, BLOCK_WIDTH, BLOCK_HEIGHT, VECTOR3(255.0f, 0.0f, 0.0f));
			}
		}
		END_FOR_STL(keyFrameList);
	}
}

void KeyFrameGraph::drawCoord(wxDC* dc)
{
	// ����
	int vertStart = mVertStart;
	txMath::clamp(vertStart, 10, (int)mSize.y - 5);
	drawLine(dc, mHoriStart, vertStart, mSize.x, vertStart, VECTOR3(100.0f, 0.0f, 0.0f));
	for (int i = 0; i < LEVEL_COUNT; ++i)
	{
		if (mPixelPerUnitHori <= LEVEL[i])
		{
			float tempUnit = mPixelPerUnitHori * (100.0f / LEVEL[i]);
			int horiScaleCount = (mSize.x - mHoriStart) / tempUnit + 1;
			for (int j = 0; j < horiScaleCount; ++j)
			{
				std::string text = txStringUtility::floatToString(j * 100.0f / LEVEL[i]);
				wxSize textSize = dc->GetTextExtent(text);
				drawText(dc, text, mHoriStart + j * tempUnit - textSize.x, vertStart, VECTOR3(100.0f, 0.0f, 0.0f));
				if (j > 0)
				{
					// ��������
					drawLine(dc, mHoriStart + j * tempUnit, 0, mHoriStart + j * tempUnit, mSize.y, VECTOR3(0.0f, 0.0f, 255.0f));
					// �̶���
					drawLine(dc, mHoriStart + j * tempUnit, vertStart, mHoriStart + j * tempUnit, vertStart - 5, VECTOR3(100.0f, 0.0f, 0.0f));
				}
			}
			break;
		}
	}

	// ����
	int horiStart = mHoriStart;
	txMath::clamp(horiStart, 25, (int)mSize.x - 2);
	drawLine(dc, horiStart, 0, horiStart, mSize.y, VECTOR3(100.0f, 0.0f, 0.0f));
	for (int i = 0; i < LEVEL_COUNT; ++i)
	{
		if (mPixelPerUnitVert <= LEVEL[i])
		{
			// �ϰ벿��
			float tempUnit = mPixelPerUnitVert * (100.0f / LEVEL[i]);
			int vertScaleCount0 = (mSize.y - mVertStart) / tempUnit + 1;
			for (int j = 0; j < vertScaleCount0; ++j)
			{
				if (j > 0)
				{
					std::string text = txStringUtility::floatToString(j * 100.0f / LEVEL[i]);
					wxSize textSize = dc->GetTextExtent(text);
					drawText(dc, text, horiStart - textSize.x - 5, mVertStart + j * tempUnit + textSize.y / 2, VECTOR3(100.0f, 0.0f, 0.0f));
					// ���Ӻ���
					drawLine(dc, horiStart, mVertStart + j * tempUnit, mSize.x, mVertStart + j * tempUnit, VECTOR3(0.0f, 0.0f, 255.0f));
					// �̶���
					drawLine(dc, horiStart - 5, mVertStart + j * tempUnit, horiStart, mVertStart + j * tempUnit, VECTOR3(100.0f, 0.0f, 0.0f));
				}
			}
			// �°벿��
			int vertScaleCount1 = mVertStart / tempUnit + 1;
			for (int j = 0; j < vertScaleCount1; ++j)
			{
				if (j > 0)
				{
					std::string text = txStringUtility::floatToString(-j * 100.0f / LEVEL[i]);
					wxSize textSize = dc->GetTextExtent(text);
					drawText(dc, text, horiStart - textSize.x - 5, mVertStart - j * tempUnit + textSize.y / 2, VECTOR3(100.0f, 0.0f, 0.0f));
					// ���Ӻ���
					drawLine(dc, horiStart, mVertStart - j * tempUnit, mSize.x, mVertStart - j * tempUnit, VECTOR3(0.0f, 0.0f, 255.0f));
					// �̶���
					drawLine(dc, horiStart - 5, mVertStart - j * tempUnit, horiStart, mVertStart - j * tempUnit, VECTOR3(100.0f, 0.0f, 0.0f));
				}
			}
			break;
		}
	}
}

txTremblingKeyFrame* KeyFrameGraph::hitKeyFrame(const wxPoint& point, wxPoint& offset)
{ 
	txTremblingNode* node = mTremblingEditorCore->getCurTremblingNode();
	if (node == NULL)
	{
		return NULL;
	}
	wxRect blockRect;
	blockRect.width = BLOCK_WIDTH;
	blockRect.height = BLOCK_HEIGHT;
	txVector<txTremblingKeyFrame*>& keyFrameList = node->getKeyFrameList();
	int keyFrameCount = keyFrameList.size();
	FOR_STL(keyFrameList, int i = 0; i < keyFrameCount; ++i)
	{
		blockRect.x = timeToPositionX(keyFrameList[i]->getTime()) - BLOCK_WIDTH / 2;
		blockRect.y = offsetToPositionY(keyFrameList[i]->getOffset()) - BLOCK_HEIGHT / 2;
		if (blockRect.Contains(point))
		{
			offset.x = point.x - (blockRect.x + BLOCK_WIDTH / 2);
			offset.y = point.y - (blockRect.y + BLOCK_HEIGHT / 2);
			END_FOR_STL(keyFrameList);
			return keyFrameList[i];
		}
	}
	END_FOR_STL(keyFrameList);
	return NULL;
}