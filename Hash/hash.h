#ifndef HASH_H
#define HASH_H

#include <opencv2/opencv.hpp>
#include "Merkle_Tree/merkle_tree.h"
#include <string>

void make_hash(const cv::Mat& edge_result, std::string& hash_result);

#endif // HASH_H
