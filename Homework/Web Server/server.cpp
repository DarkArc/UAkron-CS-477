// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "server.h"

#include <unordered_map>

int main() {
	cs477::net::initialize();

	auto addr = cs477::net::resolve_address("localhost", 8080);
	cs477::async::run(addr);
	cs477::net::finalize();
	return 0;
}

cs477::net::http_response make_response(int status, const std::string &file_type, const std::string &body) {
  std::unordered_map<std::string, std::string> typeMap{
    {"html", "text/html"},
    {"css", "text/css"},
    {"js", "text/javascript"},
    {"xml", "text/xml"},
    {"png", "image/png"},
    {"gif", "image/gif"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"ico", "image/x-icon"}
  };
  
  // If we don't know the file's MIME type, consider it a text file
  auto content_type = typeMap.find(file_type);
  std::string type_str = "text/plain";
  if (content_type != typeMap.end()) {
    type_str = content_type->second;
  }
  
  cs477::net::http_response rsp;
	rsp.status = status;
	switch (status) {
	  case 200:
		  rsp.message = "Ok";
		  break;
	  case 404:
		  rsp.message = "Not Found";
		  break;
	  case 500:
		  rsp.message = "Internal Error";
		  break;
	}

	if (status == 200) {
		if (body.length()) {
			rsp.body = body;
			rsp.headers.emplace_back("Content-Type", type_str);
		}
	}

	return rsp;
}

