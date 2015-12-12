
#include "stdafx.h"

#include "server.h"
#include <string>
#include <regex>

namespace cs477 {
  namespace async {
    const std::string base_dir = "C:\\Server";

    std::string sanatize(std::string str) {
      // Convert to windows directory pattern
      str = std::regex_replace(str, std::regex("/"), "\\");
      return str;
    }

    void socket_handler(cs477::net::socket sock) {

      // Read an http request
      auto f = cs477::net::read_http_request_async(sock).then([sock](auto f) {
        auto rq = f.get();

        auto file_dir = base_dir + sanatize(rq.url);

        try {
          cs477::read_file_async(file_dir.c_str()).then([sock](auto f) {
            auto docBody = f.get();

            // Write the response (successfully, w/ body)
            auto rsp = make_response(200, docBody);
            cs477::net::write_http_response_async(sock, rsp);
          });
        } catch (...) {
          // Write the response (not found/something bad happened)
          auto rsp = make_response(404, {});
          cs477::net::write_http_response_async(sock, rsp);
        }

        return 0;
      });
    }


    void run(const sockaddr_in &addr) {
      auto host = std::make_shared<cs477::net::acceptor>();
      host->listen(addr);

      for (int i = 0; i < 32; i++) {
        host->accept_async(socket_handler);
      }

      // Just wait forever.
      cs477::promise<void> p;
      auto f = p.get_future();
      f.wait();
    }
  }
}
