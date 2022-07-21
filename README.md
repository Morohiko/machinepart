# machinepart

Basic machinepart realisation of self controlled camera.

More details in docs/


TODO:
make ipc for motor controller, split into 2 subsystems:
  - motor_controller(linux): should send new angle to second subs
  - motor(rtos): should generate pwm based on angle
  interface between 2 subs should be ipc(shm?)