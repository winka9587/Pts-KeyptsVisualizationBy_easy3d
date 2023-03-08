/********************************************************************
 * Copyright (C) 2015 Liangliang Nan <liangliang.nan@gmail.com>
 * https://3d.bk.tudelft.nl/liangliang/
 *
 * This file is part of Easy3D. If it is useful in your research/work,
 * I would be grateful if you show your appreciation by citing it:
 * ------------------------------------------------------------------
 *      Liangliang Nan.
 *      Easy3D: a lightweight, easy-to-use, and efficient C++ library
 *      for processing and rendering 3D data.
 *      Journal of Open Source Software, 6(64), 3255, 2021.
 * ------------------------------------------------------------------
 *
 * Easy3D is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 3
 * as published by the Free Software Foundation.
 *
 * Easy3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************/
#include "build/tutorials/Tutorial_201_Viewer_default/viewer.h"
#include <easy3d/renderer/camera.h>
#include <easy3d/viewer/viewer.h>
#include <easy3d/util/resource.h>
#include <easy3d/util/initializer.h>
#include <vector>

#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/renderer/drawable_points.h>

#include <easy3d/core/random.h>
using namespace easy3d;

const float point_size = 8.0f;
const float key_point_size = 30.0f;
//const vec4 bg_color = vec4(255, 255, 255, 0.0); // alpha 透明度
const vec4 bg_color = vec4(0, 0, 0, 0.0); // alpha 透明度


std::string save_folder = "E:/JL/result2/";

// pcds：点云集合
// colors：点的颜色
// is_key：是否是关键点点云, 关键点的点大小使用key_point_size；否则使用point_size
// auto_close 是否自动关闭窗口, 如果为true, 窗口会显示一下然后自动关闭，用于批处理
// 为false则窗口可交互
void viz_pcd(std::vector<PointCloud*>& pcds, std::vector< vec4>& colors, std::vector<bool> is_key, std::string img_save_path, bool auto_close=false) {
    myViewer viewer("visualize keypoints");
    int w = 1920;
    int h = 1080;
    viewer.viewer_size(w, h);
    //viewer.fit_screen();
    auto camera_ = viewer.camera();
    // camera_->setType(Camera::PERSPECTIVE);
    camera_->setType(Camera::ORTHOGRAPHIC);
    
    camera_->setPosition(vec3(0.7, 0.7, 0.7));
    camera_->setUpVector(vec3(0, 1, 0));        // Z pointing up
    camera_->setViewDirection(vec3(-1, -1, -1));  // X pointing out
    viewer.set_background_color(bg_color);

    for (int i = 0; i < pcds.size(); i++) {
        auto pcd = pcds[i];
        auto pcd_color = colors[i];
        auto key_or_not = is_key[i];
        viewer.add_model(pcd);
        PointsDrawable* drawable = pcd->renderer()->get_points_drawable("vertices");
        drawable->set_impostor_type(PointsDrawable::SPHERE); // draw points as spheres.
        drawable->set_uniform_coloring(pcd_color);
        if (key_or_not) {
            drawable->set_point_size(key_point_size);
        }
        else {
            drawable->set_point_size(point_size);
        }
        
    }
    //viewer.update();
    

    if (auto_close) {
        viewer.auto_close();
        viewer.run(false);
        // 批处理时才会保存图像
        viewer.snapshot(img_save_path, false);  // 保存图片, 背景不为透明
        std::cout << "img saved to " << img_save_path << std::endl;
    }
    else {
        // 交互则不保存图像
        viewer.run(false);
    }

    

    return;
}
/*
     auto_close：
          true，批处理，自动关闭窗口
          false：交互
*/
void viz_save_easy3D(std::string obj_path, std::string obj_name, bool auto_close) {
    // initialize Easy3D.
    initialize();
    // load file
    /*std::string obj_name = "11547e8d8f143557525b133235812833";
    std::string obj_path = "E:/JL/11547e8d8f143557525b133235812833/";*/
    std::string deform_path = obj_path + "deformed_pointcloud.ply";
    std::string source_keypoints_path = obj_path + "source_keypoints.ply";
    std::string source_pointcloud_path = obj_path + "source_pointcloud.ply";
    std::string target_keypoints_path = obj_path + "target_keypoints.ply";
    std::string target_pointcloud_path = obj_path + "target_pointcloud.ply";

    // load ply file
    // deform
    PointCloud* deform_pcd = PointCloudIO::load(deform_path);
    vec4 deform_color = vec4(0.6f, 0.6f, 1.0f, 1.0f);

    PointCloud* source_pcd = PointCloudIO::load(source_pointcloud_path);
    vec4 source_pcd_color = vec4(0.6f, 0.6f, 1.0f, 1.0f);

    PointCloud* source_key = PointCloudIO::load(source_keypoints_path);
    vec4 source_key_color = vec4(1.0f, 0.2f, 0.2f, 1.0f);

    PointCloud* target_pcd = PointCloudIO::load(target_pointcloud_path);
    vec4 target_pcd_color = vec4(0.6f, 0.6f, 1.0f, 1.0f);

    PointCloud* target_key = PointCloudIO::load(target_keypoints_path);
    vec4 target_key_color = vec4(1.0f, 0.2f, 0.2f, 1.0f);

    std::vector<PointCloud*> input_pcds;
    std::vector<vec4> input_colors;
    std::vector<bool> is_key;
    std::string img_save_path = "";

    // source pts+key
    input_pcds.clear();
    input_colors.clear();
    is_key.clear();
    input_pcds.push_back(source_pcd);
    input_pcds.push_back(source_key);
    input_colors.push_back(source_pcd_color);
    input_colors.push_back(source_key_color);
    is_key.push_back(false);  // key的点更大
    is_key.push_back(true);
    img_save_path = save_folder + obj_name + "_source.png";
    viz_pcd(input_pcds, input_colors, is_key, img_save_path, auto_close);

    // target pts+key
    input_pcds.clear();
    input_colors.clear();
    is_key.clear();
    input_pcds.push_back(target_pcd);
    input_pcds.push_back(target_key);
    input_colors.push_back(target_pcd_color);
    input_colors.push_back(target_key_color);
    is_key.push_back(false);
    is_key.push_back(true);
    img_save_path = save_folder + obj_name + "_target.png";
    viz_pcd(input_pcds, input_colors, is_key, img_save_path, auto_close);

    // deform
    input_pcds.clear();
    input_colors.clear();
    is_key.clear();
    input_pcds.push_back(deform_pcd);
    input_colors.push_back(deform_color);
    is_key.push_back(false);
    img_save_path = save_folder + obj_name + "_deform.png";
    viz_pcd(input_pcds, input_colors, is_key, img_save_path, auto_close);

    input_pcds.clear();
    input_colors.clear();
}

std::string get_last_dir(std::string path) {
    //int pos1 = path.find_last_of('\\');  // 查找最后一个反斜杠的位置
    //int pos2 = path.find_last_of('/');   // 查找最后一个正斜杠的位置
    //std::string last_dir;
    //if (pos1 != std::string::npos && pos2 != std::string::npos)
    //{
    //    // 如果path中同时存在反斜杠和正斜杠，则取最后一个出现的那个作为分隔符
    //    int pos = std::max(pos1, pos2);
    //    last_dir = path.substr(pos + 1);  // 获取最后一个目录名
    //}
    //else if (pos1 != std::string::npos)
    //{
    //    // 如果只有反斜杠，则用反斜杠作为分隔符
    //    last_dir = path.substr(pos1 + 1);
    //}
    //else if (pos2 != std::string::npos)
    //{
    //    // 如果只有正斜杠，则用正斜杠作为分隔符
    //    last_dir = path.substr(pos2 + 1);
    //}
    size_t pos = path.find_last_of("\\/");
    std::string last_dir;
    if (pos != std::string::npos) { // 如果找到了分隔符
        last_dir = path.substr(pos + 1); // 取分隔符后面的子字符串作为最后一个目录
        std::cout << last_dir << std::endl; // 输出最后一个目录
    }
    return last_dir;
}
/*
*  空格键关闭窗口
    批量处理
    第1个参数为obj_path，例："E:/JL/11547e8d8f143557525b133235812833/"
    第2个参数为obj_name，例："11547e8d8f143557525b133235812833"
*/
int main_batch(int argc, char** argv) {
    std::string obj_path = argv[1];  // E:\JL\keypoints\camera_train\02876657\331601288075868e342fe691e18bb5fd
    // 取最后一个子串，获取其最后的路径
    std::string obj_name = get_last_dir(obj_path);
    // 为了防止最后以/或\结尾，导致获得的obj_name为空, 进行检查
    if (obj_name.size() == 0) {
        auto obj_path_tmp = obj_path;
        obj_name = get_last_dir(obj_path_tmp.erase(obj_path_tmp.size() - 1));
    }
    std::cout << "obj_path:" << obj_path << std::endl;
    std::cout << "obj_name:" << obj_name << std::endl;

    // 运行easy3d
    viz_save_easy3D(obj_path, obj_name, true);
    return 1;
}

int main(int argc, char** argv) {
    std::string obj_path = argv[1];  // E:\JL\keypoints\camera_train\02876657\331601288075868e342fe691e18bb5fd
    std::string save_folder = argv[2];
    // 取最后一个子串，获取其最后的路径
    std::string obj_name = get_last_dir(obj_path);
    // 为了防止最后以/或\结尾，导致获得的obj_name为空, 进行检查
    if (obj_name.size() == 0) {
        auto obj_path_tmp = obj_path;
        obj_name = get_last_dir(obj_path_tmp.erase(obj_path_tmp.size() - 1));
    }
    std::cout << "obj_path:" << obj_path << std::endl;
    std::cout << "obj_name:" << obj_name << std::endl;

    bool auto_close = true;
    // 处理单帧
     // initialize Easy3D.
    initialize();
    // load file
    /*std::string obj_name = "11547e8d8f143557525b133235812833";
    std::string obj_path = "E:/JL/11547e8d8f143557525b133235812833/";*/
    std::string source_keypoints_path = obj_path + "preKeysinNOCS.ply";
    std::string source_pointcloud_path = obj_path + "viewPtsinNOCS.ply";

    // load ply file
    PointCloud* source_pcd = PointCloudIO::load(source_pointcloud_path);
    vec4 source_pcd_color = vec4(0.6f, 0.6f, 1.0f, 1.0f);

    PointCloud* source_key = PointCloudIO::load(source_keypoints_path);
    vec4 source_key_color = vec4(1.0f, 0.2f, 0.2f, 1.0f);

    std::vector<PointCloud*> input_pcds;
    std::vector<vec4> input_colors;
    std::vector<bool> is_key;
    std::string img_save_path = "";

    // source pts+key
    input_pcds.clear();
    input_colors.clear();
    is_key.clear();
    input_pcds.push_back(source_pcd);
    input_pcds.push_back(source_key);
    input_colors.push_back(source_pcd_color);
    input_colors.push_back(source_key_color);
    is_key.push_back(false);  // key的点更大
    is_key.push_back(true);
    img_save_path = save_folder + obj_name + ".png";
    viz_pcd(input_pcds, input_colors, is_key, img_save_path, auto_close);

    return 1;
}
