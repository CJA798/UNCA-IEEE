////////////////////////////////////////////////////////////////////////////////
//  Description: This file contains the DataPackage class
//
//  Modified: Carlos Anzola (23 Feb 2023)
////////////////////////////////////////////////////////////////////////////////
//  Class ID:
//    0 = Pink_Duck
//    1 = Yellow_Duck
//    2 = Red_Pillar
//    3 = Green_Pillar
//    4 = White_Pillar
//
//  Data string format:
//    isCameraOn,numObjectsInPlatform,classObjectsInPlatform,orientationObjectsInPlatform
//    - Example:
//        camera works, 2 objects: pink duck at 90°, and green pillar at 38°
//        String raw_data = "1,2,0,90,3,38"
//    - NOTE: class and orientation are alternating
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "macros.h"

// Define an enumeration to track errors in the data package
enum class Data_Package_Error {
  DATA_PACKAGE_ERROR_NONE,
  DATA_PACKAGE_ERROR_CAMERA_NOT_WORKING,
  //DATA_PACKAGE_ERROR_TOO_MANY_OBJECTS,
  DATA_PACKAGE_ERROR_INVALID_NUMBER_OF_OBJECTS,
  //DATA_PACKAGE_ERROR_TOO_MANY_CLASSES,
  DATA_PACKAGE_ERROR_INVALID_CLASS_ID,
  DATA_PACKAGE_ERROR_MISMATCHED_CLASSES_OBJECTS,
  DATA_PACKAGE_ERROR_NULL_POINTER,
};

class DataPackage {
public:
  // Public attributes
  bool isCameraWorking;
  int numObjectsInPlatform;
  std::vector<int> classObjectsInPlatform;
  std::vector<int> orientationObjectsInPlatform;
  
  // Constructor to initialize the class attributes
  DataPackage() :
    isCameraWorking(false),
    numObjectsInPlatform(0),
    classObjectsInPlatform(),
    orientationObjectsInPlatform(),
    status(Data_Package_Error::DATA_PACKAGE_ERROR_NONE)
  {}

  // Method to clear data package
  void reset_data_package(){
    isCameraWorking = false;
    numObjectsInPlatform = 0;
    classObjectsInPlatform.clear();
    orientationObjectsInPlatform.clear();
    status = Data_Package_Error::DATA_PACKAGE_ERROR_NONE;
  }
  
  // Method to parse the data package from an input array
  void parse_data_package(uint8_t* input_array, size_t input_size) {
    reset_data_package();
    // Parse input string into tokens
    char input_string[MAX_INPUT_SIZE];
    strncpy(input_string, (char*) input_array, input_size);
    input_string[input_size] = '\0';
    
    // Parse camera status
    char* token = strtok(input_string, ",");    
    if (token == NULL) {
      // Invalid input string
      status = Data_Package_Error::DATA_PACKAGE_ERROR_NULL_POINTER;
      return;
    }
    this->isCameraWorking = atoi(token);
    if (isCameraWorking == 0) {
      status = Data_Package_Error::DATA_PACKAGE_ERROR_CAMERA_NOT_WORKING;
      return;
    } 

    // Parse number of objects
    token = strtok(NULL, ",");
    if (token == NULL) {
      // Invalid input string
      status = Data_Package_Error::DATA_PACKAGE_ERROR_NULL_POINTER;
      return;
    }
    this->numObjectsInPlatform = atoi(token);
    if (this->numObjectsInPlatform < 0 || this->numObjectsInPlatform > 17) {
      status = Data_Package_Error::DATA_PACKAGE_ERROR_INVALID_NUMBER_OF_OBJECTS;
      return;
    }
    
    // Parse classes and orientations of objects
    for (int i = 0; i < this->numObjectsInPlatform; i++) {
      // Parse class of object
      token = strtok(NULL, ",");
      if (token == NULL) {
        // Invalid input string
        status = Data_Package_Error::DATA_PACKAGE_ERROR_NULL_POINTER;
        return;
      }
      int class_of_object = atoi(token);
      if (class_of_object < 0 || class_of_object > 4) {
      status = Data_Package_Error::DATA_PACKAGE_ERROR_INVALID_CLASS_ID;
      return;
    }

      // Parse orientation of object
      token = strtok(NULL, ",");
      if (token == NULL) {
        // Invalid input string
        status = Data_Package_Error::DATA_PACKAGE_ERROR_NULL_POINTER;
        return;
      }
      int orientation_of_object = atoi(token);
      
      this->classObjectsInPlatform.push_back(class_of_object);
      this->orientationObjectsInPlatform.push_back(orientation_of_object);
    }
  }
  
  // Public attribute to track status
  Data_Package_Error status;

  // Getter for isCameraWorking attribute
  bool getIsCameraWorking() const {
    return isCameraWorking;
  }

  // Getter for numObjectsInPlatform attribute
  int getNumObjectsInPlatform() const {
    return numObjectsInPlatform;
  }

  // Getter for classObjectsInPlatform attribute
  const std::vector<int>& getClassObjectsInPlatform() const {
    return classObjectsInPlatform;
  }

  // Getter for orientationObjectsInPlatform attribute
  const std::vector<int>& getOrientationObjectsInPlatform() const {
    return orientationObjectsInPlatform;
  }

  // Getter for status attribute
  Data_Package_Error getStatus() const {
    return status;
  }

};
