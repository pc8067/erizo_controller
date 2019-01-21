#ifndef ERIZO_CONTROLLER_H
#define ERIZO_CONTROLLER_H

#include <thread>
#include <memory>
#include <atomic>
#include <functional>

#include <json/json.h>

#include "websocket/socket_io_server.h"
#include "redis/redis_helper.h"
#include "rabbitmq/amqp_rpc.h"
#include "rabbitmq/amqp_recv.h"
#include "model/client.h"
#include "common/config.h"

class ErizoController
{
  DECLARE_LOGGER();

public:
  ErizoController();
  ~ErizoController();

  int init();
  void close();

private:
  void notifyToSubscribe(const std::string &client_id, const std::string &stream_id);

  void asyncTask(const std::function<void()> &func);

  int allocAgent(Client &client);

  int allocErizo(Client &client);

  int addPublisher(const std::string &erizo_id,
                   const std::string &client_id,
                   const std::string &stream_id,
                   const std::string &label);

  void removePublisher(const std::string &erizo_id,
                       const std::string &client_id,
                       const std::string &stream_id);

  int addVirtualPublisher(const BridgeStream &bridge_stream);

  int addSubscriber(const std::string &erizo_id,
                    const std::string &client_id,
                    const std::string &stream_id,
                    const std::string &label);

  void removeSubscriber(const std::string &erizo_id,
                        const std::string &client_id,
                        const std::string &stream_id);

  int addVirtualSubscriber(const BridgeStream &bridge_stream);

  void processSignaling(const std::string &erizo_id,
                        const std::string &client_id,
                        const std::string &stream_id,
                        const Json::Value &msg);

  // void createBridgeStream(const std::string &sender_erizo_id,
  //                         const std::string &sender_agent_id,
  //                         const std::string &recver_erizo_id,
  //                         const std::string &recver_agent_id,
  //                         const std::string &stream_id);

  void onSignalingMessage(const std::string &msg);

  std::string onMessage(SocketIOClientHandler *hdl, const std::string &msg);

  void onClose(SocketIOClientHandler *hdl);

  Json::Value handleToken(Client &client, const Json::Value &root);

  Json::Value handlePublish(Client &client, const Json::Value &root);

  Json::Value handleSubscribe(Client &client, const Json::Value &root);

  void handleSignaling(Client &client, const Json::Value &root);

private:
  std::shared_ptr<RedisHelper> redis_;
  std::shared_ptr<SocketIOServer> socket_io_;

  std::shared_ptr<AMQPRPC> amqp_;
  std::shared_ptr<AMQPRecv> amqp_signaling_;
  std::unique_ptr<erizo::ThreadPool> thread_pool_;
  bool init_;
};

#endif
