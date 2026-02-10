#include <cstdint>
#include <vector>
namespace config {
    static std::vector<int8_t> left_motors = {-1, 2, -3};
    static std::vector<int8_t> right_motors = {4, -5, 6};
    static int chassisRPM = 450;
    static int chassisTrackWidth = 10;

    static int vertical_encoder = 0;
    static float verticalOffset = -2.5; // From Tracking Center
    static int horizontal_encoder = 0;
    static float horizontalOffset = -5.75; // From Tracking Center
    static int imu = 0;
    
    static float angularKp = 0;
    static float angularKi = 0;
    static float angularKd = 0;
    static float angularAntiWindup = 0;
    static float angularSmallErrorTolerance = 0; // Inches
    static float angularSmallErrorDuration = 0; // milliseconds
    static float angularBigErrorTolerance = 0; // Inches
    static float angularBigErrorDuration = 0; // milliseconds

    static float linearKp = 0;
    static float linearKi = 0;
    static float linearKd = 0;
    static float linearAntiWindup = 0;
    static float linearSmallErrorTolerance = 0; // Inches
    static float linearSmallErrorDuration = 0; // milliseconds
    static float linearBigErrorTolerance = 0; // Inches
    static float linearBigErrorDuration = 0; // milliseconds
}