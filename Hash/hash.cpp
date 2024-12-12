#include <string>
#include <opencv2/opencv.hpp>
#include "Merkle_Tree/merkle_tree.h"

using namespace std;

void make_hash(const cv::Mat& edge_result, string& hash_result) {
    string mat_data;
    for (int i = 0; i < edge_result.rows; ++i) {
        for (int j = 0; j < edge_result.cols; ++j) {
            mat_data += to_string(edge_result.at<uchar>(i, j));
        }
    }
    hash_result = hash_sha256(mat_data);
}
