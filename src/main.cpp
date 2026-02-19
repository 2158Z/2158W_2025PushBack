#include "main.h"
#include "config.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "selector.h"

// ASSET(pureppuresuit_testpath_txt);
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

// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder,
                                              lemlib::Omniwheel::NEW_2,
                                              config::verticalOffset);

lemlib::OdomSensors
    sensors(&vertical_tracking_wheel,   // vertical tracking wheel 1
            nullptr,                    // vertical tracking wheel 2
            nullptr, // horizontal tracking wheel 1
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

pros::Motor intake_motor(config::intakeMotor);
pros::Motor indexer(config::indexer);
pros::Motor indexer2(config::indexer2);
pros::adi::DigitalOut matchLoader1('A');
pros::adi::DigitalOut matchLoader2('C');
pros::adi::DigitalOut descore('F');

bool matchLoaderToggle = false;
bool descoreToggle = false;

void initialize() {
  left_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_motor_group.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  pros::lcd::initialize();
  chassis.calibrate();
	selector::init();

  // pros::Task screen_task([&]() {
  //   while (true) {
  //     console.printf("X: %f, Y: %f, Theta: %f \n", chassis.getPose().x,
  //                    chassis.getPose().y, chassis.getPose().theta);
  //     pros::delay(100);
  //   }
  // });
}

void disabled() {}

void competition_initialize() {}

void autonomous() { 
  switch(selector::auton) {
      case 1: // Blue 1
        chassis.setPose(-48,-48,90);
        chassis.moveToPose(-24, -48, 90, 15000);
        chassis.turnToHeading(0, 15000);
        break; // Blue 2
      case 2:
        break;
      case 3:
        break;
      case -1:
        break;
      case -2 :
        break;
      case -3:
        break;
      case 0:
        break;
      default:
        break;
  }
}

pros::Controller controller(pros::E_CONTROLLER_MASTER);
void opcontrol() {
  while (true) {
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    chassis.curvature(leftY, rightX);

    indexer.move_voltage(0);
    intake_motor.move_voltage(0);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) { // Intake
      intake_motor.move_voltage(12000);
      indexer.move_voltage(-12000);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { // Low score/outtake
      intake_motor.move_voltage(-12000);
      indexer.move_voltage(12000);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // High score
      intake_motor.move_voltage(12000);
      indexer.move_voltage(-12000);
      indexer2.move_voltage(12000);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) { // Middle Score
      intake_motor.move_voltage(12000);
      indexer.move_voltage(-12000);
      indexer2.move_voltage(-12000);
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      matchLoaderToggle = !matchLoaderToggle;
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      descoreToggle = !descoreToggle;
    }

    matchLoader1.set_value(matchLoaderToggle);
    matchLoader2.set_value(matchLoaderToggle);
    descore.set_value(descoreToggle);

    pros::delay(25);
  }
}

void testAutonFunc() {
	// chassis.setPose(-48,-48,90);
	// chassis.follow(pureppuresuit_testpath_txt, 5, 15000); // inch, ms
}
