////////////////////////////////////////////////////////////////////////////////
//  Description: This file contains the DataPackage class
//
//  Modified: Carlos Anzola (23 Feb 2023)
////////////////////////////////////////////////////////////////////////////////

#include <vector>

// Define an enumeration to track errors in the data package
enum class Data_Package_Error {
  DATA_PACKAGE_ERROR_NONE,
  DATA_PACKAGE_ERROR_CAMERA_NOT_WORKING,
  DATA_PACKAGE_ERROR_TOO_MANY_OBJECTS,
  DATA_PACKAGE_ERROR_TOO_MANY_CLASSES,
  DATA_PACKAGE_ERROR_MISMATCHED_CLASSES_OBJECTS,
};

class DataPackage {
public:
  // Public attributes
  bool isCameraWorking;
  unsigned int numObjectsInPlatform;
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
    return;
  }
  // Method to parse the data package from an input array
  void parse_data_package(uint8_t* input_array, size_t input_size) {
    // Reset the attributes and status flag
    reset_data_package();
    
    // Parse the data package
    if (input_size >= 3) {
      // Parse camera status
      isCameraWorking = (input_array[0] != 0);
      
      // Parse number of objects
      numObjectsInPlatform = input_array[1];
      if (numObjectsInPlatform < 0 || numObjectsInPlatform > 2) {
        status = Data_Package_Error::DATA_PACKAGE_ERROR_TOO_MANY_OBJECTS;
        return;
      }
      
      // Parse classes of objects and their orientation
      for (int i = 0; i < numObjectsInPlatform; i++) {
        int class_of_object = input_array[2 + i*2];
        int orientation_of_object = input_array[2 + i*2 + 1];
        
        if (class_of_object < 0 || class_of_object > 255) {
          status = Data_Package_Error::DATA_PACKAGE_ERROR_TOO_MANY_CLASSES;
          return;
        }
        
        if (orientation_of_object < 0 || orientation_of_object > 255) {
          status = Data_Package_Error::DATA_PACKAGE_ERROR_MISMATCHED_CLASSES_OBJECTS;
          return;
        }
        
        classObjectsInPlatform.push_back(class_of_object);
        orientationObjectsInPlatform.push_back(orientation_of_object);
      }
      
      if (classObjectsInPlatform.size() != numObjectsInPlatform ||
          orientationObjectsInPlatform.size() != numObjectsInPlatform) {
        status = Data_Package_Error::DATA_PACKAGE_ERROR_MISMATCHED_CLASSES_OBJECTS;
        return;
      }
    } else {
      // Input array is too small
      status = Data_Package_Error::DATA_PACKAGE_ERROR_TOO_MANY_CLASSES;
      return;
    }
  }

  // Public attribute to track statuss
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
