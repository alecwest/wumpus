################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Agent.cpp \
../src/AgentRoom.cpp \
../src/AgentWorld.cpp \
../src/GameRoom.cpp \
../src/GameWorld.cpp \
../src/IntelligentAgent.cpp \
../src/NullRoom.cpp \
../src/PlayerAgent.cpp \
../src/Room.cpp \
../src/World.cpp \
../src/main.cpp 

OBJS += \
./src/Agent.o \
./src/AgentRoom.o \
./src/AgentWorld.o \
./src/GameRoom.o \
./src/GameWorld.o \
./src/IntelligentAgent.o \
./src/NullRoom.o \
./src/PlayerAgent.o \
./src/Room.o \
./src/World.o \
./src/main.o 

CPP_DEPS += \
./src/Agent.d \
./src/AgentRoom.d \
./src/AgentWorld.d \
./src/GameRoom.d \
./src/GameWorld.d \
./src/IntelligentAgent.d \
./src/NullRoom.d \
./src/PlayerAgent.d \
./src/Room.d \
./src/World.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


