#include "gaze.h"
#include "actionUnits.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>


// -----------------------
// Python type conversions
// -----------------------

namespace py = pybind11;

py::tuple point3f_to_tuple(const cv::Point3f& p) 
{
    return py::make_tuple(p.x, p.y, p.z);
}

cv::Point3f tuple_to_point3f(const py::tuple& t) 
{
    return cv::Point3f(t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>());
}

py::tuple vec2d_to_tuple(const cv::Vec2d& v) 
{
    return py::make_tuple(v[0], v[1]);
}

cv::Vec2d tuple_to_vec2d(const py::tuple& t) 
{
    return cv::Vec2d(t[0].cast<double>(), t[1].cast<double>());
}

cv::Mat numpy_to_mat(const py::array_t<uint8_t>& arr)
{
    py::buffer_info buf_info = arr.request();

    size_t height = buf_info.shape[0];
    size_t width = buf_info.shape[1];
    size_t channels = buf_info.shape[2];

    // Very important - Python equivalent of cv::Mat uses 3 channels
    return cv::Mat(height, width, CV_8UC3, (uint8_t*)buf_info.ptr);
}

cv::Rect_<float> tuple_to_rect(const py::tuple& t) 
{
    return cv::Rect_<float>(t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>(), t[3].cast<float>());
}


// ---------------
// Python bindings
// ---------------

PYBIND11_MODULE(GazeTracking, handle) {
    // TODO: add some more docs about API functions
    handle.doc() = "A gaze tracking module to interact with images or video frames.";

    // Gaze structure binding
    py::class_<Gaze>(handle, "Gaze")
        .def(py::init<cv::Point3f, cv::Point3f, cv::Point3f, cv::Point3f, cv::Vec2d>())
        .def_property("eye1",
            [](const Gaze& self) { return point3f_to_tuple(self.eye1); },
            [](Gaze& self, const py::tuple& t) { self.eye1 = tuple_to_point3f(t); })
        .def_property("direction1",
            [](const Gaze& self) { return point3f_to_tuple(self.direction1); },
            [](Gaze& self, const py::tuple& t) { self.direction1 = tuple_to_point3f(t); })
        .def_property("eye2",
            [](const Gaze& self) { return point3f_to_tuple(self.eye2); },
            [](Gaze& self, const py::tuple& t) { self.eye2 = tuple_to_point3f(t); })
        .def_property("direction2",
            [](const Gaze& self) { return point3f_to_tuple(self.direction2); },
            [](Gaze& self, const py::tuple& t) { self.direction2 = tuple_to_point3f(t); })
        .def_property("angle",
            [](const Gaze& self) { return vec2d_to_tuple(self.angle); },
            [](Gaze& self, const py::tuple& t) { self.angle = tuple_to_vec2d(t); })
        .def("__repr__", [](const Gaze& gaze) -> std::string {
            return gaze.toString(); });

    // Gaze extractor interface binding
    py::class_<GazeExtractor>(handle, "GazeExtractor")
        .def(py::init<bool, std::optional<bool>, std::optional<bool>, std::optional<bool>,
                      std::optional<int>, std::optional<float>, std::optional<float>>(),
             py::arg("ld_video_mode") = false, py::arg("wild") = std::nullopt, py::arg("multi_view") = std::make_optional(true),
             py::arg("limit_pose") = std::nullopt, py::arg("n_iter") = std::nullopt,
             py::arg("reg_factor") = std::nullopt, py::arg("weight_factor") = std::nullopt,
             "Constructor for GazeExtractor.\n\n"
             ":param ld_video_mode: decides to use DetectLandmarksInVideo() or DetectLandmarksInImage() function.\n"
             ":param wild: a specyfic set of parameters for tough conditions (various lighting, incomplete view of the face).\n"
             ":param multi_view: (Recommended) decides whether to consider multiple views during model reinit.\n"
             ":param limit_pose: should pose be limited to 180 degrees frontal.\n"
             ":param n_iter: number of optimization iterations.\n"
             ":param reg_factor: regularization parameter.\n"
             ":param weight_factor: refers to how much weight is applied to certain constraints during the optimization process.")
        .def("set_camera_calibration", &GazeExtractor::setCameraCalibration,
             py::arg("fx"), py::arg("fy"), py::arg("cx"), py::arg("cy"))
        .def("estimate_camera_calibration", [](GazeExtractor& self, const py::array_t<uint8_t>& frame) -> void {
            cv::Mat frameMat = numpy_to_mat(frame);
            self.estimateCameraCalibration(frameMat);
        }, py::arg("frame"))
        .def("detect_gaze", [](GazeExtractor& self, const py::array_t<uint8_t>& frame, double timestamp, const py::tuple& roi) -> std::optional<Gaze> {
            Frame frameMat = numpy_to_mat(frame);
            BoundingBox bbox = tuple_to_rect(roi);
            return self.detectGaze(frameMat, timestamp, bbox);
        }, py::arg("frame"), py::arg("timestamp"), py::arg("roi"));
    
    // Action unit extractor interface binding
    py::class_<AUExtractor>(handle, "AUExtractor")
        .def(py::init<bool, bool, std::optional<bool>, std::optional<bool>, std::optional<bool>,
                      std::optional<int>, std::optional<float>, std::optional<float>>(),
             py::arg("ld_video_mode") = false, py::arg("fa_video_mode") = false,
             py::arg("wild") = std::nullopt, py::arg("multi_view") = std::make_optional(true),
             py::arg("limit_pose") = std::nullopt, py::arg("n_iter") = std::nullopt,
             py::arg("reg_factor") = std::nullopt, py::arg("weight_factor") = std::nullopt,
             "Constructor for AUExtractor.\n\n"
             ":param ld_video_mode: decides to use DetectLandmarksInVideo() or DetectLandmarksInImage() function.\n"
             ":param fa_video_mode: decides to use AddNextFrame() or PredictStaticAUsAndComputeFeatures() function.\n"
             ":param wild: a specyfic set of parameters for tough conditions (various lighting, incomplete view of the face).\n"
             ":param multi_view: (Recommended) decides whether to consider multiple views during model reinit.\n"
             ":param limit_pose: should pose be limited to 180 degrees frontal.\n"
             ":param n_iter: number of optimization iterations.\n"
             ":param reg_factor: regularization parameter.\n"
             ":param weight_factor: refers to how much weight is applied to certain constraints during the optimization process.")
        .def("detect_action_units", [](AUExtractor& self, const py::array_t<uint8_t>& frame, double timestamp, const py::tuple& roi) -> std::vector<std::pair<std::string, double>> {
            Frame frameMat = numpy_to_mat(frame);
            BoundingBox bbox = tuple_to_rect(roi);
            return self.detectActionUnits(frameMat, timestamp, bbox);
        }, py::arg("frame"), py::arg("timestamp"), py::arg("roi"));
}