#include <unistd.h>
#include <sys/fcntl.h>
#include <cassert>
#include <tinyMuduo/HttpRequest.h>
#include <tinyMuduo/HttpResponse.h>
#include <tinyMuduo/HttpServer.h>
#include <tinyMuduo/EventLoop.h>
#include <fstream>
#include <streambuf>  
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "util.h"

using namespace httpparser;

const std::string MODLE_ABS_BASE_DIR = "/mnt/workspace/Parser/";  // fix with env
const std::string INFERENCE_NAME = "test2.py";
const std::string DATA_ABS_BASE_DIR = "/mnt/workspace/Muduo/examples/parsingServer/usrdata/"; // fix with env
const std::string WEB_BASE_DIR = "web/";
const std::vector<std::string> config_args{MODLE_ABS_BASE_DIR + INFERENCE_NAME, "--res_dir", "results_v1", "--visual", "true"};
unordered_map<std::string, std::string> pages;

static void saveStringToFile(const std::string &file_name, const std::string & data) 
{
  std::ofstream file(file_name);
  if (file.is_open()) {
    file << data;
  } else {
    LOG_ERROR("failed to save data into %s", file_name.c_str());
  }
}

static std::string retriveFileAsString(const std::string & file_name)
{
  std::ifstream file(file_name);
  if (!file.is_open()) {
    LOG_ERROR("failed to read file %s", file_name.c_str());
    return "Nothing";
  }
  return string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

static std::string pasringDialog(const std::string id, const std::string &dialog, const std::string &type) {
  LOG_INFO("id = %s, dialog = %s, type %s\n", id.c_str(), dialog.c_str(), type.c_str());
  // prepare args
  std::string dfn(DATA_ABS_BASE_DIR + "usr-data-" + id + ".conll"); // 保存用户提交的语料. 便于后续扩展功能. id为get返回的.
  saveStringToFile(dfn, dialog);                                    
  std::string rfn(DATA_ABS_BASE_DIR + "usr-res-" + id + ".log");    // 保存用户推理的结果. 便于后续扩展功能, 如历史记录等. 
  std::vector<std::string> args(config_args);
  args.push_back("--format"); args.push_back(type);
  args.push_back("--data_file"); args.push_back(dfn);
  args.push_back("--uid_store"); args.push_back(rfn);
  bool finished = ex_py(MODLE_ABS_BASE_DIR, INFERENCE_NAME, args);
  if (!finished)
    return "Inference Failed";
  // uas, las, visual res
  return std::string(retriveFileAsString(rfn));
}

static void handlePost(const HttpRequest& req, HttpResponse* resp) {

  std::string content = req["content"];
  std::string id = req["Id"];
  std::string type = req["Content-Type"]; // json / conll

  std::string parsing_res = pasringDialog(id, content, type);
  resp->setStatusCode(HttpResponse::k200Ok);    
  resp->setStatusMsg("OK");
  resp->addHeader("Access-Control-Allow-Origin", "*");
  resp->addHeader("Access-Control-Allow-Methods", "*");
  resp->setContent(parsing_res);
}

// 处理跨域请求
static void handleOptions(const HttpRequest& req, HttpResponse* resp) 
{
  if(req["Access-Control-Request-Method"] != "NULL") {
    std::cout << req.method << " " <<req["Access-Control-Request-Method"] << " " << req["Access-Control-Request-Headers"] << std::endl;
    resp->setStatusCode(HttpResponse::k200Ok);    
    resp->setStatusMsg("OK");
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Methods", "*");
    resp->addHeader("Access-Control-Allow-Headers", "*");
  } else {
    LOG_INFO("Other Situations");
    resp->setStatusCode(HttpResponse::k200Ok);    
    resp->setStatusMsg("OK");
  }
}

static void handleGet(const HttpRequest& req, HttpResponse* resp)
{
  LOG_INFO("uri %s\n", req.uri.c_str());
  resp->setStatusCode(HttpResponse::k200Ok);    
  resp->setStatusMsg("OK");
  resp->setContent(pages[req.uri]);
}

void respond(const HttpRequest& req, HttpResponse* resp)
{
  std::cout<<"<<req.inspect()>>"<<std::endl<<req.inspect()<<std::endl;
  if (req.method == "OPTIONS") {
    handleOptions(req, resp);
  } else if(req.method == "POST") {
    handlePost(req, resp);
  } else if (req.method == "GET") {
    handleGet(req, resp);
  } else {
    std::cout<<"method "<<req.method<<endl;
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMsg("Not Implemented");
    // resp->setCloseConnection(true);
  }
}


static void sourceLoad() 
{
  pages["/inference"] = retriveFileAsString(WEB_BASE_DIR + "index.html");
  pages["/favicon.ico"] = retriveFileAsString(WEB_BASE_DIR + "ico/favicon.ico");
}

int main(int argc, char* argv[])
{
  sourceLoad();
  EventLoop loop;
  LOG_INFO("listening on port %d", 7000);
  HttpServer hs(&loop, InetAddress(7000, "172.18.32.173"), "HttpServer");
  hs.setHttpCallback(respond);
  hs.setSubThreadNum(1);
  hs.start();
  loop.loop();
}
