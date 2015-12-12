
#pragma once

#include <thread>

#include "../../class-common/include/cs477.h"
#include "../../class-common/include/queue.h"
#include "../../class-common/include/net.h"
#include "../../class-common/include/http.h"
#include "../../class-common/include/file.h"

cs477::net::http_response make_response(int status, const std::string &json);

namespace cs477 {
  namespace async {
    void run(const sockaddr_in &addr);
  }
}

