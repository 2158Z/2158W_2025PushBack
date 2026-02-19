#include <cstdint>
#include <vector>
namespace config {
    static std::vector<int8_t> left_motors = {-11, -12, -13};
    static std::vector<int8_t> right_motors = {14, 15, 16};
    static int intakeMotor = 8;
    static int indexer = 9;
    static int indexer2 = 10;

    static int chassisRPM = 450;
    static double chassisTrackWidth = 15.5;

    static int vertical_encoder = 2;  
    static float verticalOffset = -4; // From Tracking Center
    static int horizontal_encoder = 0;
    static float horizontalOffset = 0; // From Tracking Center
    static int imu = 3;
    
    static float angularKp = 10;
    static float angularKi = 0;
    static float angularKd = 3;
    static float angularAntiWindup = 3;
    static float angularSmallErrorTolerance = 1; // Inches
    static float angularSmallErrorDuration = 100; // milliseconds
    static float angularBigErrorTolerance = 3; // Inches
    static float angularBigErrorDuration = 500; // milliseconds

    static float linearKp = 10;
    static float linearKi = 0;
    static float linearKd = 3;
    static float linearAntiWindup = 3;
    static float linearSmallErrorTolerance = 1; // Inches
    static float linearSmallErrorDuration = 100; // milliseconds
    static float linearBigErrorTolerance = 3; // Inches
    static float linearBigErrorDuration = 500; // milliseconds
}