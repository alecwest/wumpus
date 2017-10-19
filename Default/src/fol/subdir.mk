################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/fol/Constant.cpp \
../src/fol/Function.cpp \
../src/fol/HornClause.cpp \
../src/fol/HornLiteral.cpp \
../src/fol/LiteralSymbols.cpp \
../src/fol/Term.cpp \
../src/fol/Unifier.cpp \
../src/fol/Variable.cpp 

OBJS += \
./src/fol/Constant.o \
./src/fol/Function.o \
./src/fol/HornClause.o \
./src/fol/HornLiteral.o \
./src/fol/LiteralSymbols.o \
./src/fol/Term.o \
./src/fol/Unifier.o \
./src/fol/Variable.o 

CPP_DEPS += \
./src/fol/Constant.d \
./src/fol/Function.d \
./src/fol/HornClause.d \
./src/fol/HornLiteral.d \
./src/fol/LiteralSymbols.d \
./src/fol/Term.d \
./src/fol/Unifier.d \
./src/fol/Variable.d 


# Each subdirectory must supply rules for building sources it contributes
src/fol/%.o: ../src/fol/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


