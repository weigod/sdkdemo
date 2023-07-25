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

  // 打包业务消息, 绑定流名和主播端图层名
  nlohmann::json BuildUpdateAnchorLayer();
  // 打包业务消息, 透传消息至小程序(小程序对应的hyExt.exe.onGameMessage可监听获取到该消息)
  nlohmann::json BuildSendMessageToApplet(const std::string& msg);
  // 发送业务消息
  void SendBizMsg(const std::string& eventName, const nlohmann::json& event);
  // 用于解析业务通道消息
  int ParseMessage(std::string jsonStr);

public:
  void CreateGameWindow();
  void GetWindowPixels();
  void DumpImage(int32_t width, int32_t height, const char* buf, int32_t bufSize);

private:
  uint32_t m_thrifPort {0};
  uint32_t m_thrifListenPort {0};
  std::string m_jobId;
  std::unique_ptr<std::thread> m_gameRenderThread;
  std::atomic<bool> m_exitFlag;
  std::string m_streamName;//自定义一个流名，保持唯一性
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