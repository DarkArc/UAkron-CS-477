#pragma once

#include "matrix.h"

#include <stdexcept>
#include <regex>

// This is really dirty, but on newer versions of libpng, int_p_NULL
// is not defined and it is needed here for gil
#define int_p_NULL (int*)NULL

#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <boost/gil/image_view_factory.hpp>

inline matrix convert_img(const boost::gil::gray8_image_t& img) {
  matrix x;
  x.create(img.height(), img.width());

  auto view = boost::gil::const_view(img);
  auto imgData = x.data;

  int curPos = 0;

  for (int y = 0; y < view.height(); ++y) {
    auto x_itr = view.row_begin(y);

    for (int x = 0; x < view.width(); ++x) {
      imgData[curPos++] = x_itr[x];
    }
  }

  return x;
}

inline matrix load_image(const std::string& path) {

  std::regex pngRegex(".*\\.png$");
  std::regex jpegRegex(".*\\.jpg$");

  if (std::regex_match(path, pngRegex)) {
    boost::gil::gray8_image_t input;
    boost::gil::jpeg_read_image(path, input);

    return convert_img(input);
  }

  if (std::regex_match(path, jpegRegex)) {
    boost::gil::gray8_image_t input;
    boost::gil::png_read_image(path, input);

    return convert_img(input);
  }

  throw std::runtime_error("Illegal image format");
}

inline void save_png(const matrix& x, const std::string & path) {
  throw std::runtime_error("Not yet implemented.");
}
