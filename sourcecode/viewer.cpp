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

#include "viewer.h"
#include <easy3d/core/surface_mesh.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/drawable_triangles.h>
#include <easy3d/renderer/ambient_occlusion.h>
#include <easy3d/renderer/shader_manager.h>
#include <easy3d/renderer/shader_program.h>
#include <easy3d/renderer/shape.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/util/setting.h>

#include <3rd_party/glfw/include/GLFW/glfw3.h>	// for the KEYs

//
//
//#include <easy3d/viewer/viewer.h>
//
//#include <chrono>
//#include <iostream>
//
//#include <easy3d/renderer/opengl.h>        // Initialize with glewInit()
//#include <3rd_party/glfw/include/GLFW/glfw3.h>    // Include glfw3.h after our OpenGL definitions

// test
//#include <easy3d/core/surface_mesh.h>
//#include <easy3d/core/graph.h>
//#include <easy3d/core/point_cloud.h>
//#include <easy3d/core/poly_mesh.h>
//#include <easy3d/renderer/renderer.h>
//#include <easy3d/renderer/manipulator.h>
//#include <easy3d/renderer/drawable_points.h>
//#include <easy3d/renderer/drawable_lines.h>
//#include <easy3d/renderer/drawable_triangles.h>
//#include <easy3d/renderer/shader_program.h>
//#include <easy3d/renderer/shader_manager.h>
//#include <easy3d/renderer/transform.h>
//#include <easy3d/renderer/shape.h>
//#include <easy3d/renderer/camera.h>
//#include <easy3d/renderer/manipulated_camera_frame.h>
//#include <easy3d/renderer/key_frame_interpolator.h>
//#include <easy3d/renderer/framebuffer_object.h>
//#include <easy3d/renderer/opengl_util.h>
//#include <easy3d/renderer/opengl_error.h>
//#include <easy3d/renderer/text_renderer.h>
//#include <easy3d/renderer/texture_manager.h>
//#include <easy3d/fileio/point_cloud_io.h>
//#include <easy3d/fileio/graph_io.h>
//#include <easy3d/fileio/surface_mesh_io.h>
//#include <easy3d/fileio/poly_mesh_io.h>
//#include <easy3d/fileio/ply_reader_writer.h>
//#include <easy3d/fileio/point_cloud_io_ptx.h>
//#include <easy3d/util/dialog.h>
//#include <easy3d/util/file_system.h>
//#include <easy3d/util/logging.h>
//#include <easy3d/util/timer.h>
//#include <easy3d/util/string.h>
//#include <easy3d/util/resource.h>
//#include <easy3d/util/setting.h>



using namespace easy3d;


bool myViewer::key_press_event(int key, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        exit();
        update();
        return true;
    }
    return myViewer::key_press_event(key, modifiers);
}


// override easy3Dd::viewer的run函数, 以便能够自动关闭窗口
int myViewer::run_auto_close(bool see_all) {
    //// initialize before showing the window because it can be slow
    //init();

    //// make sure scene fits the screen when the window appears
    //if (see_all)
    //    fit_screen();

    //// show the window
    //glfwShowWindow(window_);

    //try {   // main loop
    //    static int frame_counter = 0;
    //    double last_time = glfwGetTime();   // for frame rate counter

    //    while (!glfwWindowShouldClose(window_) && !should_exit_) {
    //        if (!glfwGetWindowAttrib(window_, GLFW_VISIBLE)) { // not visible
    //            glfwWaitEvents();
    //            continue;
    //        }

    //        if (show_frame_rate_) {
    //            // Calculate ms/frame
    //            double current_time = glfwGetTime();
    //            ++frame_counter;
    //            if (current_time - last_time >= 1.0f) {
    //                sprintf(gpu_time_, "fps: %2.0f (%4.1f ms/frame)",
    //                    double(frame_counter) / (current_time - last_time), 1000.0 / double(frame_counter));
    //                frame_counter = 0;
    //                last_time = current_time;
    //            }
    //        }

    //        pre_draw();
    //        draw();
    //        post_draw();
    //        glfwSwapBuffers(window_);

    //        // Don't call 'glfwPollEvents()' at the beginning of the main loop.
    //        // Reason: first frame needs time to complete.
    //        if (is_animating_ && animation_func_) {
    //            glfwPollEvents();

    //            // TODO: make framerate a parameter
    //            if (!show_frame_rate_) {
    //                static const int animation_fps = 30;
    //                static const double interval = 1000.0 / (animation_fps + 5); // the extra 5 for adjusting
    //                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(interval)));
    //            }
    //            animation_func_(this);
    //        }
    //        else if (show_frame_rate_)
    //            glfwPollEvents();
    //        else
    //            glfwWaitEvents();
    //        // auto exit
    //        should_exit_ = true;
    //        update();

    //    }

    //    /* Process events once more */
    //    glfwPollEvents();
    //    return EXIT_SUCCESS;
    //}
    //catch (const std::exception& e) {
    //    LOG(ERROR) << "Caught exception in main loop: " << e.what();
    //    cleanup();
    //    return EXIT_FAILURE;
    //}
    return 0;
}

void myViewer::auto_close() {
    // auto exit
    should_exit_ = true;
    update();
}