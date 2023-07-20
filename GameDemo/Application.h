#pragma once
#include <memory>
#include <string>
#include <list>
#include <map>
#include "spdlog/spdlog.h"
#include "spdlog/details/os.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/dup_filter_sink.h"
#include <nlohmann/json.hpp>
#include <atomic>
#if __cplusplus >= 201703L
#include <filesystem>
using fs = std::filesystem;
#else
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

struct cpp_raw_video_frame;
struct cpp_pcm_info;

class Application
{
public:
  Application();
  ~Application();

  int Init(int argc, char** argv);
  int UnInit();
  int Run();

  void OnRecvCustomData(const std::string& customData);
  void GameRenderLoop();

  // ���ҵ����Ϣ, ��������������ͼ����
  nlohmann::json BuildUpdateAnchorLayer();
  // ���ҵ����Ϣ, ͸����Ϣ��С����(С�����Ӧ��hyExt.exe.onGameMessage�ɼ�����ȡ������Ϣ)
  nlohmann::json BuildSendMessageToApplet(const std::string& msg);
  // ����ҵ����Ϣ
  void SendBizMsg(const std::string& eventName, const nlohmann::json& event);
  // ���ڽ���ҵ��ͨ����Ϣ
  int ParseMessage(std::string jsonStr);

public:
  void CreateGameWindow();
  void GetWindowPixels();

private:
  uint16_t m_thrifPort { 0 };
  std::string m_jobId;
  std::unique_ptr<std::thread> m_gameRenderThread;
  std::atomic<bool> m_exitFlag;
  std::string m_streamName;//�Զ���һ������������Ψһ��
  int32_t m_resolutionWidth = 1920;
  int32_t m_resolutionHeight = 1080;

  HWND m_hwnd;
  int32_t m_wndClientWidth = 1280;
  int32_t m_wndClientHeight = 720;
  std::string m_videoBuf;
  std::mutex m_videoBufMutex;
  int32_t m_reqId = 1;
};

static std::shared_ptr<spdlog::logger> m_logger;