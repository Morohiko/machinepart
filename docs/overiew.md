## TODO: Modules (required)

### Overview

Modules its part of project, must running as separate thread

### Communication through modules
### proposal 1
Communication can be like `sharing one memory`.
In that case need avoid access to that memory from different modules at the same time.
Make variable like `busy`. When write/read to memory, install variable to true, if another memory want to write/read then he need to check busy variable. Something like `condition variable`.

Machine part like a server(slave), all constrol instruction must send from glasses(android).
How it must work:

### Core (module & main thread(main() ) )
 - create thread for connecting machinepart to phone through TCP, this stream using for controlling machine part.
 - start Camera module thread(on request)
 - start Camera data transmitter thread(on request)
 - start Gyroscope data receiver thread(on request)
 - start Motor Controller thread(on request)
 
### Camera module (module & thread)
 - get stream from camera (must have access to camera)
 - convert it to bytes array (in this module because its need to do in OpenCV scope)
 - save it to Camera memory struct (as bytes array)

### Camera data transmitter (module & thread)
 - get data from Camera memory struct
 - send struct to client (glassespart) (TCP or UDP ??????)

### Gyroscope data receiver (module & thread)
 - recv data from client part as byte array (glasspart) (TCP or UDP ?????)
 - convert data to X, Y, Z coordinates (can be in Motor controller module)
 - save to Motor memory struct

### Motor controller (module & thread)
 - get data from memory struct
 - make interrupt for motor start move to possition based on memory struct (mocked cause not have real motor) ?

### Controller
 - receive configuration data from config_json(init)/shell/emulator/gp

## Demo

### Camera demo
Provide example of functional for using camera sources.
Sources used OpenCV module for access to camera, getting Mat struct from them
and convert it to bytes array.

### Wifi TCP demo

Provide example of functional for using communication with client part through TCP and send/receive plain text.

### Wifi UDP Demo

Provide example of functional for using communication with client part through UDP and send/receive plain text.

### Gyroscope data receiver demo

Provide example of functional for using receive gyroscope data from client and try parse it to x, y, z coordinates.