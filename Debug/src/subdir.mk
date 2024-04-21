################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CConnectSock.cpp \
../src/CServerSock.cpp \
../src/CSock.cpp \
../src/DataProvider.cpp \
../src/main.cpp 

OBJS += \
./src/CConnectSock.o \
./src/CServerSock.o \
./src/CSock.o \
./src/DataProvider.o \
./src/main.o 

CPP_DEPS += \
./src/CConnectSock.d \
./src/CServerSock.d \
./src/CSock.d \
./src/DataProvider.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C++ Compiler'
	arm-linux-gnueabihf-g++ -mcpu=cortex-a7 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -I"/home/rako/Data/SourceCode/eclipse_workspace/libsml/include" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


