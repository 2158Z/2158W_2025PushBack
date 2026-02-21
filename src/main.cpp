#include "main.h"
#include "config.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "selector.h"

ASSET(skills_path1_txt);
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

void intake() {
  setDescore(true);
  intake_motor.move_voltage(12000);
  indexer.move_voltage(-12000);
  indexer2.move_voltage(12000);
}

void stopMotors(){
  intake_motor.move_voltage(0);
  indexer.move_voltage(0);
  indexer2.move_voltage(0);
}

void lowScore(){
  intake_motor.move_voltage(-12000);
  indexer.move_voltage(12000);
}

void highScore(){
  descoreToggle = false;
  intake_motor.move_voltage(12000);
  indexer.move_voltage(-12000);
  indexer2.move_voltage(12000);
}

void middleScore(){
  intake_motor.move_voltage(12000);
  indexer.move_voltage(-12000);
  indexer2.move_voltage(-12000);
}

void setMatchLoader(bool state){
  matchLoader1.set_value(state);
  matchLoader2.set_value(state);
}

void setDescore(bool state){
  descore.set_value(state);
}


void autonomous() { 
  switch(selector::auton) {
      case 1: // Blue 1
        break;
      case 2: // Blue 2
        break;
      case 3:
        break;
      case -1: // Red 1
        break;
      case -2:
        break;
      case -3:
        break;
      case 0: // Skills

        int matchLoadTime = 5000;
        int highScoreTime = 5000;

        chassis.setPose(47.25, 0, 90, 2000);
        chassis.moveToPose(-48, -48, 270, 2000); // Getting Ready to Load
        intake();
        setMatchLoader(true);
        pros::delay(200);
        chassis.moveToPose(-64, -48, 270, 2000);
        pros::delay(matchLoadTime); //wait for matchload
        setMatchLoader(false);
        pros::delay(200);
        chassis.moveToPose(-39, -60, 90, 2000);
        stopMotors();
        chassis.moveToPose(27, -60, 45, 2000);
        chassis.moveToPose(38, -52, 315, 2000);
        chassis.moveToPose(27, -48, 270, 2000); //highScore 
        highScore();
        pros::delay(highScoreTime);
        setMatchLoader(true); //ready to load
        pros::delay(200);
        chassis.moveToPose(62, -48, 90, 2000); //matchload
        pros::delay(matchLoadTime); 
        setMatchLoader(false);
        pros::delay(200);
        chassis.moveToPose(27, -48, 270, 2000); //highScore
        highScore();
        pros::delay(highScoreTime);
        stopMotors();
        chassis.moveToPose(24, -36, 0, 2000, {.maxSpeed=90}); //before approaching balls
        intake();
        chassis.turnToHeading(0, 2000);
        chassis.moveToPose(24, -24, 0, 2000, {.maxSpeed=90});
        setMatchLoader(true); //catch balls on floor
        pros::delay(200);
        setMatchLoader(false);
        pros::delay(200);
        chassis.moveToPose(24, 12, 0, 2000, {.maxSpeed=90});
        chassis.moveToPose(24, 24, 0, 2000, {.maxSpeed=90});
        setMatchLoader(true); //catch balls on floor
        pros::delay(200);
        chassis.moveToPose(35, 48, 270, 2000);
        highScore();


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
    indexer2.move_voltage(0);
    intake_motor.move_voltage(0);

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) { // Intake
      descoreToggle = true;
      intake_motor.move_voltage(12000);
      indexer.move_voltage(-12000);
      indexer2.move_voltage(12000);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) { // Low score/outtake
      intake_motor.move_voltage(-12000);
      indexer.move_voltage(12000);
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) { // High score
      descoreToggle = false;
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
