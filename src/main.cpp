#include "main.h"
#include "config.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "robodash/views/selector.hpp"

ASSET(pureppuresuit_testpath_txt);
pros::MotorGroup left_motor_group(config::left_motors, pros::MotorGears::blue);
pros::MotorGroup right_motor_group(config::right_motors,
                                   pros::MotorGears::blue);

lemlib::Drivetrain drivetrain(&left_motor_group, &right_motor_group,
                              config::chassisTrackWidth,
                              lemlib::Omniwheel::NEW_325, config::chassisRPM,
                              2);

pros::Imu imu(config::imu);
pros::Rotation horizontal_encoder(config::horizontal_encoder);
pros::Rotation vertical_encoder(config::vertical_encoder);

// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder,
                                                lemlib::Omniwheel::NEW_2,
                                                config::horizontalOffset);

// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder,
                                              lemlib::Omniwheel::NEW_2,
                                              config::verticalOffset);

lemlib::OdomSensors
    sensors(&vertical_tracking_wheel,   // vertical tracking wheel 1
            nullptr,                    // vertical tracking wheel 2
            &horizontal_tracking_wheel, // horizontal tracking wheel 1
            nullptr,                    // horizontal tracking wheel 2
            &imu);

lemlib::ControllerSettings lateral_controller(
    config::linearKp, config::linearKi, config::linearKd,
    config::linearAntiWindup, config::linearSmallErrorTolerance,
    config::linearSmallErrorDuration, config::linearBigErrorTolerance,
    config::linearBigErrorDuration, 20);

lemlib::ControllerSettings angular_controller(
    config::angularKp, config::angularKi, config::angularKd,
    config::angularAntiWindup, config::angularSmallErrorTolerance,
    config::angularSmallErrorDuration, config::angularBigErrorTolerance,
    config::angularBigErrorDuration, 0);

lemlib::ExpoDriveCurve
    throttle_curve(3,    // joystick deadband out of 127
                   10,   // minimum output where drivetrain will move out of 127
                   1.019 // expo curve gain
    );

lemlib::ExpoDriveCurve
    steer_curve(3,    // joystick deadband out of 127
                10,   // minimum output where drivetrain will move out of 127
                1.019 // expo curve gain
    );

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller,
                        sensors);

// Color hue is an the hue value in HSV using (X, 75, 80), -1 is default color (white)
rd::Selector::routine_t blueAuton1 = {"Blue 1", nullptr, "", 220};
rd::Selector::routine_t blueAuton2 = {"Blue 2", nullptr, "", 230};
rd::Selector::routine_t blueAuton3 = {"Blue 3", nullptr, "", 240};

rd::Selector::routine_t redAuton1 = {"Red 1", nullptr, "", 0};
rd::Selector::routine_t redAuton2 = {"Red 2", nullptr, "", 5};
rd::Selector::routine_t redAuton3 = {"Red 3", nullptr, "", 10};

rd::Selector::routine_t skillsAuton = {"Skills", nullptr, "", 120};
rd::Selector::routine_t testAuton = {"Test", nullptr, "", 60};

rd::Selector selector({
    blueAuton1, blueAuton2, blueAuton3, redAuton1, redAuton2, redAuton3, skillsAuton, testAuton
});

rd::Console console;

void initialize() {
  pros::lcd::initialize();
  chassis.calibrate();

  pros::Task screen_task([&]() {
    while (true) {
      console.printf("X: %f, Y: %f, Theta: %f \n", chassis.getPose().x,
                     chassis.getPose().y, chassis.getPose().theta);
      pros::delay(100);
    }
  });
}

void disabled() {}

void competition_initialize() {}

void autonomous() { selector.run_auton(); }

pros::Controller controller(pros::E_CONTROLLER_MASTER);
void opcontrol() {
  while (true) {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis.curvature(leftY, rightX);
    pros::delay(25);
  }
}

void testAutonFunc() {
	chassis.setPose(-48,-48,90);
	chassis.follow(pureppuresuit_testpath_txt, 5, 15000); // inch, ms
}