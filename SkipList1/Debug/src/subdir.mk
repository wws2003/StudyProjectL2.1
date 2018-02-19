################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IntSkipListImpl.cpp \
../src/SkipList1.cpp \
../src/SkipListImpl.cpp \
../src/SkipListStack.cpp 

OBJS += \
./src/IntSkipListImpl.o \
./src/SkipList1.o \
./src/SkipListImpl.o \
./src/SkipListStack.o 

CPP_DEPS += \
./src/IntSkipListImpl.d \
./src/SkipList1.d \
./src/SkipListImpl.d \
./src/SkipListStack.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__CPLUSPLUS=20113L -D__cplusplus=201103L -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


