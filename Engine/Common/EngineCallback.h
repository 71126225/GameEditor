#ifndef _ENGINE_CALLBACK_H_
#define _ENGINE_CALLBACK_H_

class TextureAnimWindow;
class VideoWindow;
// isBreak��ʾ�Ƿ����ж�������֡�Ĳ��Ŷ����µĵ��ûص�,��ͬ
typedef void(*TextureAnimPlayEndCallback)(TextureAnimWindow* window, void* userData, const bool& isBreak);
// isBreak��ʾ�Ƿ����ж�����Ƶ�Ĳ��Ŷ����µĵ��ûص�,��ͬ
typedef void(*VideoPlayEndCallback)(VideoWindow* window, void* userData, const bool& isBreak);

// ¼�����Ļص�,�ڽ��յ�¼������ʱ����
class txWavRecorder;
typedef void(*RecordCallback)(txWavRecorder* recorder, void* userData, char* data, const int& dataSize);

// �����ؼ�֡�Ļص�����,curAnimTimeΪ���ŵ���֡��,���ǲ��ŵ�����,cutAnimΪtrue��ʾ����Ϊ�������л��˶����õĸûص�����
typedef void(*AnimKeyFrameCallback) (void* user_data, const int& aditionalParam, const std::string& animName, const std::string& entityName, const float& curAnimTime, const float& animSpeed, const bool& cutAnim);

// ��Ϊ�ص�
class txAction;
typedef void(*ActionCallback)(txAction* action, void* userData);

// ����ص�
class txComponentKeyFrame;
typedef void(*KeyFrameCallback)(txComponentKeyFrame* component, void* userData, const bool& breakTremling, const bool& done);

// �����¼��ص�
class txTexture;
class Window;
typedef void(*WindowEventCallback)(Window* window, void* userData);

// �����첽���ػص�
class Layout;
typedef void(*LayoutAsyncDone)(Layout* layout, void* userData);

// �̻߳ص�
typedef bool(*CustomThreadCallback)(void* args);

#endif